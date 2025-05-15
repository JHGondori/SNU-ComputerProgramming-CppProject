#include "Button.hpp"

Button::Button(float x, float y, float l_x, float l_y, const char *n, int r, int g, int b, const sf::Font &font, bool activ) : text(font) {
    posx = x; posy = y; Lx = l_x; Ly = l_y;

    shape.setPosition(sf::Vector2f(x, y));
    shape.setSize(sf::Vector2f(l_x, l_y));
    strcpy(name, n);
    color.r = r;
    color.g = g;
    color.b = b;

    shape.setFillColor(color);

    text.setString(n);
    text.setCharacterSize(int(l_y - 12));

    sf::FloatRect rc = text.getLocalBounds();
    text.setOrigin({rc.size.x / 2, rc.size.y / 2});

    text.setPosition({x + l_x / 2, y + l_y / 3});
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Regular);

    activated = activ;
}

bool Button::mouseon(sf::RenderWindow &window){
    int mpx = sf::Mouse::getPosition(window).x;
    int mpy = sf::Mouse::getPosition(window).y;
    if(activated && mpx>=shape.getPosition().x && mpx<=shape.getPosition().x+shape.getSize().x && mpy>=shape.getPosition().y && mpy<=shape.getPosition().y+shape.getSize().y) return true;
    else return false;
}

void Button::Act(){
    shape.setPosition(sf::Vector2f(posx - 5, posy - 5));
    shape.setSize(sf::Vector2f(Lx + 10, Ly + 10));
    return;
}

void Button::unAct(){
    shape.setPosition(sf::Vector2f(posx, posy));
    shape.setSize(sf::Vector2f(Lx, Ly));
    return;
}

void Button::mouse_effect(sf::RenderWindow &window){
    if(mouseon(window)) Act();
    else unAct();
    return;
}

void Button::draw_button(sf::RenderWindow &window){
    if(activated){
        window.draw(shape);
        window.draw(text);
    }
    return;
}