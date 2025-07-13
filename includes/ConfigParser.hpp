#pragma once


#include "ServerConfiguration.hpp"
#include "Helper.hpp"
#include <set>


class ConfigParser
{
        
    private:
        
        unsigned long _maxClientBodySize;
        
        std::string _listeningServerPort;

        std::map<int, std::string> _defaultErrorPages;


    public:
    
        std::vector<ServerConfiguration*> _serverBlocksFromConfig;
        ConfigParser();
        ConfigParser(const ConfigParser &other);
        ~ConfigParser();

        
        ConfigParser &operator=(const ConfigParser &other);

        
        bool validateConfig(const std::string &configFilePath);
        bool validateDirectives(const std::string &fileName);
        void validateLocationBlock(std::ifstream &fileStream, const std::vector<std::string> &tokens, ServerConfiguration *serverBlock);
        void checkForDuplicateServerBlocks();
        void displayConfiguration();

        
        const std::vector<ServerConfiguration *> &getServerBlocks() const;
        unsigned long getClientMaximumBodySize() const;


        
        const std::string &getListeningServerPort() const;

        
        class FileNotFoundException : public std::exception {
        public:
            const char *what() const throw();
        };

        class InvalidDirectiveException : public std::exception {
        public:
            const char *what() const throw();
        };

        class MissingBracketsException : public std::exception {
        public:
            const char *what() const throw();
        };

        class DuplicateServerBlockException : public std::exception {
        public:
            const char *what() const throw();
        };

        class DirectiveDoesNotExistException : public std::exception {
        public:
            const char *what() const throw();
        };

        class Invalid_AutoIndex_Exception : public std::exception {  
            public:
               const char *what() const throw();
        };


};