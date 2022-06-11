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

class Bow : public sf::Sprite
 {
  public:
    Bow( const sf::Texture& texture,const sf::Vector2u &w_size): sf::Sprite(texture)
    {
        setOrigin(60.0,30.0);
        setPosition(w_size.x/2, w_size.y-50);
        angle = getRotation();
    }


    void rotate_(const sf::Time &elapsed){
        float dt = elapsed.asSeconds();

        if(angle > 90 && angle < 100)
            rotation_speed *= -1;
        else if(angle < 270 && angle > 260)
            rotation_speed *= -1;

        rotate(rotation_speed*dt);
        angle = getRotation();
    }

  private:
    float angle;
    int rotation_speed=100;
 };

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Clock clock;

    //textures
    sf::Texture Start_tex;
    sf::Texture Exit_tex;
    sf::Texture BG_tex;
    sf::Texture Bow_tex;
    if(!Start_tex.loadFromFile("Start.png")) {std::cerr << "Could not load texture" << std::endl; return 0;}
    if(!Exit_tex.loadFromFile("Exit.png")) {std::cerr << "Could not load texture" << std::endl; return 0;}
    if(!BG_tex.loadFromFile("BG.png")) {std::cerr << "Could not load texture" << std::endl; return 0;}
    if(!Bow_tex.loadFromFile("Bow.png")) {std::cerr << "Could not load texture" << std::endl; return 0;}

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

    //game objects
    Bow bow(Bow_tex , window.getSize());



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
    timer_txt.setOutlineColor(sf::Color::Black);
    timer_txt.setOutlineThickness(1);

    sf::Text points_txt(std::to_string(points),font);
    points_txt.setCharacterSize(30);
    points_txt.setFillColor(sf::Color::Cyan);
    points_txt.setPosition(window.getSize().x - 20,5.0);
    points_txt.setOutlineColor(sf::Color::Black);
    points_txt.setOutlineThickness(1);

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

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) std::cerr << "Arrow" << std::endl;
        bow.rotate_(elapsed);
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
           window.draw(bow);
       }

       window.display();
   }

   return 0;
}
