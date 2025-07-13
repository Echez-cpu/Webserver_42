#pragma once

#include "Socket.hpp"
#include <iostream>
#include "Helper.hpp"
#include "ClientRequest.hpp"
#include "ReplyClient.hpp"
#include "PostActionResponse.hpp"
#include "DeleteActionResponse.hpp"
#include "GetActionResponse.hpp"
#include "FailActionResponse.hpp"
class UR_Locator;

//typdef std::string str;

class ClientSession : public Socket {
	
public:

		ClientSession();

		virtual ~ClientSession();

        void            handleClientRequest(char buf[2000]);
        str     buildHttpResponseString(void);
        void handleRequestParseException(ClientRequest& request, std::exception& e);

private:
        std::vector<std::pair<ClientRequest*, ReplyClient*> >	requestResponseArray;
       //  UR_Locator                                 _uri; // Patrick!!! haha
};
