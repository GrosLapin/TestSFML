
#include <SFML/Graphics.hpp>
#include <iostream>
#include "fonctions_grapiphiques.hpp"

#include "fonctions_math.hpp"
#include "gestion_option.hpp"
#include "route.hpp"

using namespace testSFML;
using namespace std;
int main(int argc, char** argv)
{

    gestion_option param(argc, argv);
    param.add("--ecart-type-centre","1");
    param.add("--ecart-type-maison","1");
    param.add("--nb-maison",1000);
    
    param.allow_raw_args(false);
    param.load_file("./exe.param");

    double ecart_type_centre = param.get_val<double>("--ecart-type-centre");
    double ecart_type_maison = param.get_val<double>("--ecart-type-maison");
    int nb_maison = param.get_val<int>("--nb-maison");

    // creation des routes pour verifier que ça marche bien
    vector<sf::VertexArray> vec_lignes;
    vector<route<sf::Vector2f>> routes;
    routes.emplace_back(sf::Vector2f(-5,-5));
    routes.back().add({-10 , -20});
    routes.back().add({-50 , -20});
    
    vec_lignes.push_back(create_lines(routes.back()));
    
    
    normal normal_centre(0,ecart_type_centre); // (default mean = zero, and standard deviation = unity)
    normal normal_maison(0,ecart_type_maison);

    const int taille = 500;
    sf::RenderWindow window(sf::VideoMode(taille, taille), "SFML works!");
	sf::View view1(window.getDefaultView());
	window.setView(view1);

    std::cout << "v2F " << is_point<sf::Vector2f>::value << std::endl;
    std::cout << "v2i " << is_point<sf::Vector2i>::value << std::endl;
    std::cout << is_point<int>::value << std::endl;
    std::cout << is_point<lapin>::value << std::endl;

   

	std::vector<sf::ConvexShape> poly;
	poly.emplace_back(create_polygone({ {0,0} , {0,taille} , {taille,taille}  , {taille,0}}));
	poly.back().setFillColor(sf::Color::Transparent);
	poly.back().setOutlineThickness(2);
	poly.back().setOutlineColor(sf::Color(250, 150, 100));
	std::vector <sf::Vector2f> points;


    std::vector <sf::CircleShape> cercles;


    std::vector<centre_influence<sf::Vector2f> > centres;
    centre_influence<sf::Vector2f> test;
    test.centre = sf::Vector2f(100,100);
    test.repartition = normal_centre;
    centres.push_back(test);


    centres.push_back({sf::Vector2f(10,10),normal_centre});
    centres.push_back({sf::Vector2f(200,200),normal_centre});

    for (const auto& centre : centres )
    {
        cercles.emplace_back(5);
        cercles.back().setPosition(centre.centre);
    }

    auto create_maison = [&] ()
    {
        cercles.emplace_back(2);
        cercles.back().setPosition(random_point(centres,{0,0},{taille,taille}));
        centres.push_back({cercles.back().getPosition(),normal_maison,1});
    };
    for (int i= 0 ; i < nb_maison ; i ++ )
    {
        create_maison();
    }

    
    
    
    
    
    sf::Clock timer;

	float temps = 0 ;
    while (window.isOpen())
    {
			
		
        
        // gestion des touches pour la navigation
        float px_par_sec = 100;

        // le deplacement de la map
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)   )  {  view1.move( 0,      -temps*px_par_sec) ; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) )  {  view1.move( 0,       temps*px_par_sec) ;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) )  {  view1.move(-temps*px_par_sec,  0);}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  {  view1.move(temps*px_par_sec,   0);}

        // le zoom ( + et - et p et m pour les clavier sans pavé numérique )
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) || sf::Keyboard::isKeyPressed(sf::Keyboard::P ) )         view1.zoom(0.995f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) || sf::Keyboard::isKeyPressed(sf::Keyboard::P ) )     view1.zoom(1.005f); 

        
        
        window.setView(view1);
        
        
        sf::Event event;
        while (window.pollEvent(event))
        {
			


            if (event.type == sf::Event::Closed)
                window.close();

			
			
			if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left))
            {

				auto mousePos = convert_to<sf::Vector2f> ( sf::Mouse::getPosition(window) );
				points.emplace_back(mousePos);
				
				if ( points.size() == 3 )
				{
					vec_lignes.push_back(create_lines(points));
					cout << angle_deg (points[0],points[1],points[2]) << endl;
				}
				if ( points.size() > 3 )
				{
					vec_lignes.erase(vec_lignes.end()-1);
					points.clear();	
				}


            }

            if ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return )
			{
				vec_lignes.erase(vec_lignes.end()-1);
				points.clear();
			}
        }

        window.clear();
        
        for (const auto& lines : vec_lignes )
        {
            window.draw(lines);
        }
        for (const auto& convex : poly)
        {
            window.draw(convex);
        }

        for (const auto& circle : cercles)
        {
            window.draw(circle);
        }

        window.display();
		temps =  (float)timer.getElapsedTime().asSeconds ();
		timer.restart();
    }

    return 0;
}
