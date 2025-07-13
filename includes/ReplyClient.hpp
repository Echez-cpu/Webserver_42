#pragma once

#include "Helper.hpp"
# include "ClientRequest.hpp"
# include "UR_Locator.hpp"
# include <string>

class ReplyClient {
    
    public:

        ReplyClient();
		virtual ~ReplyClient();

        
        void            setStatusCode(StatusCode s);
        str     getServerReply(void);
        void    setServerReply(void);
        void    ShowReply(void);

        virtual void    constructResponse(ClientRequest& request) = 0;
        virtual void    constructDefaultResponseWithBody(ClientRequest& request, const str& bodyContent) = 0;
        virtual void    constructConfigurationResponse(ClientRequest& request, const str& targetFilePath) = 0;
        virtual void    setHeaders() = 0;
        
    
    
    protected:
       
        str                         _raw_status_line;
        str                         _raw_headers;
        str                         _raw_body;
        str                         _raw_response;

        
        str                         Http_version_;
        StatusCode                          _status_code;
        str                         _status_string;
        std::map<std::string, std::string>  _headers;
        str                         _resource;

        void    composeHeaderString(void);
        void    addDateHeader(void);
        void    addContentLengthToHeaders();
        void    addConnectionHeader(str connection_type);
        void    addContentTypeHeader();
        void    addCacheControlHeader(const char* type);
        void    addRetryAfterHeader(int seconds);
		void	setHostHeader(const char* server_name);

        void    initializeResourceFromRequest(ClientRequest& request);
        void addSetCookieHeader(const std::string& name, const std::string& value, const std::string& path = "/", int maxAge = 3600);

    
};
