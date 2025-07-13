/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pokpalae <pokpalae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:28:41 by pokpalae          #+#    #+#             */
/*   Updated: 2025/06/24 20:11:50 by pokpalae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/DeleteActionResponse.hpp"
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <cstdio>

// Constructors
// Expected when implemented expected

DeleteActionResponse::DeleteActionResponse() {}

DeleteActionResponse::DeleteActionResponse(ServerConfiguration* serverBlock) {
    _rootFolder = serverBlock->getDocumentRoot();
    _host = serverBlock->getHostname();
}

DeleteActionResponse::~DeleteActionResponse() {}


// Helper Function

static std::string extractImageIdFromPath(const std::string& resourcePath) {
    std::string::size_type lastSlash = resourcePath.rfind('/');
    std::string::size_type extensionPos = resourcePath.find(".jpeg", lastSlash);

    if (lastSlash != std::string::npos && extensionPos != std::string::npos) {
        return resourcePath.substr(lastSlash + 1, extensionPos - lastSlash - 1);
    }
    return "";
}


// Core DELETE logic

void DeleteActionResponse::executeDeleteResponse(ClientRequest& request) {
    
    (void)request;
    const std::string tempFilePath = "t_deleted/delete-entry.txt";
    const std::string resourceId = extractImageIdFromPath(_resource);

    // Write DELETE parameters to temporary file
    std::ofstream tempFile(tempFilePath.c_str(), std::ios::out);
    if (!tempFile.is_open()) {
        std::cerr << "Failed to open temp file for DELETE operation." << std::endl;
        return;
    }

    tempFile << "_method=DELETE&id=" + resourceId;
    tempFile.close();

    // Execute the CGI PHP script
    std::string command = "php basic_CGI/delete_listing.php " + tempFilePath;
    std::system(command.c_str());

    // Attempt to delete the actual resource file
    const std::string imagePath = _rootFolder + "/images/" + resourceId + ".jpeg";
    if (std::remove(imagePath.c_str()) != 0) {
        std::cerr << "Warning: failed to remove image file at path: " << imagePath << std::endl;
    }

    // Clean up temporary request body file
    if (std::remove(tempFilePath.c_str()) != 0) {
        std::cerr << "Warning: failed to remove temp file at path: " << tempFilePath << std::endl;
    }
}


// Header Setup

void DeleteActionResponse::setLocationHeader() {
    _headers.insert(std::make_pair("Location", "/"));
}

void DeleteActionResponse::setHeaders() {
    addDateHeader();
    addConnectionHeader("close");
    setLocationHeader();
    addCacheControlHeader("no-cache");
    setHostHeader(_host.c_str());

    // Extend with more headers as needed
}


// Full Response Construction

void DeleteActionResponse::constructResponse(ClientRequest& request) {
    initializeResourceFromRequest(request);

    // Set status line
    _raw_status_line = Http_version_ + " 204 No Content\r\n";

    // Handle the DELETE operation
    executeDeleteResponse(request);

    // Configure headers and body
    setHeaders();
    composeHeaderString();

    // Finalize full response
    setServerReply();

    std::cout << "Generated DELETE response:\n"
              << _raw_status_line << _raw_headers << _raw_body << std::endl;
}

// Stub Methods (no-op / placeholder)

void DeleteActionResponse::constructDefaultResponseWithBody(ClientRequest& request, const str& bodyContent) {
    (void)request;
    (void)bodyContent;
}

void DeleteActionResponse::constructConfigurationResponse(ClientRequest& request, const str& targetFilePath) {
    (void)request;
    (void)targetFilePath;
}
