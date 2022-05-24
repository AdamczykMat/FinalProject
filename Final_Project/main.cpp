#include <iostream>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Button : public sf::Sprite
 {
  public:

    bool isClicked(sf::Vector2i &mouse_position) const
    {
        sf::FloatRect button_bounds = getGlobalBounds();

        if(mouse_position.x >= button_bounds.left &&
           mouse_position.x <= button_bounds.left + button_bounds.width &&
           mouse_position.y >= button_bounds.top &&
           mouse_position.y <= button_bounds.top + button_bounds.height)
            return true;
        return false;
    }

  private:

 };

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Clock clock;


    sf::Texture Start_tex;
    sf::Texture Exit_tex;
    sf::Texture BG_tex;
    if(!Start_tex.loadFromFile("Start.png")) {std::cerr << "Could not load texture" << std::endl; return 0;}
    if(!Exit_tex.loadFromFile("Exit.png")) {std::cerr << "Could not load texture" << std::endl; return 0;}
    if(!BG_tex.loadFromFile("BG.png")) {std::cerr << "Could not load texture" << std::endl; return 0;}

    Button StartB;
    StartB.setPosition( (window.getSize().x)/2 - 150.0,200.0);
    StartB.setTexture(Start_tex);

    Button ExitB;
    ExitB.setPosition((window.getSize().x)/2 - 150.0 ,400.0);
    ExitB.setTexture(Exit_tex);

    sf::Sprite BG;
    BG_tex.setRepeated(true);
    BG.setTexture(BG_tex);
    BG.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));



    bool game_started = false;

    while (window.isOpen()) {
       sf::Time elapsed = clock.restart();

       sf::Event event;
       while (window.pollEvent(event)) {
           if (event.type == sf::Event::Closed)
               window.close();
       }

       if(!game_started)//menu
       {
           sf::Vector2i mouse_pos;
           if(event.type == sf::Event::MouseButtonPressed)
           {
               if(event.mouseButton.button == sf::Mouse::Left){
                    mouse_pos = sf::Mouse::getPosition(window);
                    std::cerr << mouse_pos.x << " : " << mouse_pos.y  << std::endl;}

               if(StartB.isClicked(mouse_pos))
                   game_started = true;
               else if(ExitB.isClicked(mouse_pos))
               {
                   window.close();
                   return 1;
               }
           }
       }
       else//actual game
       {

       }




       window.clear(sf::Color::Black);
       window.draw(BG);

       if(!game_started)
       {
           window.draw(StartB);
           window.draw(ExitB);
       }

       window.display();
   }

   return 0;
}
