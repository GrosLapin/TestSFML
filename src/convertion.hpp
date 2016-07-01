#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <sstream>
#include "traits.hpp"


namespace testSFML {

		template<
			class T,
			class V = typename std::enable_if< is_container<T>::value >::type,
			class U = typename std::enable_if< !is_string<T>::value >::type
		>
		inline std::stringstream& operator<< (std::stringstream& out, const T& container)
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
		inline std::stringstream& operator<< (std::stringstream& out, const Point& p)
		{

			out << "(" << getX(p)  << "," << getY(p) << ")" ;

			return out;
		}
		
	/// TODO revoir pour pourvoir faire du X v Y si possible
	// Question pourquoi je peux pas juste faire 
	/*
	 * 
	 * std::stringstream ss();
	   std::ostream o;
	   ss << o <<  t;
	   return ss.str();
	 * 
	 * 
	 */
	template <class T>
	std::string to_string_martin(const T& t) {
       std::stringstream ss();
	   ss  <<  t;
	   return ss.str();
    }
	
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