#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <random>
#include <utility>
#include <memory>
#include <string.h>
#include "Number.hpp"
#include "Body.hpp"
#include "RK4.hpp"
#include "Button.hpp"

int main() {
    //generate window
    sf::RenderWindow window(sf::VideoMode(1600, 800), "N - Body Simulator");
    
    //time check
    sf::Clock clock;

    //random value
    std::random_device rd;
    std::uniform_int_distribution<int> dis(0, 255);

    int body_N = 0; // body number

    int N = 50000; // simulation total frame

    std::vector<sf::Vector2f> R, V; // position and velocity

    std::vector<std::unique_ptr<Body> > body; // body class

    sf::Font font;
    font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Rounded Bold.ttf"); // load font in mac

    if(!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Rounded Bold.ttf")) printf("error");

    Button Run(10, 10, 70, 30, "Run", 0, 255, 0, font, true); // Run button
    Button Add(10, 50, 70, 30, "Add", 0, 0, 255, font, true); // Add body button
    Button Set_x(10, 90, 70, 30, "Set X", 50, 50, 50, font, false); // Set body x button
    Number X(90, 100, 25, 255, 255, 255, font);

    Button Set_y(10, 130, 70, 30, "Set Y", 50, 50, 50, font, false); // Set body y button
    Number Y(90, 140, 25, 255, 255, 255, font);

    Button Set_Vx(10, 170, 70, 30, "Set Vx", 80, 80, 80, font, false); // Set body v_x button
    Number VX(90, 180, 25, 255, 255, 255, font);

    Button Set_Vy(10, 210, 70, 30, "Set Vy", 80, 80, 80, font, false); // Set body v_y button
    Number VY(90, 220, 25, 255, 255, 255, font);

    Button Done(10, 250, 70, 30, "Done", 100, 100, 100, font, false); // body setting end button
    Button Del(90, 50, 70, 30, "Delete", 255, 0, 0, font, true); // Delete body button
    Button Pause(10, 10, 70, 30, "Pause", 0, 255, 255, font, false); // Pause button
    Button Restart(10, 10, 70, 30, "Restart", 0, 0, 255, font, false); // Restart button
    Button Stop(90, 10, 70, 30, "Stop", 255, 0, 0, font, false); // Stop button

    std::vector<std::vector<std::vector<sf::Vector2f> > > RK4result(100, std::vector<std::vector<sf::Vector2f> >(N, std::vector<sf::Vector2f>(2))); // simulation result

    std::vector<float> v; // RK4 body 임시 설정
    v.assign(1, 1);
    RK4 bodies(v);

    int running_time = 0; // 얘는 운동 시간이라고 보면 됨.
    bool draw_run = false;
    bool paused = false;
    bool calc = false;
    bool AddAct = false;
    bool DelAct = false;
    bool SetxAct = false;
    bool SetyAct = false;
    bool SetvxAct = false;
    bool SetvyAct = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                if(Run.mouseon(window) && Run.activated) {
                    //Wait a minute 출력해야됨
                    calc = true;
                    Set_x.activated = false;
                    Set_y.activated = false;
                    Set_Vx.activated = false;
                    Set_Vy.activated = false;
                    Done.activated = false;

                    Run.activated = false;
                    Add.activated = false;
                    Del.activated = false;

                    Pause.activated = true;
                    Stop.activated = true;
                }
                else if(Add.mouseon(window) && Add.activated){
                    AddAct = true;
                    Set_x.activated = true;
                    Set_y.activated = true;
                    Set_Vx.activated = true;
                    Set_Vy.activated = true;
                    Done.activated = true;

                    Run.activated = false;
                    Add.activated = true;
                    Del.activated = false;

                    body_N++;
                    R.push_back(sf::Vector2f(0, 0));
                    V.push_back(sf::Vector2f(0, 0));
                    body.push_back(std::unique_ptr<Body>(new Body(0, 0, 7, dis(rd), dis(rd), dis(rd))));

                    X = Number(90, 100, 25, 255, 255, 255, font);
                    Y = Number(90, 140, 25, 255, 255, 255, font);
                    VX = Number(90, 180, 25, 255, 255, 255, font);
                    VY = Number(90, 220, 25, 255, 255, 255, font);
                    body[body_N - 1]->showvelocity();
                }

                else if(Del.mouseon(window) && Del.activated){
                    DelAct = true;
                    Run.activated = false;
                    Add.activated = false;
                    Del.activated = false;

                    Done.activated = true;
                }

                else if(Set_x.mouseon(window) && Set_x.activated){
                    SetxAct = true;
                    SetyAct = false;
                    SetvxAct = false;
                    SetvyAct = false;            
                }

                else if(Set_y.mouseon(window) && Set_y.activated){
                    SetxAct = false;
                    SetyAct = true;
                    SetvxAct = false;
                    SetvyAct = false;
                }

                else if(Set_Vx.mouseon(window) && Set_Vx.activated){
                    SetxAct = false;
                    SetyAct = false;
                    SetvxAct = true;
                    SetvyAct = false;
                }

                else if(Set_Vy.mouseon(window) && Set_Vy.activated){
                    SetxAct = false;
                    SetyAct = false;
                    SetvxAct = false;
                    SetvyAct = true;
                } 

                else if(Done.mouseon(window) && Done.activated){
                    DelAct = false; // Delete unactivated
                    AddAct = false; // Add unactivated
                    Set_x.activated = false;
                    Set_y.activated = false;
                    Set_Vx.activated = false;
                    Set_Vy.activated = false;
                    Done.activated = false;

                    Run.activated = true;
                    Add.activated = true;
                    Del.activated = true;

                    SetxAct = false;
                    SetyAct = false;
                    SetvxAct = false;
                    SetvyAct = false;
                }

                else if(Pause.mouseon(window) && Pause.activated){
                    paused = true;
                    Pause.activated = false;
                    Restart.activated = true;
                }

                else if(Restart.mouseon(window) && Restart.activated){
                    paused = false;
                    Pause.activated = true;
                    Restart.activated = false;
                }

                else if(Stop.mouseon(window) && Stop.activated){//모든 것을 초기화
                    draw_run = false;
                    paused = false;

                    running_time = 0;

                    R.clear();
                    R = std::vector<sf::Vector2f>();

                    V.clear();
                    V = std::vector<sf::Vector2f>();
                    body.clear();
                    body = std::vector<std::unique_ptr<Body> >();

                    body_N = 0;
                    bodies.M.assign(1, 1);

                    bodies.merged.clear();
                    bodies.merged = std::vector<std::vector<std::vector<int> > >();

                    RK4result.clear();
                    RK4result = std::vector<std::vector<std::vector<sf::Vector2f> > >(100, std::vector<std::vector<sf::Vector2f> >(N, std::vector<sf::Vector2f>(2)));

                    Restart.activated = false;
                    Pause.activated = false;
                    Stop.activated = false;

                    Run.activated = true;
                    Add.activated = true;
                    Del.activated = true;

                }

                else if(DelAct){
                    //get mouse position
                    sf::Vector2f mp;
                    int mpx = sf::Mouse::getPosition(window).x;
                    int mpy = sf::Mouse::getPosition(window).y;
                    mp.x = ((float)mpx - 800.0f) / 50.0f;
                    mp.y = - ((float)mpy - 400.0f) / 50.0f;

                    for(int i = body_N - 1; i >= 0; i--){ // 최근에 추가된 body를 제거하기 위함.
                        float distanceMnP = (R[i].x - mp.x) * (R[i].x - mp.x) + (R[i].y - mp.y) * (R[i].y - mp.y);
                        if(distanceMnP <= 0.14 * 0.14){
                            R.erase(R.begin() + i);
                            V.erase(V.begin() + i);
                            body.erase(body.begin() + i); // 마우스가 거리 안에 있으면 지우기
                            body_N--;
                            break; // 지울 놈 찾으면 나가기
                        }
                    }
                }
            }
            if(event.type == sf::Event::KeyPressed && AddAct){
                if(event.key.code == sf::Keyboard::Num0){
                    if(SetxAct) X.addvalue("0");
                    if(SetyAct) Y.addvalue("0");
                    if(SetvxAct) VX.addvalue("0");
                    if(SetvyAct) VY.addvalue("0");
                }
                if(event.key.code == sf::Keyboard::Num1){
                    if(SetxAct) X.addvalue("1");
                    if(SetyAct) Y.addvalue("1");
                    if(SetvxAct) VX.addvalue("1");
                    if(SetvyAct) VY.addvalue("1");
                }
                if(event.key.code == sf::Keyboard::Num2){
                    if(SetxAct) X.addvalue("2");
                    if(SetyAct) Y.addvalue("2");
                    if(SetvxAct) VX.addvalue("2");
                    if(SetvyAct) VY.addvalue("2");
                }
                if(event.key.code == sf::Keyboard::Num3){
                    if(SetxAct) X.addvalue("3");
                    if(SetyAct) Y.addvalue("3");
                    if(SetvxAct) VX.addvalue("3");
                    if(SetvyAct) VY.addvalue("3");
                }
                if(event.key.code == sf::Keyboard::Num4){
                    if(SetxAct) X.addvalue("4");
                    if(SetyAct) Y.addvalue("4");
                    if(SetvxAct) VX.addvalue("4");
                    if(SetvyAct) VY.addvalue("4");
                }
                if(event.key.code == sf::Keyboard::Num5){
                    if(SetxAct) X.addvalue("5");
                    if(SetyAct) Y.addvalue("5");
                    if(SetvxAct) VX.addvalue("5");
                    if(SetvyAct) VY.addvalue("5");
                }
                if(event.key.code == sf::Keyboard::Num6){
                    if(SetxAct) X.addvalue("6");
                    if(SetyAct) Y.addvalue("6");
                    if(SetvxAct) VX.addvalue("6");
                    if(SetvyAct) VY.addvalue("6");
                }
                if(event.key.code == sf::Keyboard::Num7){
                    if(SetxAct) X.addvalue("7");
                    if(SetyAct) Y.addvalue("7");
                    if(SetvxAct) VX.addvalue("7");
                    if(SetvyAct) VY.addvalue("7");
                }
                if(event.key.code == sf::Keyboard::Num8){
                    if(SetxAct) X.addvalue("8");
                    if(SetyAct) Y.addvalue("8");
                    if(SetvxAct) VX.addvalue("8");
                    if(SetvyAct) VY.addvalue("8");
                }
                if(event.key.code == sf::Keyboard::Num9){
                    if(SetxAct) X.addvalue("9");
                    if(SetyAct) Y.addvalue("9");
                    if(SetvxAct) VX.addvalue("9");
                    if(SetvyAct) VY.addvalue("9");
                }
                //음수
                if(event.key.code == sf::Keyboard::Hyphen){
                    if(SetxAct) X.addvalue("-");
                    if(SetyAct) Y.addvalue("-");
                    if(SetvxAct) VX.addvalue("-");
                    if(SetvyAct) VY.addvalue("-");
                }
                //소수점
                if(event.key.code == sf::Keyboard::Period){
                    if(SetxAct) X.addvalue(".");
                    if(SetyAct) Y.addvalue(".");
                    if(SetvxAct) VX.addvalue(".");
                    if(SetvyAct) VY.addvalue(".");
                }
                //지우기
                if(event.key.code == sf::Keyboard::Backspace){
                    if(SetxAct) X.delvalue();
                    if(SetyAct) Y.delvalue();
                    if(SetvxAct) VX.delvalue();
                    if(SetvyAct) VY.delvalue();
                }
                //body 이동
                if(X.correction() && Y.correction() && VX.correction() && VY.correction()){
                    R[body_N - 1].x = X.value;
                    R[body_N - 1].y = Y.value;
                    V[body_N - 1].x = VX.value;
                    V[body_N - 1].y = VY.value;
                    body[body_N - 1]->Body_X = X.value;
                    body[body_N - 1]->Body_Y = Y.value;
                    body[body_N - 1]->Body_VX = VX.value;
                    body[body_N - 1]->Body_VY = VY.value;
                    body[body_N - 1]->showvelocity(); // 방향 설정
                }
            }
        }

        Run.mouse_effect(window);
        Add.mouse_effect(window);
        Set_x.mouse_effect(window);
        Set_y.mouse_effect(window);
        Set_Vx.mouse_effect(window);
        Set_Vy.mouse_effect(window);
        Done.mouse_effect(window);
        Del.mouse_effect(window);
        Pause.mouse_effect(window);
        Restart.mouse_effect(window);
        Stop.mouse_effect(window);

        // Draw the bodies
        // Calculate moving
        if(calc){
            for(int i = 1; i < body_N; i++) bodies.M.push_back(1);
            RK4result = bodies.Run(body_N, 0.001, N, R, V, bodies.M, window, font);
            draw_run = true;
            calc = false;
        }   
        window.clear(); // clear window
        //합쳐지면 크기를 늘리도록 함

        for(int i = 0; i < body_N; i++){
            //기본적인 설정
            body[i]->shape.setPosition(sf::Vector2f(800.0f + 50.0f * body[i]->Body_X, 400.0f - 50.0f * body[i]->Body_Y));
            body[i]->shape.setFillColor(body[i]->color);

            if(draw_run == true){
                float count = bodies.M[i];
                for(int j = 0; j < body_N; j++){
                    if(j == i) continue;
                    if(bodies.merged[running_time][i][j] == 1) count += bodies.M[j];
                }
                body[i]->shape.setRadius(body[i]->Body_R * std::sqrt(count)); // count만큼 크기 곱해서 표현하기(원래는 뭐 세제곱근 해야하지만 넘기죠)
                body[i]->shape.setOrigin(body[i]->Body_R * std::sqrt(count), body[i]->Body_R * std::sqrt(count));
                if(!paused) window.draw(&body[i]->vertices[0], body[i]->vertices.size(), sf::Points);
            }
            //draw가 false일 때는 merged가 만들어지지 않은 때이므로 예외처리
            else {
                body[i]->shape.setRadius(body[i]->Body_R);
                body[i]->shape.setOrigin(body[i]->Body_R, body[i]->Body_R);
                window.draw(&body[i]->velocity[0], body[i]->velocity.size(), sf::Lines);
                body[i]->showstat(window, font, i);
            }
            window.draw(body[i]->shape);
        }

        if(draw_run == true){
            if(running_time < N){
                if(paused){
                    for(int i = 0; i < body_N; i++) {
                        body[i]->showvelocity();
                        window.draw(&body[i]->velocity[0], body[i]->velocity.size(), sf::Lines);
                        body[i]->showstat(window, font, i);
                    }
                }
                for(int i = 0; i < body_N; i++) body[i]->move(RK4result[i][running_time][0].x, RK4result[i][running_time][0].y, RK4result[i][running_time][1].x, RK4result[i][running_time][1].y);
                if(!paused) running_time++;
            }
            else if(running_time >= N){
                draw_run = false;
                paused = false;

                running_time = 0;

                R.clear();
                R = std::vector<sf::Vector2f>();

                V.clear();
                V = std::vector<sf::Vector2f>();
                body.clear();
                body = std::vector<std::unique_ptr<Body> >();

                body_N = 0;
                bodies.M.assign(1, 1);

                bodies.merged.clear();
                bodies.merged = std::vector<std::vector<std::vector<int> > >();

                RK4result.clear();
                RK4result = std::vector<std::vector<std::vector<sf::Vector2f> > >(100, std::vector<std::vector<sf::Vector2f> >(N, std::vector<sf::Vector2f>(2)));

                Restart.activated = false;
                Pause.activated = false;
                Stop.activated = false;

                Run.activated = true;
                Add.activated = true;
                Del.activated = true;
            }
        }

        // Draw buttons
        Run.draw_button(window);
        Add.draw_button(window);
        Set_x.draw_button(window);
        Set_y.draw_button(window);
        Set_Vx.draw_button(window);
        Set_Vy.draw_button(window);
        Del.draw_button(window);
        Done.draw_button(window);
        Pause.draw_button(window);
        Restart.draw_button(window);
        Stop.draw_button(window);

        if(AddAct){
            window.draw(X.text);
            window.draw(Y.text);
            window.draw(VX.text);
            window.draw(VY.text);
        }

        window.display();
    }
    
    return 0;
}