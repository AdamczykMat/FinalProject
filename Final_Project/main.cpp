#include <iostream>
#include <vector>
#include <cmath>

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

    int getRotationSpeed(){return rotation_speed;}

  private:
    float angle;
    int rotation_speed=150;
 };

class Arrow : public sf::Sprite
 {
  public:
    Arrow( const sf::Texture& texture,const sf::Vector2u &w_size): sf::Sprite(texture)
    {
        setOrigin(15.0,60.0);
        setPosition(w_size.x/2, w_size.y-50);
        angle = getRotation();
    }

    void fire(){is_fired = true;}


    void move_(const sf::Time &elapsed ,const sf::Vector2u &w_size, const sf::FloatRect &target_bounds,const sf::Vector2f &bow_rot){

        float dt = elapsed.asSeconds();


        if(!is_fired)
        {
            // same rotation as bow
            if(angle > 90 && angle < 100)
                rotation_speed *= -1;
            else if(angle < 270 && angle > 260)
                rotation_speed *= -1;

            rotate(rotation_speed*dt);
            angle = getRotation();
        }
        else
        {
            sf::FloatRect bounds = getGlobalBounds();

            if(bounds.left < 0 || bounds.left + bounds.width > w_size.x || bounds.top < 0 || hit_target(target_bounds))
            {
            setPosition(w_size.x/2, w_size.y-50);
            setRotation(bow_rot.x);
            rotation_speed = bow_rot.y;
            is_fired = false;
            angle = getRotation();
            }

            if(is_fired)
            {
                float v = xy_speed*dt; // movement vector
                float angle_in_rad = 2*3.14 * (std::abs(90-angle) / 360);


                if(angle < 180)
                    move(std::abs(cos(angle_in_rad)*v),std::abs(sin(angle_in_rad)*v)*-1); //right side fire
                else
                    move(std::abs(cos(angle_in_rad)*v)*-1,std::abs(sin(angle_in_rad)*v)*-1); //left side fire
            }


        }
    }

    bool hit_target(const sf::FloatRect &target_bounds)
    {
        sf::FloatRect bounds = getGlobalBounds();

        if(bounds.top > target_bounds.top && bounds.top < target_bounds.top + target_bounds.height && bounds.left > target_bounds.left && bounds.left < target_bounds.left + target_bounds.width)
            return true;
        return false;
    }

  private:

    float angle;
    int rotation_speed=150;
    int xy_speed = 750;
    bool is_fired = 0;
 };


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Clock clock;

    //textures
    sf::Texture Start_tex;
    sf::Texture Exit_tex;
    sf::Texture BG_tex;
    sf::Texture Bow_tex;
    sf::Texture Arrow_tex;
    if(!Start_tex.loadFromFile("Start.png") ||
       !Exit_tex.loadFromFile("Exit.png") ||
       !BG_tex.loadFromFile("BG.png") ||
       !Bow_tex.loadFromFile("Bow.png") ||
       !Arrow_tex.loadFromFile("Arrow.png"))
    {std::cerr << "Could not load texture" << std::endl; return 0;}

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
    Arrow arrow(Arrow_tex , window.getSize());

    sf::CircleShape Target(25.0);
    Target.setPosition(rand() % window.getSize().x - Target.getGlobalBounds().height ,rand() % window.getSize().y/3);
    Target.setFillColor(sf::Color(rand()%255, rand()%255, rand()%255));
    Target.setOutlineColor(sf::Color::Black);
    Target.setOutlineThickness(2);

    //in-game variables
    bool game_started = false;
    float timer = 60;
    int points = 0;

    //game text
    sf::Font font;
    font.loadFromFile("arial.ttf");

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

    sf::Text restart_text("Press 'R' to restart",font);
    restart_text.setCharacterSize(50);
    restart_text.setFillColor(sf::Color::Red);
    restart_text.setPosition(200,window.getSize().y /2);
    restart_text.setOutlineColor(sf::Color::Black);
    restart_text.setOutlineThickness(1);

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

        points_txt.setString(std::to_string(points));


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) arrow.fire();
        bow.rotate_(elapsed);
        arrow.move_(elapsed,window.getSize(),Target.getGlobalBounds(),sf::Vector2f(bow.getRotation(),bow.getRotationSpeed()));

        if(arrow.hit_target(Target.getGlobalBounds()))
        {
            points++;
            Target.setPosition(rand() % window.getSize().x ,rand() % window.getSize().y/2);
            Target.setFillColor(sf::Color(rand()%255, rand()%255, rand()%255));
        }

        if(timer < 0)
        {
            //game ended screen

            window.clear(sf::Color::Black);
            window.draw(BG);
            window.draw(points_txt);
            window.draw(restart_text);

            window.display();
            while(true)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    bow.setRotation(180);
                    arrow.setRotation(180);
                    timer = 60.0;
                    points = 0;
                    break;
                }
            }
        }
       }


       window.clear(sf::Color::Black);
       window.draw(BG);

       if(!game_started)
       {
           window.draw(StartB);
           window.draw(ExitB);
       }
       else{

           window.draw(bow);
           window.draw(arrow);
           window.draw(Target);
           window.draw(timer_txt);
           window.draw(points_txt);
       }

       window.display();
   }

   return 0;
}
