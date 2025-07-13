#pragma once

#include <iostream>
class UR_Locator;
typedef std::string str;
#include "LocationBlock.hpp"

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_MAGENTA "\033[35m"

#include <sys/stat.h>
# include <poll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/wait.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <vector>
# include <map>
# include <exception>
# include <stdexcept>
# include <cstddef>
# include <unistd.h>
# include <stdlib.h>
# include <iostream>
# include <stdio.h>
# include <fstream>
# include <fcntl.h> // don't forget non_blocking attributes
# include <dirent.h>
# include <string>
# include <cstring>
# include <string.h>
# include <sstream>
# include <algorithm>
# include <ctime>
# include <time.h>

typedef enum Http_Method {
    GET = 0,
    POST,
    DELETE,
    NOT_SET
} Http_Method;


std::vector<std::string> ft_split(const str &str, char delim);
str remove_slashes(str string);
bool hasFileExtension(const str& s, const str& ext);
bool ft_is_avail(str resource);

bool is_folder_in_tree(str root_folder, str folder_name);
bool isNotWhitespace(int ch);
void setNonBlocking(int fd);


typedef enum StatusCode {
    
    UNDEFINED = 0,
    OK = 200,
    CREATED = 201,
    NO_CONTENT = 204,

    FOUND = 302,

    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    LENGTH_REQUIRED = 411,
    PAYLOAD_TOO_LARGE = 413,
    UNSUPPORTED_MEDIA_TYPE = 415,

    INTERNAL_SERVER_ERROR = 500,
    HTTP_VERSION_NOT_SUPPORTED = 505
} StatusCode;

str DefaultErrorPage(StatusCode status_code);

// LocationBlock* findBestMatchedLocation(std::vector<LocationBlock*>& locations, const std::string& uri);
LocationBlock* locationExistsInBlock(std::vector<LocationBlock*>& lbs, std::string url);
bool isDirectory(const std::string& path);
