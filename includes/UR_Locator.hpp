
#pragma once
# include <iostream>

typedef std::string str;

class UR_Locator {
    private:
        
        str     _path;
        str     _query;

    public:
        UR_Locator();
		virtual ~UR_Locator();
  
        str&    getPath(void);
        str&    getQuery(void);
        
        
        void    setPath(str path); 
        void    setQuery(str query);  
       
};