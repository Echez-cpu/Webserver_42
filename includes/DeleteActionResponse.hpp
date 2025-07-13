#ifndef DELETE_ACTION_RESPONSE_HPP
#define DELETE_ACTION_RESPONSE_HPP

#include "ReplyClient.hpp"
#include "ServerConfiguration.hpp"
#include <string>
#include "Helper.hpp"


class DeleteActionResponse : public ReplyClient
{    
    private:
        // Internal Helpers
        void setHeaders();
        void setRawBody();
        void setLocationHeader();

        // Internal State
        std::string _rootFolder;
        std::string _host;
    
    public:
        // Constructors / Destructor
        DeleteActionResponse();
        DeleteActionResponse(ServerConfiguration* serverBlock);
        virtual ~DeleteActionResponse();

        // Core Response Builders
        void constructResponse(ClientRequest& request);
        void constructDefaultResponseWithBody(ClientRequest& request, const str& bodyContent);
        void constructConfigurationResponse(ClientRequest& request, const str& targetFilePath);

        // File Handling Logic
        void executeDeleteResponse(ClientRequest& request);

        // Utility & Debugging
        // void setHttpResponseStatusCode(HttpResponseStatus statusCode); // no use 
        void printResponse() const; // no use

};

#endif // DELETE_RESPONSE_HPP