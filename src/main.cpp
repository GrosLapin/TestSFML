
#include <SFML/Graphics.hpp>
#include <iostream>
#include "fonctions_grapiphiques.hpp"

#include "fonctions_math.hpp"

using namespace testSFML;
int main()
{


     normal s; // (default mean = zero, and standard deviation = unity)


    const int taille = 500;
    sf::RenderWindow window(sf::VideoMode(taille, taille), "SFML works!");


    std::cout << "v2F " << is_point<sf::Vector2f>::value << std::endl;
    std::cout << "v2i " << is_point<sf::Vector2i>::value << std::endl;
    std::cout << is_point<int>::value << std::endl;
    std::cout << is_point<lapin>::value << std::endl;

    std::vector<sf::Vector2f> points = { {0,0} , {150,10} , {120,90}  , {30,100} , {0,50}};


	std::vector<sf::ConvexShape> poly;
	poly.emplace_back(createPolygone(points));
	poly.emplace_back(createPolygone({ {100,100} , {250,110} , {220,190}  , {130,200} , {100,150}}));

     points.clear();

    sf::CircleShape circle(10);
    circle.setPosition({100,100});


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

			if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left))
            {
				/*std::cout << " x : " << event.mouseButton.x << " y : " <<   event.mouseButton.y  << std::endl;
				std::cout << "abs :" << sf::Mouse::getPosition() << std::endl;
				std::cout << "relatif : " <<  sf::Mouse::getPosition(window) << std::endl;*/
				points.emplace_back(sf::Mouse::getPosition(window));

                double dist =  distance(circle.getPosition(),sf::Mouse::getPosition(window) ) ;
                std::cout << "distance : " <<dist << std::endl;
				std::cout << getGaussianValue(s,dist/100) << std::endl;
            }

            if ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return )
			{

				std::cout << "entrer" << std::endl;
				poly.emplace_back(createPolygone(points));
				points.clear();


			}
        }

        window.clear();
		/*for (const auto& convex : poly)
		{
			window.draw(convex);
		}*/

		window.draw(circle);

        window.display();
    }

    return 0;
}
