#include "../includes/ConfigParser.hpp"

ConfigParser::ConfigParser()
    : _maxClientBodySize(0), _listeningServerPort("1239") {
}

ConfigParser::ConfigParser(const ConfigParser &other)
    : _maxClientBodySize(other._maxClientBodySize),
      _listeningServerPort(other._listeningServerPort),
      _defaultErrorPages(other._defaultErrorPages),
      _serverBlocksFromConfig(other._serverBlocksFromConfig) {
}

ConfigParser::~ConfigParser() {
    for (size_t i = 0; i < _serverBlocksFromConfig.size(); ++i) {
        delete _serverBlocksFromConfig[i];
    }
    _serverBlocksFromConfig.clear();
    std::cout << "Yours faithful: Configparser" << std::endl;
}

ConfigParser &ConfigParser::operator=(const ConfigParser &other) {
    if (this != &other) {
        _maxClientBodySize = other._maxClientBodySize;
        _listeningServerPort = other._listeningServerPort;
        _defaultErrorPages = other._defaultErrorPages;
        _serverBlocksFromConfig = other._serverBlocksFromConfig;
    }
    return *this;
}

// Checks if the number of '{' and '}' braces match in the given file
// bool checkBlocks(std::string fileName)
// UTILITY FUNCTION Refatcor later
bool checkBalanceBraces(const std::string &fileName) {
    std::ifstream file(fileName.c_str());
    if (!file.is_open()) {
        std::cout << "Failed to open config file: " << fileName << std::endl;
        return false;
    }

    int openBraces = 0;
    int closeBraces = 0;
    std::string line;

    while (std::getline(file, line)) {
        for (std::string::size_type i = 0; i < line.size(); ++i) {
            char char_value = line[i];
            if (char_value == '{') {
                ++openBraces;
            } else if (char_value == '}') {
                ++closeBraces;
            }
        }
    }

    return openBraces == closeBraces;
}
// Trims leading and trailing whitespace characters from a string
// std::string trim(std::string s)
// UTILITY FUNCTION Refactor later
std::string trimSpaces(const std::string &str) {
    std::string::size_type start = str.find_first_not_of(" \t\r\n");
    std::string::size_type end = str.find_last_not_of(" \t\r\n");

    if (start == std::string::npos) {
        return ""; // String is all whitespace
    }

    return str.substr(start, end - start + 1);
}



void validateServerBlockRow(ServerConfiguration *serverConfig, const std::vector<std::string> &directiveTokens)
{
    if (directiveTokens.empty())
        return;

    const std::string &directive = directiveTokens.at(0);

    if (directive == "listen") {
        if (directiveTokens.size() > 1)
            serverConfig->setPort(directiveTokens.at(1));
    } else if (directive == "client_max_body_size") {
        if (directiveTokens.size() > 1)
            serverConfig->setMaxClientBodySize(strtoul(directiveTokens.at(1).c_str(), NULL, 10));
    } else if (directive == "server_name") {
        if (directiveTokens.size() > 1)
            serverConfig->setHostname(directiveTokens.at(1));
    } else if (directive == "index") {
        if (directiveTokens.size() > 1)
            serverConfig->setDefaultIndex(directiveTokens.at(1));
    } else if (directive == "root") {
        if (directiveTokens.size() > 1)
            serverConfig->setDocumentRoot(directiveTokens.at(1));
    } else if (directive == "error_pages") {
        if (directiveTokens.size() > 2)
            serverConfig->setErrorPage(directiveTokens.at(1), directiveTokens.at(2));
    } else if (directive == "limit_except") {
        serverConfig->setAllowedMethods(directiveTokens);
    }
}


//void    parseLocationBlockLine(LocationBlock* currLocationBlock, std::vector<std::string> tokens)
void validateLocationBlockRow(LocationBlock *locationBlock, const std::vector<std::string> &directiveTokens) {
    if (directiveTokens.empty())
        return;

    const std::string& directive = directiveTokens.at(0);

    if (directive == "index") {
        if (directiveTokens.size() > 1)
            locationBlock->setDefaultIndex(directiveTokens.at(1));
    } else if (directive == "root") {
        if (directiveTokens.size() > 1)
            locationBlock->setDocumentRoot(directiveTokens.at(1));
    } else if (directive == "limit_except") {
        locationBlock->configureAllowedMethods(directiveTokens);
    } else if (directive == "client_max_body_size") {
        if (directiveTokens.size() > 1)
            locationBlock->setMaxClientBodySize(strtoul(directiveTokens.at(1).c_str(), NULL, 10));
    }
    // Patrick's addiction
    else if (directive == "return") 
    {
      if (directiveTokens.size() == 3) 
      {
        int status = atoi(directiveTokens[1].c_str());
        const std::string &target = directiveTokens[2];
        locationBlock->setRedirect(status, target);
      }
    }
      //Patrick's Addiction
    else if (directive == "autoindex") 
    {
      if (directiveTokens.size() > 1) 
        {
         const std::string &value = directiveTokens.at(1);
            if (value == "on")
            {
                locationBlock->setDirectoryListing(true);
            }
            else if (value == "off")
            {
                locationBlock->setDirectoryListing(false);
            }

            else
            {
                throw ConfigParser::Invalid_AutoIndex_Exception();
            }
              
        }
    }
    
    else if (directive == "cgi_extension") {
    if (directiveTokens.size() > 1)
        locationBlock->setCgiExtension(directiveTokens.at(1)); 
}
    else if (directive == "cgi_path") {
    if (directiveTokens.size() > 1)
        locationBlock->setCgiPath(directiveTokens.at(1)); 
}

    else if (directive == "upload_path") {
    if (directiveTokens.size() > 1)
        locationBlock->setUploadPath(directiveTokens.at(1));
}


}


// Add a locationblock to the serverblock

// void    Config::parseLocationBlock(std::ifstream& file, std::vector<std::string> tokens, ServerBlock* sb);
void ConfigParser::validateLocationBlock(std::ifstream &fileStream, const std::vector<std::string> &directiveTokens, ServerConfiguration*serverBlock) 
{
    // Create a new LocationBlock and add it to the ServerBlock
    LocationBlock *locationBlock = new LocationBlock;
    serverBlock->addRoute(locationBlock);
    locationBlock->setPathUri(directiveTokens.at(1));

    // Allowed directive keys inside a location block
    std::vector<std::string> allowedDirectives;
    allowedDirectives.push_back("index");
    allowedDirectives.push_back("root");
    allowedDirectives.push_back("limit_except");
    allowedDirectives.push_back("client_max_body_size");
    allowedDirectives.push_back("return");            //  Patrick added this
    allowedDirectives.push_back("autoindex");        //  from Patrick
    allowedDirectives.push_back("cgi_extension");   //   from Patrick
    allowedDirectives.push_back("cgi_path");       //.   from Patrick
    allowedDirectives.push_back("upload_path");




    std::string currentLine;
    while (std::getline(fileStream, currentLine))
    {
        currentLine = trimSpaces(currentLine);

        // Skip empty lines or comments
        if (currentLine.empty() || currentLine[0] == '#')
        {
            std::cout << "Skipping empty line or comment in location block." << std::endl;
            continue;
        }

        // End of location block
        if (currentLine[0] == '}')
        {
            return;
        }

        // Tokenize the current line by spaces
        std::stringstream lineStream(currentLine);
        std::vector<std::string> parsedLocTokens;
        std::string locToken;
        while (std::getline(lineStream, locToken, ' ')) 
        {
            locToken = trimSpaces(locToken);
            if (!locToken.empty()) 
            {
                parsedLocTokens.push_back(locToken);
            }
        }

        if (parsedLocTokens.empty())
        {
            std::cout << "Empty line found in location block, skipping." << std::endl;
            continue;
        }

        // Validate directive
        if (std::find(allowedDirectives.begin(), allowedDirectives.end(), parsedLocTokens[0]) == allowedDirectives.end())
        {
            std::cout << "Unrecognized location directive: " << parsedLocTokens[0] << std::endl;
            throw DirectiveDoesNotExistException();
        }

        // Process the directive
        validateLocationBlockRow(locationBlock, parsedLocTokens);

        // If closing brace found at the end of the line, block is done
        if (currentLine.find("}") != std::string::npos)
        {
            return;
        }
    }
}

//bool Config::checkDirectives(std::string fileName)
bool ConfigParser::validateDirectives(const std::string &configFileName) {
    std::ifstream configFile(configFileName.c_str());
    if (!configFile.is_open()) {
        return false;
    }

    std::vector<std::string> validDirectiveKeys;
    validDirectiveKeys.push_back("http");
    validDirectiveKeys.push_back("server");
    validDirectiveKeys.push_back("listen");
    validDirectiveKeys.push_back("server_name");
    validDirectiveKeys.push_back("index");
    validDirectiveKeys.push_back("root");
    validDirectiveKeys.push_back("location");
    validDirectiveKeys.push_back("limit_except");
    validDirectiveKeys.push_back("client_max_body_size");
    validDirectiveKeys.push_back("error_pages");

    std::string currentLine;
    while (std::getline(configFile, currentLine)) {
        currentLine = trimSpaces(currentLine); // Remove leading/trailing whitespace

        // Skip empty lines and those starting with comments or brackets
        if (currentLine.empty()) {
            std::cout << "EMPTY" << std::endl;
        }
        if (currentLine.empty() ||
            currentLine[0] == '#' ||
            currentLine[0] == '{' ||
            currentLine[0] == '}') {
            continue;
        }

        // Tokenize the line
        std::stringstream lineStream(currentLine);
        std::vector<std::string> directiveTokens;
        std::string word;
        while (std::getline(lineStream, word, ' ')) {
            word = trimSpaces(word);
            if (!word.empty()) {
                directiveTokens.push_back(word);
            }
        }

        // Validate the first token (directive)
        if (directiveTokens.empty() ||
            std::find(validDirectiveKeys.begin(), validDirectiveKeys.end(), directiveTokens[0]) == validDirectiveKeys.end()) {
            std::cout << "Unrecognized token: " << directiveTokens[0] << std::endl;
            return false;
        }

        // Server block creation
        if (directiveTokens[0] == "server") {
            ServerConfiguration* newServerBlock = new ServerConfiguration();
            _serverBlocksFromConfig.push_back(newServerBlock);
        }

        // Handle inner directives
        if (directiveTokens[0] != "http" && directiveTokens[0] != "server") {
            if (directiveTokens[0] == "location") {
                validateLocationBlock(configFile, directiveTokens, _serverBlocksFromConfig.back());
            } else {
                validateServerBlockRow(_serverBlocksFromConfig.back(), directiveTokens);
            }
        }

        // Optional: Check for extra tokens
        // if (directiveTokens.size() > 1) {
        //     std::cout << "Extra tokens on line: " << currentLine << std::endl;
        //     return false;
        // }
    }

    return true;
}

bool ConfigParser::validateConfig(const std::string &configFilePath) {
    // Block checks
    if (!checkBalanceBraces(configFilePath))
        throw MissingBracketsException();

    // Check directive names;
    if (!validateDirectives(configFilePath))
        throw InvalidDirectiveException();
    checkForDuplicateServerBlocks();
    return true;
}


void    ConfigParser::checkForDuplicateServerBlocks(void){
    std::set<std::string> uniquePorts;
    std::set<std::string> uniqueServerNames;

    for (std::vector<ServerConfiguration *>::iterator it = _serverBlocksFromConfig.begin(); it != _serverBlocksFromConfig.end(); ++it) {
        // check Ports
        if (uniquePorts.find((*it)->getPort()) != uniquePorts.end())
            throw DuplicateServerBlockException();
        uniquePorts.insert((*it)->getPort());
        // check Server Names
        if (uniqueServerNames.find((*it)->getHostname()) != uniqueServerNames.end())
            throw DuplicateServerBlockException();
        uniqueServerNames.insert((*it)->getHostname());
    }
}


// Getters
unsigned long ConfigParser::getClientMaximumBodySize() const {
    return _maxClientBodySize;
}

const std::string& ConfigParser::getListeningServerPort() const {
    return _listeningServerPort;
}

const std::vector<ServerConfiguration *> &ConfigParser::getServerBlocks() const {
    return _serverBlocksFromConfig;
}


void ConfigParser::displayConfiguration() 

{
    std::cout << COLOR_MAGENTA << "** IS CONFIG CORRECT? **" << COLOR_RESET << std::endl;

    for (std::vector<ServerConfiguration*>::iterator it = _serverBlocksFromConfig.begin(); it != _serverBlocksFromConfig.end(); ++it) 
    {
        std::cout << COLOR_CYAN << "Server name:                    " << COLOR_RESET << (*it)->getHostname() << std::endl;
        std::cout << COLOR_CYAN << "Client Maximum Body Size:       " << COLOR_RESET << (*it)->getMaxClientBodySize() << std::endl;
        std::cout << COLOR_CYAN << "Listening Server Port:          " << COLOR_RESET << (*it)->getPort() << std::endl;
        std::cout << COLOR_CYAN << "Default Pages:                  " << COLOR_RESET << (*it)->getDefaultIndex() << std::endl;

        (*it)->printRoutes();
        std::cout << std::endl;
    }
}


const char *ConfigParser::FileNotFoundException::what() const throw() {
    return "Configuration file not found.";
}

const char *ConfigParser::InvalidDirectiveException::what() const throw() {
    return "Invalid directive found in configuration file.";
}

const char *ConfigParser::MissingBracketsException::what() const throw() {
    return "Missing opening or closing brackets in configuration file.";
}

const char *ConfigParser::DuplicateServerBlockException::what() const throw() {
    return "Duplicate server block found in configuration file.";
}

const char *ConfigParser::DirectiveDoesNotExistException::what() const throw() {
    return "Directive does not exist in configuration file.";
}

const char *ConfigParser::Invalid_AutoIndex_Exception::what() const throw() {
     return "autoindex_directive is missing";
}

