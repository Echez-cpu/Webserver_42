/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetActionResponse.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pokpalae <pokpalae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:28:41 by pokpalae          #+#    #+#             */
/*   Updated: 2025/07/13 00:32:46 by pokpalae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/GetActionResponse.hpp"

#include <fstream>
#include <cstdio>
#include <string>
#include <iterator>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/select.h>


// Constructors
GetActionResponse::GetActionResponse() {}

GetActionResponse::GetActionResponse(ServerConfiguration* serverBlock) {
    _host = serverBlock->getHostname();
}

GetActionResponse::~GetActionResponse() {}


std::string generateSessionID(size_t length = 16) {
    const std::string chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string sessionID;
    for (size_t i = 0; i < length; ++i)
        sessionID += chars[rand() % chars.size()];
    return sessionID;
}



void GetActionResponse::setCookie(ClientRequest& request)
{
    if (request.getCookieValue("session_id").empty()) 
    {

        std::string newSessionId = generateSessionID();
        
        // No session, set a new one
        addSetCookieHeader("session_id", newSessionId); // ok put your eyes here please
    }
    
    else 
    {
        std::cout << COLOR_MAGENTA << "Returning user with session: " << request.getCookieValue("session_id") << " Welcome back" << COLOR_RESET  << std::endl;
    }
}

std::string generateDirectoryListing(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (!dir)
        return std::string("<h1>Cannot open directory</h1>");

    std::string listing;
    listing += "<html><body><ul>";

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string name(entry->d_name);
        listing += "<li><a href=\"";
        listing += name;
        listing += "\">";
        listing += name;
        listing += "</a></li>";
    }

    listing += "</ul></body></html>";
    closedir(dir);
    std::cout << "Directory listing HTML hahahah: " << listing << std::endl;
    return listing;
}





std::string executePhpScript(const std::string& scriptPath, int timeoutSeconds = 5) {
    std::string result;
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return "Error creating pipe";

    pid_t pid = fork();
    if (pid == -1)
        return "Error forking";

    if (pid == 0) {
        // Child process
        close(pipefd[0]); // Close unused read end

        // Redirect stdout and stderr to pipe write end
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        // Execute PHP, ensure you have correct path or just "php" to use PATH
        execlp("php", "php", scriptPath.c_str(), (char*)NULL);

        // If exec fails:
        _exit(127);
    }

    // Parent process
    close(pipefd[1]); // Close unused write end

    // Set pipe to non-blocking
    int flags = fcntl(pipefd[0], F_GETFL, 0);
    fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

    fd_set readfds;
    struct timeval timeout;
    timeout.tv_sec = timeoutSeconds;
    timeout.tv_usec = 0;

    bool timeoutOccurred = false;
    bool doneReading = false;

    while (!doneReading && !timeoutOccurred) {
        FD_ZERO(&readfds);
        FD_SET(pipefd[0], &readfds);

        int ret = select(pipefd[0] + 1, &readfds, NULL, NULL, &timeout);
        if (ret > 0) {
            if (FD_ISSET(pipefd[0], &readfds)) {
                char buffer[1024];
                ssize_t count = read(pipefd[0], buffer, sizeof(buffer));
                if (count > 0) 
                {
                    result.append(buffer, count);
                    // Reset timeout for more data
                    timeout.tv_sec = timeoutSeconds;
                    timeout.tv_usec = 0;
                } 
                else if (count == 0) 
                {
                    // EOF
                    doneReading = true;
                } 
                 else if (count <= 0) // uncomment if the evaluator doesn't agree...
                 {
                     doneReading = true;
                 }
                
              // else if (count == -1 && errno != EAGAIN) {           
                    // doneReading = true;
                // }
            }
        } else if (ret == 0) {
            // Timeout expired
            timeoutOccurred = true;
            kill(pid, SIGKILL);
            result = "<h1>504 Gateway Timeout</h1><p>CGI took too long.</p>";
        } else {
            // select error
            doneReading = true;
        }
    }

    close(pipefd[0]);

    // Wait for child to avoid zombies
    int status;
    waitpid(pid, &status, 0);

    if (result.find("PHP Parse error") != std::string::npos ||
        result.find("Fatal error") != std::string::npos)
        {
        
            return  "<h1>500 Internal Server Error</h1><p>CGI crashed.</p>";
        }

    return result;
}



void GetActionResponse::setRawBody() {
    std::ifstream fileStream(_resource.c_str());
    std::string fileContent((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

    if (hasFileExtension(_resource, ".html") || hasFileExtension(_resource, ".php"))
    {
        _raw_body = executePhpScript(_resource);
    } 
    
    else {
        _raw_body = fileContent;
    }

    addContentLengthToHeaders();
}

// Sets the required HTTP headers for the response
void GetActionResponse::setHeaders() {
    addDateHeader();
    addConnectionHeader("keep-alive");
    addContentTypeHeader();
    addRetryAfterHeader(2); // Optional retry header
    setHostHeader(_host.c_str());
}


void GetActionResponse::constructResponse(ClientRequest& request) {   

    initializeResourceFromRequest(request);

std::cout << "[DEBUG] isDirectory: " << isDirectory(_resource) << std::endl;

LocationBlock* loc = request.getMatchedLocationBlock();
   if (!loc)
    {
     std::cout << "[DEBUG] No location block matched!" << std::endl;
    }
 
    else
    {

        std::cout << "[DEBUG] Autoindex enabled: " << loc->isDirectoryListingEnabled() << std::endl;
    }
    
    if (isDirectory(_resource)) {
        LocationBlock* loc = request.getMatchedLocationBlock();
        if (loc) 
        {
            // If index doesn't exist, serve listing (if allowed)
            if (loc->isDirectoryListingEnabled()) {
                _raw_status_line = Http_version_ + " 200 OK\r\n";
                _raw_body = generateDirectoryListing(_resource);
                std::cout << "Directory listing HTML: " << "I got here it's legal!!" << std::endl;
                setHeaders();
                setCookie(request);
                addContentLengthToHeaders();
                composeHeaderString();
                setServerReply();
                return;
            }

            // Neither index nor autoindex == 403
            _raw_status_line = Http_version_ + " 403 Forbidden\r\n";
            _raw_body = "<h1>Directory listing forbidden</h1>";
            addContentLengthToHeaders();
            setCookie(request);
            composeHeaderString();
            setServerReply();

            std::cout << "Directory listing HTML: " << "I should never reach her sir illegal!!" << std::endl;
            return;
        }
    }





    // Fallback: normal file serving
    _raw_status_line = Http_version_ + " 200 OK\r\n";
    setHeaders();
    setCookie(request);
    setRawBody();
   
    if (_raw_body.find("<h1>500 Internal Server Error</h1><p>CGI crashed.</p>") != std::string::npos)
    {
        _raw_status_line = Http_version_ + " 500 Internal Server Error\r\n";
         setHeaders();
         setCookie(request);
         composeHeaderString();
         setServerReply();
         return;
    }

  if (_raw_body.find("<h1>504 Gateway Timeout</h1><p>CGI took too long.</p>") != std::string::npos)     
    {
        
         _raw_status_line = Http_version_ + " 504 Gateway Timeout\r\n";
         setHeaders();
         setCookie(request);
         composeHeaderString();
         setServerReply();
         return;
    }  
    
    composeHeaderString();
    setServerReply();
}




// Placeholder for default error response with custom body
void GetActionResponse::constructDefaultResponseWithBody(ClientRequest& request, const str& bodyContent) {
    (void)request;
    (void)bodyContent;
}

// Placeholder for serving a configured response from a file path
void GetActionResponse::constructConfigurationResponse(ClientRequest& request, const str& targetFilePath) {
    (void)request;
    (void)targetFilePath;
}


