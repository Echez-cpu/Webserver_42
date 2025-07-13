#include "../includes/FailActionResponse.hpp"

// Constructor
FailActionResponse::FailActionResponse() {}

// Destructor
FailActionResponse::~FailActionResponse() {}

// Sets the "Location" header to redirect the client
void FailActionResponse::setLocationHeader() {
    _headers.insert(std::make_pair("Location", "/"));
}

// Set HTTP headers based on status and response context
void FailActionResponse::setHeaders() {
    addDateHeader();
    addContentTypeHeader();

    if (_raw_status_line.find("Found") != std::string::npos) {
        setLocationHeader();
    }

    // Additional headers may be added here
}

// Read content from the file indicated by _resource and set it as body
void FailActionResponse::setRawBody() {
    std::ifstream file(_resource.c_str());
    std::string content;

    if (file) {
        std::ostringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
    }

    _raw_body = content;
    addContentLengthToHeaders();
}

// No-op for now, could be used for additional logic later
void FailActionResponse::constructResponse(ClientRequest& req) {
    (void)req;
}

// Set response body and headers based on a raw body and request status
void FailActionResponse::constructDefaultResponseWithBody(ClientRequest& request, const str& bodyContent) {
    _status_code = HTTP_VERSION_NOT_SUPPORTED;

    switch (request.getStatusCode()) {
        case HTTP_VERSION_NOT_SUPPORTED:
            _raw_status_line = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
            break;
        case PAYLOAD_TOO_LARGE:
            _raw_status_line = "HTTP/1.1 413 Payload Too Large\r\n";
            break;
        case NOT_FOUND:
            _raw_status_line = "HTTP/1.1 404 Not Found\r\n";
            break;
        case INTERNAL_SERVER_ERROR:
            _raw_status_line = "HTTP/1.1 500 Internal Server Error\r\n";
            break;
        case UNAUTHORIZED:
            _raw_status_line = "HTTP/1.1 401 Unauthorized\r\n";
            break;
        case FOUND:
            _raw_status_line = "HTTP/1.1 302 Found\r\n";
            setLocationHeader();
            break;
        default:
            _raw_status_line = "HTTP/1.1 400 Bad Request\r\n";
            break;
    }

    setHeaders();
    composeHeaderString();
    _raw_body = bodyContent;
    setServerReply();

    std::cout << "Raw response is:\n" << _raw_response << std::endl; 
}

// Converts an enum status code to its string representation
static std::string statusToString(StatusCode status_code) {
    switch (status_code) {
        case BAD_REQUEST:
            return "400 Bad Request";
        case UNAUTHORIZED:
            return "401 Unauthorized";
        case NOT_FOUND:
            return "404 Not Found";
        case LENGTH_REQUIRED:
            return "411 Length Required";
        case PAYLOAD_TOO_LARGE:
            return "413 Payload Too Large";
        case INTERNAL_SERVER_ERROR:
            return "500 Internal Server Error";
        case HTTP_VERSION_NOT_SUPPORTED:
            return "505 HTTP Version Not Supported";
        default:
            return "400 Bad Request";
    }
}

// Constructs a response based on a static error HTML file
void FailActionResponse::constructConfigurationResponse(ClientRequest& request, const str& targetFilePath) {
    _resource = targetFilePath;
    _raw_status_line = Http_version_ + " " + statusToString(request.getStatusCode()) + "\r\n";

    setHeaders();
    composeHeaderString();
    setRawBody();
    setServerReply();

    std::cout << "Raw response is:\n" << _raw_response << std::endl;
}

