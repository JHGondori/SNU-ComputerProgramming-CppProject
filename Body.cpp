#include "Body.hpp"

Body::Body(float x, float y, float radius, float r, float g, float b){
    Body_X = x;
    Body_Y = y;
    Body_VX = 0; // 처음 만들어지는 body는 속도가 (0, 0)으로 설정
    Body_VY = 0;
    Body_R = radius;
    color.r = r;
    color.g = g;
    color.b = b;
}

void Body::move(float newX, float newY, float newVX, float newVY){
    Body_X = newX;
    Body_Y = newY;
    Body_VX = newVX;
    Body_VY = newVY;

    vertices.push_back(sf::Vertex(sf::Vector2f(800.0f + 50.0f * newX, 400.0f - 50.0f * newY), color));

    if(vertices.size() > trail_length) vertices.erase(vertices.begin());
}

void Body::showvelocity(){
    velocity.clear();
    velocity = std::vector<sf::Vertex>();
    velocity.push_back(sf::Vertex(sf::Vector2f(800.0f + 50.0f * Body_X, 400.0f - 50.0f * Body_Y), color));
    velocity.push_back(sf::Vertex(sf::Vector2f(800.0f + 50.0f * (Body_X + Body_VX), 400.0f - 50.0f * (Body_Y + Body_VY)), sf::Color::White));
}

void Body::showstat(sf::RenderWindow &window, sf::Font &font, int index){
    char STAT[100];  // 충분히 큰 버퍼 크기 설정
    snprintf(STAT, 100, "(%.6f, %.6f, %.6f, %.6f)", Body_X, Body_Y, Body_VX, Body_VY);

    statshape.setFillColor(color);
    statshape.setPosition(1550, 55 + 40 * index);
    statshape.setRadius(Body_R);
    statshape.setOrigin(Body_R, Body_R);

    stattext.setFont(font);
    stattext.setFillColor(sf::Color::White);
    stattext.setCharacterSize(14);
    stattext.setString(STAT);

    sf::FloatRect rc = stattext.getLocalBounds();
    stattext.setOrigin(rc.width, rc.height / 2);
    stattext.setPosition(1530, 55 + 40 * index);

    sf::Text explain;
    explain.setFont(font);
    explain.setFillColor(sf::Color::White);
    explain.setString("State(X, Y, V_x, V_y)");
    explain.setPosition(1250, 5);
    explain.setCharacterSize(30);

    window.draw(explain);
    window.draw(statshape);
    window.draw(stattext);

    return;    
}