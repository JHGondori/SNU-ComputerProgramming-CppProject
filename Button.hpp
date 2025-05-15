#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>   
#include <cmath>
#include <vector>
#include <random>
#include <utility>
#include <memory>
#include <string.h>

class Button{

public:
    sf::RectangleShape shape;
    sf::Color color;
    sf::Text text;

    float posx, posy, Lx, Ly;

    bool activated;
    char name[100];
    Button(float x, float y, float l_x, float l_y, const char *n, int r, int g, int b, const sf::Font &font, bool activ);
    
    bool mouseon(sf::RenderWindow &window);

    void Act();

    void unAct();

    void mouse_effect(sf::RenderWindow &window);

    void draw_button(sf::RenderWindow &window);
};

#endif // BUTTON_HPP