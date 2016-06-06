
#include <SFML/Graphics.hpp>
#include <iostream>
#include "fonctions_grapiphiques.hpp"


int main()
{
    const int taille = 500;
    sf::RenderWindow window(sf::VideoMode(taille, taille), "SFML works!");


    std::cout << is_point<sf::Vector2f>::value << std::endl;
    std::cout << is_point<int>::value << std::endl;
    std::cout << is_point<lapin>::value << std::endl;

    std::vector<sf::Vector2f> points = { {0,0} , {150,10} , {120,90}  , {30,100} , {0,50}};
    sf::ConvexShape convex = createPolygone(points);


    std::vector<lapin> pointLapin = { {100,100} , {250,110} , {220,190}  , {130,200} , {100,150}};
    sf::ConvexShape convex2 = createPolygone(pointLapin);



    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(convex);
        window.draw(convex2);
        window.display();
    }

    return 0;
}
