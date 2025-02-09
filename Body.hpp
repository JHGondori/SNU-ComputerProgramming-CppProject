#ifndef BODY_HPP
#define BODY_HPP

#include<SFML/Graphics.hpp>
#include<cmath>
#include<vector>
#include<random>
#include<utility>
#include<memory>
#include<string.h>

class Body{ // 하나의 body의 자취, 속도, 위치를 표시하는 용도의 클래스

public:
    sf::CircleShape shape;
    sf::Color color;
    sf::CircleShape statshape;
    sf::Text stattext;
    float Body_X;
    float Body_Y;
    float Body_VX;
    float Body_VY;
    float Body_R;
    float trail_length = 1000; // body 자취 점 개수
    std::vector<sf::Vertex> vertices; // body의 자취
    std::vector<sf::Vertex> velocity; // body의 속도

    Body(float x, float y, float radius, float r, float g, float b);
    
    virtual ~Body() = default; // body를 소멸시킬 때 vector<Body>를 소멸시키는거라 default로 설정

    void move(float newX, float newY, float newVX, float newVY); // body의 stat (Body_X, Body_Y, Body_VX, Body_VY) 를 (NewX, NewY, NewVX, NewVY) 로 옮기기, 이 때 vertices에 자취 추가

    void showvelocity(); // 자신의 속도를 velocity vector를 설정하는 함수

    void showstat(sf::RenderWindow &window, sf::Font &font, int index);
};

#endif // BODY_HPP