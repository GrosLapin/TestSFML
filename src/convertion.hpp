#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <sstream>
#include "traits.hpp"
#include "fonction_string.hpp"

namespace testSFML {

	
	
	
	// because magic !
	
	template <	class T	 >
	auto to_string(const T& t) -> std::enable_if_t<	std_to_stringable_t<T>::value ,
													std::string > 
	{
      return std::to_string(t);
    }
    
    template <	class T	 >
	std::enable_if_t<	! std_to_stringable_t<T>::value ,
						std::string
					> to_string(const T& t) {
       std::stringstream ss;
	   ss  <<  t;
	   return ss.str();
    }
	
	/*
	template <	class T	 , 
				class =   std::enable_if_t<	std_to_stringable_t<T>::value >,
				class = void
			>
	std::string to_string(const T& t)
	{
      return std::to_string(t);
    }
    
    template <	class T	 ,
				class =   std::enable_if_t<	! std_to_stringable_t<T>::value >
			 >
	std::string to_string(const T& t) {
       std::stringstream ss;
	   ss  <<  t;
	   return ss.str();
    }
    */
    
	/*
	template <	class T	 >
	std::string to_string(T const & t, std::true_type) { return std::to_string(t); }
	
	template <	class T	 >
	std::string to_string(T const & t, std::false_type)
	{
       std::stringstream ss;
	   ss  <<  t;
	   return ss.str();
	}
	
	template <	class T	 >
	std::string to_string(T const & t)
	{
		return to_string(t, std_to_stringable_t<T>());
	}
	
	to_string(5);
	*/
    
	
    template <class T>
    T convert_to(const std::string& str) {
       std::stringstream ss(str);
       T temp;
       ss >> temp;
       return temp;
    }

    template <>
    double convert_to<double> (const std::string& str) {
        return std::atof(str.c_str());
    }

    template <>
    int convert_to<int> (const std::string& str) {
        return std::atoi(str.c_str());
    }

    template <>
    std::string convert_to<std::string> (const std::string& str) {
        return str;
    }
        
        
        /***
	 *
	 *
	 * Fait la conversion d'un point à un autre. globalement ça sert a dire au compilateur qu'on sais ce qu'on fait
	 * Et qu'on veux pas de warnings
	 *
	 ***/
        template <  class PointCible,
                    class PointFrom,
                    class X = typename std::enable_if< is_point<PointCible>::value >::type,
                    class W = typename std::enable_if< is_point<PointFrom>::value >::type>
	PointCible convert_to (const PointFrom & from)
	{
			using type_cible_x = decltype ( getX ( std::declval<PointCible>() ));
			using type_cible_y = decltype ( getY ( std::declval<PointCible>() ));
			return { type_cible_x(getX(from)) , type_cible_y(getY(from)) };
	}
}


#endif