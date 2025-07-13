#ifndef POST_ACTION_RESPONSE_HPP
#define POST_ACTION_RESPONSE_HPP

class ClientSession;
#include "DeleteActionResponse.hpp"
#include "Helper.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include "ClientSession.hpp"

class PostActionResponse : public ReplyClient {
    private:
        std::string _rootDirectory;
        std::string _host;

        // Helper methods
        void setHeaders();
        void setRawBody();
        void setLocationHeader();
        void setRefreshHeader(double seconds);

    public:
        // Constructors & Destructor
        PostActionResponse();
        PostActionResponse(ServerConfiguration* config);
        virtual ~PostActionResponse();

        // Core response construction
        void constructResponse(ClientRequest& request);
        void constructDefaultResponseWithBody(ClientRequest& request, const str& bodyContent);
        void constructConfigurationResponse(ClientRequest& request, const str& targetFilePath);

        // Execution logic
        void executePostResponse(ClientRequest& request);
        void executePostDeleteResponse(ClientRequest& request); // optional hybrid logic

        // Utilities
        void printResponse();
        // void setStatusCode(StatusCode statusCode); // it's useless Daniel
};

#endif // CLASS_POST_RESPONSE_HPP