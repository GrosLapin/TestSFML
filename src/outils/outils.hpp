#ifndef OUITLS_HPP
#define OUITLS_HPP


#include <iostream>
#include <algorithm>
#include <vector>
#include <typeinfo>
#include <utility>
#include <fstream>

#include "fonction_string.hpp" 

namespace testSFML {
    
    inline bool file_exists(std::string const & pathname)
    {
        std::ifstream f(pathname);
        return bool(f);
    }

    inline std::string get_name_file(const std::string chemin)
    {
        if ( chemin.find('/') != std::string::npos )
        {
            return decoupe(chemin,"/").back();
        }
        return chemin;
    }
}

#endif

