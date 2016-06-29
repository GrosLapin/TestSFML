
#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>
#include "fonctions_grapiphiques.hpp"

#include "fonctions_math.hpp"
#include "gestion_option.hpp"

#include "route.hpp"
#include "rond_point.hpp"

using namespace testSFML;
using namespace std;
int main(int argc, char** argv)
{
	
    gestion_option param(argc, argv);
    param.add("--ecart-type-centre","1");
    param.add("--ecart-type-maison","1");
    param.add("--nb-centre",3);
    param.add("--nb-maison",1000);
    
    param.allow_raw_args(false);
    param.load_file("./exe.param");

    double ecart_type_centre = param.get_val<double>("--ecart-type-centre");
    double ecart_type_maison = param.get_val<double>("--ecart-type-maison");
    size_t nb_maison = param.get_val<size_t>("--nb-maison");
    size_t nb_centre = param.get_val<size_t>("--nb-centre");

    normal normal_centre(0,ecart_type_centre); // (default mean = zero, and standard deviation = unity)
    normal normal_maison(0,ecart_type_maison);
    
    const int taille = 500;
    sf::RenderWindow window(sf::VideoMode(taille, taille), "SFML works!");
	sf::View view1(window.getDefaultView());
	window.setView(view1);

    
    std::vector<centre_influence<sf::Vector2f> > centres;
    std::vector <sf::CircleShape> cercles;
    std::vector<sf::VertexArray> vec_lignes;
	
	// test 
		std::vector<sf::Vector2f> test_vec = { {10,10}, {50, 60} };
		vec_lignes.push_back(create_lines({test_vec[1],test_vec[0]}));
	// fin test
    std::vector<route<sf::Vector2f>> routes;
    std::vector<sf::ConvexShape> poly;
        
    for (size_t i = 0 ; i < nb_centre ; i++)
    {
        // par defaut le coef est de 100
        centres.push_back({ random_point(centres,{0,0},{taille,taille}) ,normal_centre});
        cercles.emplace_back(5);
        cercles.back().setPosition(centres.back().centre);
    }
    
    auto create_maison = [&] ()
    {
        cercles.emplace_back(2);
        cercles.back().setPosition(random_point(centres,{0,0},{taille,taille}));
        centres.push_back({cercles.back().getPosition(),normal_maison,1});
    };
    
    for (size_t i= 0 ; i < nb_maison ; i ++ )
    {
        create_maison();
    }
    
    
    create_routes(std::vector<centre_influence<sf::Vector2f> >( centres.begin(),
																centres.begin()+ (long int)nb_centre) ,
			      std::vector<centre_influence<sf::Vector2f> >( centres.begin()+ (long int)nb_centre,
																centres.end())		
				 );
    
	map<int,sf::Vector2f> map_test;
    for (int i = 0 ; i < nb_centre ; i++)
	{
		map_test[i] = centres[i].centre;
	}
		map<int,sf::Vector2f> map_test2;
    for (int i = nb_centre ; i < centres.size() ; i++)
	{
		map_test2[i] = centres[i].centre;
	}
	 create_routes(map_test,map_test2 );
	 cout << is_map< map<int,sf::Vector2f> >::value << endl;
	 constexpr bool is_map_v = is_map< map<int,sf::Vector2f> >::value ;
	 cout << get_value<is_map_v>(map_test.begin());
	 exit(5);
	/* cout << typeid(sf::Vector2f).name() << endl;
	 cout << typeid(decltype (get_value(map_test.begin()))).name() << endl;

    cout << is_point<decltype (get_value(map_test.begin()) )>::value << endl;
	cout.flush();
	cout << ":" <<endl;
    */
    
    
    // creation des routes pour verifier que ça marche bien
    routes.emplace_back(sf::Vector2f(-5,-5));
    routes.back().add({-10 , -20});
    routes.back().add({-50 , -20});
    vec_lignes.push_back(create_lines(routes.back()));
    
    // pour les test
	std::vector <sf::Vector2f> points;    
    
    
    // affichage d'une la zone de jeu
    poly.emplace_back(create_polygone({ {0,0} , {0,taille} , {taille,taille}  , {taille,0}}));
	poly.back().setFillColor(sf::Color::Transparent);
	poly.back().setOutlineThickness(2);
	poly.back().setOutlineColor(sf::Color(250, 150, 100));
    
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) || sf::Keyboard::isKeyPressed(sf::Keyboard::M ) )     view1.zoom(1.005f); 

        
        
        window.setView(view1);
        
        
        sf::Event event;
        while (window.pollEvent(event))
        {
			


            if (event.type == sf::Event::Closed)
                window.close();

			
			
			if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left))
            {

				// = convert_to<sf::Vector2f> ( );
				auto mousePos  = window.mapPixelToCoords( sf::Mouse::getPosition(window));
				/*points.emplace_back(mousePos);
				
				if ( points.size() == 3 )
				{
					vec_lignes.push_back(create_lines(points));
					cout << angle_deg (points[0],points[1],points[2]) << endl;
				}
				if ( points.size() > 3 )
				{
					vec_lignes.erase(vec_lignes.end()-1);
					points.clear();	
				}*/
				std:: cout <<mousePos << std::endl;
				std::cout << distance(test_vec[0],test_vec[1],mousePos) << std::endl<< std::endl<< std::endl;


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
        
        
	
        // pas fou mais bon osef des perf now
        /*
        for ( const auto& rond : rond_points )
		{
			for (const auto route : rond.getRoutes() )
			{
				window.draw(create_lines(route));
			}
		}*/

        window.display();
		temps =  (float)timer.getElapsedTime().asSeconds ();
		timer.restart();
    }

    return 0;
}
