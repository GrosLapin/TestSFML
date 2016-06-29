#ifndef OUITLS_HPP
#define OUITLS_HPP

#include <iostream>
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
    
    
    template < 	bool is_map , 
				class T>
	struct get_value_s
	{
		auto& get_value (T& ite) { return *ite; }
		const auto& get_value (const T& ite) { return *ite; }
	};
	
	template <class T>
	struct get_value_s < true, T >
	{
		auto& get_value (T& ite) { return ite->second;; }
		const auto& get_value (const T& ite) { return ite->second;; }
	};
	
	
    template< bool is_map , class T>
	auto& get_value (T& ite)
	{
			get_value_s<is_map,T> gvs;
			return gvs.get_value(ite);
	}
	
	template< bool is_map , class T>
	const auto& get_value (const T& ite)
	{
			get_value_s<is_map,T> gvs;
			return gvs.get_value(ite);
	}
	
    
    /*
	template<	class T,
				class = std::enable_if_t < !is_map<T>::value >
			>
	auto& get_value (T& ite)
	{
			std::cout << " non const non map " << std::endl;
			return *ite;
	}
	
	template<	class T,
				class = std::enable_if_t < is_map<T>::value >,
				class U = void
			>
	auto& get_value (T& ite) 
	{
		std::cout << " non const map " << std::endl;
		return ite->second;
	}
	
	
	template<	class T,
				class = std::enable_if_t < !is_map<T>::value >
			>
	const auto& get_value (const T& ite)
	{
			std::cout << " const non map " << std::endl;
			return *ite;
	}
	template<	class T,
				class = std::enable_if_t < is_map<T>::value >,
				class U = void
			>
	const auto& get_value (const T& ite) 
	{
		std::cout << " const map " << std::endl;
		return ite->second;
	}
	*/
}
#endif

