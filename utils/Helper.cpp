/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pokpalae <pokpalae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:28:41 by pokpalae          #+#    #+#             */
/*   Updated: 2025/07/08 13:54:45 by pokpalae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Helper.hpp"

bool ft_is_avail(str resource) {
    std::ifstream file(resource.c_str());
    if (!file.is_open())
        return false;
    bool available = file.good();
    file.close();
    return available;
}


bool hasFileExtension(const str& s, const str& ext) {
    if (s.length() < ext.length())
        return false;

    std::size_t pos = s.rfind(ext);
    return pos != std::string::npos && pos == (s.length() - ext.length());
}


str remove_slashes(str string) {
    std::string::size_type first = string.find_first_not_of('/');
    if (first == std::string::npos)
        return ""; // string contains only slashes

    std::string::size_type last = string.find_last_not_of('/');
    return string.substr(first, last - first + 1);
}


std::vector<std::string> ft_split(const str &str, char delim)
{
    std::vector<std::string> result;        // Holds the split substrings.
    std::istringstream ss(str);    // Create a string stream from input.
    std::string item;                     // Temporarily holds each split part.

    while (std::getline(ss, item, delim)) 
    {  
        result.push_back(item); 
    }

    return  result;
}



bool is_folder_in_tree(str root_folder, str folder_name)
{
    std::vector<str> stack;
    stack.push_back(root_folder);

    while (!stack.empty())
    {
        str current_folder = stack.back();
        stack.pop_back();

        DIR* dir = opendir(current_folder.c_str());
        if (!dir)
            continue; // Cannot open this folder, skip it

        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL)
        {
            str entry_name(entry->d_name);

            // Skip current and parent directory entries
            if (entry_name == "." || entry_name == "..")
                continue;

            if (entry->d_type == DT_DIR)
            {
                if (entry_name == folder_name)
                {
                    closedir(dir);
                    return true; // Found the folder in current subdir
                }

                // Add this subdirectory to stack for further exploration
                str sub_path = current_folder + "/" + entry_name;
                stack.push_back(sub_path);
            }
        }
        closedir(dir);
    }

    return false; // Folder not found anywhere
}


bool isNotWhitespace(int ch) 
{
    return !std::isspace(ch);
}


str DefaultErrorPage(StatusCode status_code)
{
    typedef std::map<StatusCode, std::string> ErrorPageMap;
    static ErrorPageMap defaultErrorPages;

    if (defaultErrorPages.empty())
    {
        defaultErrorPages.insert(std::make_pair(BAD_REQUEST, "<html><body>BAD_REQUEST</body></html>\r\n"));
        defaultErrorPages.insert(std::make_pair(UNAUTHORIZED, "<html><body>UNAUTHORIZED</body></html>\r\n"));
        defaultErrorPages.insert(std::make_pair(FORBIDDEN, "<html><body>FORBIDDEN</body></html>\r\n"));
        defaultErrorPages.insert(std::make_pair(NOT_FOUND, "<html><body>NOT_FOUND</body></html>\r\n"));
        defaultErrorPages.insert(std::make_pair(LENGTH_REQUIRED, "<html><body>LENGTH_REQUIRED</body></html>\r\n"));
        defaultErrorPages.insert(std::make_pair(PAYLOAD_TOO_LARGE, "<html><body>Error 413: Payload Too Large</body></html>\r\n"));
        defaultErrorPages.insert(std::make_pair(UNSUPPORTED_MEDIA_TYPE, "<html><body>UNSUPPORTED_MEDIA_TYPE</body></html>\r\n"));
        defaultErrorPages.insert(std::make_pair(INTERNAL_SERVER_ERROR, "<html><body>INTERNAL_SERVER_ERROR</body></html>\r\n"));
        defaultErrorPages.insert(std::make_pair(HTTP_VERSION_NOT_SUPPORTED, "<html><body>HTTP_VERSION_NOT_SUPPORTED</body></html>\r\n"));
    }

    ErrorPageMap::const_iterator it = defaultErrorPages.find(status_code);
    if (it != defaultErrorPages.end())
        return it->second;

    // Fallback if status_code not found
    return "<html><body>Unknown Error</body></html>\r\n";
}


void setNonBlocking(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("Failed to get socket flags");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("Failed to set socket to non-blocking");
}



// LocationBlock* findBestMatchedLocation(std::vector<LocationBlock*>& locations, const std::string& uri) {
//     LocationBlock* bestMatch = NULL;
//     std::string bestPrefix = "";

//     for (std::vector<LocationBlock*>::iterator it = locations.begin(); it != locations.end(); ++it) {
//         const std::string& path = (*it)->getPathUri();
//         if (uri.compare(0, path.length(), path) == 0) {
//             if (path.length() > bestPrefix.length()) {
//                 bestPrefix = path;
//                 bestMatch = *it;
//             }
//         }
//     }
//     return bestMatch;
// }

LocationBlock* locationExistsInBlock(std::vector<LocationBlock*>& lbs, std::string url) 
{
   std::cout << "Searching location for URL: " << url << std::endl;
   for (size_t i = 0; i < lbs.size(); ++i)
   {
       std::cout << "Location block path: " << lbs[i]->getPathUri() << std::endl;
   }

    std::vector<LocationBlock*>::iterator it = lbs.begin();
    while (it != lbs.end()) 
    {
        if ((*it)->getPathUri() == url)
            return (*it);
        ++it;
    }
    return NULL;
}


bool isDirectory(const std::string& path) {
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) != 0)
        return false;
    return S_ISDIR(pathStat.st_mode);
}
