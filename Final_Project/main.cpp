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

    //menu textures
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


    //in-game variables
    bool game_started = false;
    float timer = 60;
    int points = 0;

    //game text
    sf::Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

    sf::Text timer_txt(std::to_string(timer),font);
    timer_txt.setCharacterSize(30);
    timer_txt.setFillColor(sf::Color::Red);
    timer_txt.setPosition(5.0,5.0);

    sf::Text points_txt(std::to_string(points),font);
    points_txt.setCharacterSize(30);
    points_txt.setFillColor(sf::Color::Cyan);
    points_txt.setPosition(window.getSize().x - 20,5.0);


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
        timer -= elapsed.asSeconds();
        timer_txt.setString(std::to_string(timer));
        if(timer < 0)
        {
            //game ended screen
        }
        //continue
       }




       window.clear(sf::Color::Black);
       window.draw(BG);

       if(!game_started)
       {
           window.draw(StartB);
           window.draw(ExitB);
       }
       else{
           window.draw(timer_txt);
           window.draw(points_txt);
       }

       window.display();
   }

   return 0;
}
