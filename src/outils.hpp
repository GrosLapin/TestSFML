#ifndef OUITLS_HPP
#define OUITLS_HPP


#include <iostream>
#include <algorithm>
#include <vector>
#include <typeinfo>
#include <utility>
#include <fstream>

#include "fonction_string.hpp"
#include "traits.hpp"

namespace testSFML {


	template<
			class T,
			class V = typename std::enable_if< is_container<T>::value >::type,
			class U = typename std::enable_if< !is_string<T>::value >::type
		>
		std::ostream& operator<< (std::ostream& out, const T& container)
		{
			for (const auto& val : container)
			{
			out << val << " " ;
			}
			return out;
		}




		template<   class Point,
					class V = typename std::enable_if< is_point<Point>::value >::type
				>
		inline std::ostream& operator<< (std::ostream& out, const Point& p)
		{

			out << "(" << getX(p)  << "," << getY(p) << ")" ;

			return out;
		}


    template <	class T, class U,
				class = enable_if_all_t <is_container, T, U >,
				class = std::enable_if_t<!is_string<U>::value>,
				class = void				
			>
    bool contain (const T& conteneur1,  const U& conteneur2)
    {
        if ( conteneur1.size() < conteneur2.size() ) { return false;}
        bool ok = true;
        for ( auto it = conteneur2.begin() ; it != conteneur2.end() && ok ; it++ )
        {
            ok = (std::find(conteneur1.begin(),conteneur1.end(),(*it)) != conteneur1.end());
        }
        return ok;
    }
    
    template < class T,
			   class U, 
			   class = std::enable_if_t <is_container<T>::value>
			 >
	bool contain (const T& conteneur1 , const U& value)
	{
		static_assert(!is_map<T>::value, "je n'ai pas encore fait la version pour les map");
		return std::find(conteneur1.begin(), conteneur1.end(), value) != conteneur1.end();
	}
    
    
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

