/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostActionResponse.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pokpalae <pokpalae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:28:41 by pokpalae          #+#    #+#             */
/*   Updated: 2025/07/13 00:32:46 by pokpalae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/PostActionResponse.hpp"

PostActionResponse::PostActionResponse() {}

PostActionResponse::PostActionResponse(ServerConfiguration* serverBlock)
    : _rootDirectory(serverBlock->getDocumentRoot()), _host(serverBlock->getHostname()) {}

PostActionResponse::~PostActionResponse() {}



std::string retainDigits(const std::string& input) {
    std::string output = "";
    output.reserve(input.length()); // expandable

    std::string::const_iterator it = input.begin();
    while (it != input.end()) {
        if (std::isdigit(*it))
            output += *it;
        ++it;
    }

    return output;
}

void copyFile(const std::string& source, const std::string& destination) {
    std::ifstream src(source.c_str(), std::ios::binary);
    std::ofstream dst(destination.c_str(), std::ios::binary);

    if (!src.is_open() || !dst.is_open()) {
        std::perror(("Failed to open file for copy from " + source + " to " + destination).c_str());
        return;
    }

    dst << src.rdbuf();
}



void extractAdDetailsFromForm(std::ifstream& tmpFile) {


str currentLine; str boundaryLine; str fieldName; str fieldValue;
str imageData; str adDescription; str adPrice; str contactPhone;

    // Read the first line (boundary separator)
    std::getline(tmpFile, boundaryLine);

    while (tmpFile.good()) {
        std::getline(tmpFile, currentLine);

        // Parse Content-Disposition header
        if (currentLine.find("Content-Disposition: form-data; name=") != std::string::npos) {
            std::size_t namePos = currentLine.find("name=");
            if (namePos != std::string::npos) {
                fieldName = currentLine.substr(namePos + 6);
                std::size_t quoteEnd = fieldName.find("\"");
                if (quoteEnd != std::string::npos)
                    fieldName = fieldName.substr(0, quoteEnd);
            }

            std::getline(tmpFile, currentLine); // Either Content-Type or blank line

            if (fieldName != "picture")
                std::getline(tmpFile, currentLine); // Actual value line

            fieldValue = currentLine;

            // Remove trailing carriage return if any
            if (!fieldValue.empty() && fieldValue[fieldValue.size() - 1] == '\r')
                fieldValue.erase(fieldValue.size() - 1);
        }

        // Store values
        if (fieldName == "description") {
            adDescription = fieldValue;
        } else if (fieldName == "price") {
            adPrice = fieldValue;
        } else if (fieldName == "phone") {
            contactPhone = fieldValue;
        } else if (fieldName == "picture") {
            // Handle image binary
            if (currentLine.find("Content-Type: image/jpeg") != std::string::npos) {
                std::getline(tmpFile, currentLine); // Skip blank line

                std::string boundaryToken = retainDigits(boundaryLine);
                while (std::getline(tmpFile, currentLine)) {
                    if (currentLine.find(boundaryToken) != std::string::npos)
                        break;

                    imageData.append(currentLine);
                    imageData.append("\n");
                }
            } else {
                break;
            }
        }
    }

    // Save form data
    std::ofstream outputData("t_deleted/data-entry.txt");
    outputData << "description=" << adDescription << "&price=" << adPrice << "&phone=" << contactPhone;
    outputData.close();

    // Save image file
    if (!imageData.empty()) {
        std::ofstream outputImage("t_deleted/pic-entry.jpeg", std::ios::binary);
        outputImage.write(imageData.c_str(), imageData.size());
        outputImage.close();
    }
}





void moveAndRenameFile(const std::string& from, const std::string& to) {
    if (rename(from.c_str(), to.c_str()) != 0) {
        std::perror(("Error renaming " + from + " to " + to).c_str());
    }
}

 // anonymous namespace

void PostActionResponse::executePostResponse(ClientRequest& req) {
    
    std::ofstream   tmpFile;
    std::ifstream   inputFile;
    const char*     file_path = "t_deleted/data-entry.txt";
    std::string     command;

    tmpFile.open(file_path);
    tmpFile << req.getBodyContent();
    tmpFile.close();

    inputFile.open(file_path);
    extractAdDetailsFromForm(inputFile);
    inputFile.close();

    const char*     filePath2 = "t_deleted/id_file";
    command = "php basic_CGI/add_listing.php " + std::string(file_path) + " > " + filePath2;

    std::cout << "command is " << command << std::endl;
    int exit_status = std::system(command.c_str());
    std::cout << "exit status: " << exit_status << std::endl;
    
    std::ifstream picture_file("t_deleted/pic-entry.jpeg");

//         std::cout << "name: public/www/images/" + id_string + ".jpeg" << std::endl;

if (picture_file)
{
    std::ifstream tmpFile2(filePath2);
    std::string id_string((std::istreambuf_iterator<char>(tmpFile2)), std::istreambuf_iterator<char>());
    std::cout << "name: " << id_string << std::endl;
    std::cout << "root folder is " << _rootDirectory << std::endl;

    std::string defaultPath = _rootDirectory + "/images/" + id_string + ".jpeg";

    // Move the file once to default location
    moveAndRenameFile("t_deleted/pic-entry.jpeg", defaultPath);

    std::ifstream less_see_file(defaultPath.c_str());
    if(less_see_file)
    {
        ServerConfiguration* matchedServer = req.getMyServerBlock();
        LocationBlock* loc = locationExistsInBlock(matchedServer->getRoutes(), "uploads");
        

    if (loc && loc->getUploadPath() != "NotSet") {
        std::string uploadPath = loc->getUploadPath();

        std::string uploadDest = _rootDirectory + "/" + uploadPath + "/" + "Eval_upload" + id_string + ".jpeg";
        std::cout << "Copying file from '" << defaultPath << "' to '" << uploadDest << "'" << std::endl;

        // Copy the file from default location to upload path
        copyFile(defaultPath, uploadDest);
        std::cout << "Copy attempt finished" << std::endl;
    }

    }
}


    // delete the tmp file
    remove(file_path);
    remove("t_deleted/post-entry.txt");
    remove(filePath2);
    
}



void PostActionResponse::executePostDeleteResponse(ClientRequest& request) {
    const std::string deleteFilePath = "t_deleted/delete-entry.txt";

    {
        std::ofstream tmpFile(deleteFilePath.c_str());
        if (tmpFile) {
            tmpFile << request.getBodyContent();
        }
    }

    std::string command = "php basic_CGI/delete_listing.php " + deleteFilePath;
    std::system(command.c_str());

    std::string id = request.getBodyContent().substr(request.getBodyContent().find("&") + 4);
    std::string removePath = _rootDirectory + "/images/" + id + ".jpeg";

    std::cout << "[PostResponse] Deleting image: " << removePath << "\n";
    std::remove(removePath.c_str());
    std::remove(deleteFilePath.c_str());
    std::cout << "got here: haha" << std::endl;
}

void PostActionResponse::setLocationHeader() {
    _headers["Location"] = "/";
}

void PostActionResponse::setRefreshHeader(double sec) {
    std::ostringstream ss;
    ss << sec;
    _headers["Refresh"] = ss.str() + ";url=/";
}

void PostActionResponse::setHeaders() {
    addDateHeader();
    addConnectionHeader("close");
    setLocationHeader();
    setHostHeader(_host.c_str());
    addCacheControlHeader("no-cache");
}

void PostActionResponse::constructResponse(ClientRequest& request) {
    std::string path = (request.getUriObject()).getPath();

    initializeResourceFromRequest(request);
    _raw_status_line = Http_version_ + " 302 Found" + "\r\n";

    if (path.find("add_listing.php") != std::string::npos)
        executePostResponse(request);
    else
        executePostDeleteResponse(request);

    setHeaders();
    composeHeaderString();
    setServerReply(); // you stressed, will i ever forgive you this function?
}

void PostActionResponse::constructDefaultResponseWithBody(ClientRequest& request, const str& bodyContent) {
    (void)request;
    (void)bodyContent;
}

void PostActionResponse::constructConfigurationResponse(ClientRequest& request, const str& targetFilePath) {
    (void)request;
    (void)targetFilePath;
}

void PostActionResponse::printResponse() {
    std::cout << "PostResponse:\n"
              << _raw_status_line << _raw_headers << _raw_body << std::endl;
}

// void PostActionResponse::setStatusCode(StatusCode statusCode) {
//     setHttpResponseStatusCode(statusCode); // it's useless Daniel
// }
