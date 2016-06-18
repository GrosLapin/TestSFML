#ifndef ROUTE_HPP
#define ROUTE_HPP


#include <vector> 

#include "traits.hpp"


namespace testSFML
{
    template <class Point,
             class X = typename std::enable_if< is_point<Point>::value >::type>
    class route 
    {
        std::vector<Point> vec_point;
        
    public: 
        // on match notre définition d'un conteneur
        using iterator          = typename std::vector<Point>::iterator;
        using const_iterator    = typename std::vector<Point>::const_iterator;
        iterator        begin()         { return vec_point.begin() ; }
        const_iterator  begin() const   { return vec_point.begin() ; }
        iterator        end()           { return vec_point.end() ; }
        const_iterator  end()   const   { return vec_point.end() ; }
        size_t          size()  const   { return vec_point.size() ;}
        
        
        route (const Point & depart)
        {
            add(depart);
        }
        
        void add(const Point & depart)
        {
            vec_point.push_back(depart);
        }
        
    };
}
#endif