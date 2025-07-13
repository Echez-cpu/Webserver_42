#ifndef SERVER_CONFIGURATION_HPP
#define SERVER_CONFIGURATION_HPP

# include "LocationBlock.hpp"
# include <string>
#include "Helper.hpp"
#include <string>
#include <vector>
#include <map>
#include "LocationBlock.hpp"


class ServerConfiguration {
	private:
		unsigned long   _maxClientBodySize;
		std::string     _port;
		std::string     _documentRoot;
		std::string     _defaultIndex;
		std::string     _hostname;

		std::map<StatusCode, std::string> _errorPagePaths;
		std::vector<LocationBlock*> _routes; // Ensure memory is handled externally or with smart pointers (if permitted)

	
	public:
		ServerConfiguration();
		ServerConfiguration(const ServerConfiguration& source);
		~ServerConfiguration();

		ServerConfiguration& operator=(const ServerConfiguration& source);

        // HTTP method permissions
		bool _allowGET;
		bool _allowPOST;
		bool _allowDELETE;

		// Utility Methods
		void printRoutes() const;
		std::string fetchErrorPagePath(StatusCode statusCode);

		// Getters
		unsigned long getMaxClientBodySize() const;
		std::string getPort() const;
		std::string getDocumentRoot() const;
		std::string getDefaultIndex() const;
		std::string getHostname() const;
		std::vector<LocationBlock*>& getRoutes(); // why const Daniel?

		// Setters
		void setMaxClientBodySize(unsigned long size);
		void setPort(const std::string& port);
		void setHostname(const std::string& name);
		void setDefaultIndex(const std::string& indexFile);
		void setDocumentRoot(const std::string& rootPath);
		void setAllowedMethods(const std::vector<std::string>& methods);
		void setErrorPage(const std::string& code, const std::string& path);
		void addRoute(LocationBlock* route);
};

#endif // SERVER_CONFIGURATION_HPP