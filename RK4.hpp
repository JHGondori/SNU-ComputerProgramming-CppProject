#ifndef RK4_HPP
#define RK4_HPP

#include<SFML/Graphics.hpp>
#include<cmath>
#include<vector>
#include<random>
#include<utility>
#include<memory>
#include<string.h>

class RK4{

public:

    std::vector<std::vector<std::vector<int> > > merged; // body가 합쳐졌는지 여부를 확인하는 클래스, merged[시간][body_1][body_2] = true of false
    std::vector<float> M; // 운동을 기술하는 body의 질량(모두 1로 설정)
    RK4(std::vector<float> m);

    sf::Vector2f EOM(std::vector<sf::Vector2f> R, std::vector<float> M, int index, int time); //Equation of Motion, returns acceleration of an body
    std::vector<std::vector<std::vector<sf::Vector2f> > > Run(int body_N, float dt, int N, std::vector<sf::Vector2f> R_0, std::vector<sf::Vector2f> V_0, std::vector<float> M, sf::RenderWindow &window, sf::Font &font); //Use EOM and RK4, get X positions, Y positions, velocity_x, velocity_y, N is iterations

private:
    float G = 1; // 중력상수
};

#endif // RK4_HPP