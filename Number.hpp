#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <SFML/Graphics.hpp>
#include <cstring>
#include <cmath>

class Number {
public:
    sf::Text text; // 숫자 표시
    sf::Color color;
    char number[100];
    float value;
    
    Number(float x, float y, int size, int r, int g, int b, const sf::Font &font);
    bool correction();
    void addvalue(const char * n);
    void delvalue();
};

#endif // NUMBER_HPP
