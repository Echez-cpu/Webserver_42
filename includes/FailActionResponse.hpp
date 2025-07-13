#ifndef FAIL_ACTION_RESPONSE_HPP
# define FAIL_ACTION_RESPONSE_HPP

# include "DeleteActionResponse.hpp"
# include  "Helper.hpp"
# include <string>
# include <map>
# include <iostream>
# include <fstream>
# include <sstream>

class FailActionResponse : public ReplyClient 
{
    private:

    public:
        // Constructorss
        FailActionResponse();
		virtual ~FailActionResponse();

        // Public methods
        void        constructResponse(ClientRequest& request);
        void        constructDefaultResponseWithBody(ClientRequest& request, const str& bodyContent);
        void        constructConfigurationResponse(ClientRequest& request, const str& targetFilePath);
        void        setHeaders();
        void        setRawBody();
        void        setLocationHeader();
        void        printResponse(void); // useless
};

#endif