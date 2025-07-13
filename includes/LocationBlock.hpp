#ifndef LOCATION_BLOCK_HPP
#define LOCATION_BLOCK_HPP

#include <string>
#include <vector>

class LocationBlock {
private:
    std::string     _pathUri;               
    std::string     _documentRoot;          
    std::string     _defaultIndex;     
    unsigned long   _maxClientBodySize;   
    // my addiction (Patrick)
    bool _hasRedirect;
    int _redirectStatus;
    std::string _redirectTarget;
    bool _autoindex;
    std::string _cgiExtension;
    std::string _cgiPath;
    std::string _UploadPath;

    // Method restrictions
    bool            _allowGet;
    bool            _allowPost;
    bool            _allowDelete;

public:
    // Constructors & Destructor
    LocationBlock();
    LocationBlock(const LocationBlock& other);
    ~LocationBlock();

    // Assignment operator
    LocationBlock& operator=(const LocationBlock& other);

    // Accessors (Getters)
    std::string     getPathUri() const;
    std::string     getDocumentRoot() const;
    std::string     getDefaultIndex() const;
    unsigned long   getMaxClientBodySize() const;
    bool            isGetAllowed() const;
    bool            isPostAllowed() const;
    bool            isDeleteAllowed() const;

    // Mutators (Setters)
    void setPathUri(const std::string& uri);
    void setDocumentRoot(const std::string& rootPath);
    void setDefaultIndex(const std::string& indexFile);
    void setMaxClientBodySize(unsigned long maxSize);
    void configureAllowedMethods(const std::vector<std::string>& methods);

    // Patrick's addiction
    void setRedirect(int status, const std::string &target);
    bool hasRedirect() const;
    int getRedirectStatus() const;
    const std::string &getRedirectTarget() const;
    void setDirectoryListing(bool enabled);
    bool isDirectoryListingEnabled() const;
    void setCgiExtension(const std::string& ext);
    void setCgiPath(const std::string& path);
    std::string getCgiExtension() const;
    std::string getCgiPath() const;
    void setUploadPath(const std::string& path);
    std::string getUploadPath() const;

};

#endif 
   
