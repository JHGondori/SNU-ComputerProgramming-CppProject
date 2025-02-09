#include "RK4.hpp"

RK4::RK4(std::vector<float> m){
    for(int i = 0; i < m.size(); i++) {
        M.push_back(m[i]);
    }
}

sf::Vector2f RK4::EOM(std::vector<sf::Vector2f> R, std::vector<float> M, int index, int time){
    if(merged.size() == time){ // time 때 처음 호출된 EOM, 즉 merge 판단을 위해 호출된 함수
        merged.push_back(std::vector<std::vector<int> >(M.size(), std::vector<int>(M.size(), 0)));
    }

    sf::Vector2f dv_dt;
    bool first_update = true;
    for(int i = 0; i < R.size(); i++){
        if(i == index || merged[time][i][index] == 1) continue; // index랑 i가 같으면 pass 또는 둘이 합쳐진거라면 pass

        sf::Vector2f diff = R[index] - R[i]; // 거리를 구해서 힘을 구함
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        float distance_square = distance * distance;
        if(distance < 0.02f){
            merged[time][i][index] = 1;
            merged[time][index][i] = 1;
            continue;
        }
        if(first_update) {
            dv_dt = - G * M[i] / distance_square * diff / distance;
            first_update = false;
        }
        else dv_dt += - G * M[i] / distance_square * diff / distance;
    }

    return dv_dt;
}

std::vector<std::vector<std::vector<sf::Vector2f> > > RK4::Run(int body_N, float dt, int N, std::vector<sf::Vector2f> R_0, std::vector<sf::Vector2f> V_0, std::vector<float> M, sf::RenderWindow &window, sf::Font &font){
    sf::Text Wait; // 계산할 때 얼마나 진행되는지 시각적으로 표시하기 위한 text
    Wait.setString("Calculating");
    Wait.setCharacterSize(50);
    Wait.setFillColor(sf::Color::White);
    Wait.setPosition(0, 700);
    Wait.setOrigin(0, 0);
    Wait.setFont(font);

    //본격적인 계산
    std::vector<std::vector<sf::Vector2f> > R(N, std::vector<sf::Vector2f>(body_N));
    for(int i = 0; i < body_N; i++) R[0][i] = R_0[i];

    std::vector<std::vector<sf::Vector2f> > V(N, std::vector<sf::Vector2f>(body_N));
    for(int i = 0; i < body_N; i++) V[0][i] = V_0[i];

    for(int i = 1; i < N; i++){
        std::vector<sf::Vector2f> tempR(body_N);//유용하게 쓰일 놈

        // k1
        std::vector<sf::Vector2f> k1_R(body_N);
        for(int j = 0; j < body_N; j++) k1_R[j] = V[i-1][j];

        std::vector<sf::Vector2f> k1_V(body_N);
        for(int j = 0; j < body_N; j++) k1_V[j] = EOM(R[i-1], M, j, i-1);

        // k2
        std::vector<sf::Vector2f> k2_R(body_N);
        for(int j = 0; j < body_N; j++) k2_R[j] = V[i-1][j] + dt * k1_V[j] / 2.0f;

        std::vector<sf::Vector2f> k2_V(body_N);
        for(int j = 0; j < body_N; j++) tempR[j] = R[i-1][j] + dt * k1_R[j] / 2.0f;
        for(int j = 0; j < body_N; j++) k2_V[j] = EOM(tempR, M, j, i-1);

        // k3
        std::vector<sf::Vector2f> k3_R(body_N);
        for(int j = 0; j < body_N; j++) k3_R[j] = V[i-1][j] + dt * k2_V[j] / 2.0f;

        std::vector<sf::Vector2f> k3_V(body_N);
        for(int j = 0; j < body_N; j++) tempR[j] = R[i-1][j] + dt * k2_R[j] / 2.0f;
        for(int j = 0; j < body_N; j++) k3_V[j] = EOM(tempR, M, j, i-1);

        // k4
        std::vector<sf::Vector2f> k4_R(body_N);
        for(int j = 0; j < body_N; j++) k4_R[j] = V[i-1][j] + dt * k3_V[j];

        std::vector<sf::Vector2f> k4_V(body_N);
        for(int j = 0; j < body_N; j++) tempR[j] = R[i-1][j] + dt * k3_R[j];
        for(int j = 0; j < body_N; j++) k4_V[j] = EOM(tempR, M, j, i-1);
        //update
        for(int j = 0; j < body_N; j++) R[i][j] = R[i-1][j] + dt / 6.0f * (k1_R[j] + k2_R[j] * 2.0f + k3_R[j] * 2.0f + k4_R[j]);

        for(int j = 0; j < body_N; j++) V[i][j] = V[i-1][j] + dt / 6.0f * (k1_V[j] + k2_V[j] * 2.0f + k3_V[j] * 2.0f + k4_V[j]);

        //merge check
        for(int j = 0; j < body_N; j++) EOM(R[i], M, j, i);

        //merge -> set position to average of position and do same to velocity
        std::vector<int> check(body_N);
        float count = 0;
        for(int j = 0; j < body_N; j++){
            count = M[j];
            if(check[j]==1) continue;
            check[j] = 1;
            sf::Vector2f tempR = M[j] * R[i][j];
            sf::Vector2f tempV = M[j] * V[i][j];
            for(int k = j+1; k < body_N; k++){
                if(merged[i][j][k] == 1){
                    check[k] = 1;
                    count += M[k];
                    tempR += M[k] * R[i][k]; 
                    tempV += M[k] * V[i][k];
                }
            }
            for(int k = j; k < body_N; k++){
                if(merged[i][j][k] == 1 || (k == j && count > 0)){
                    R[i][k] = tempR / count; // average of position
                    V[i][k] = tempV / count; // average of velocity
                }
            }
        }
        // 계산중 띄우기
        if(i == 1){
            window.draw(Wait);
            window.display();
        }
        else if(i == N/4){
            Wait.setString("Calculating.");
            window.draw(Wait);
            window.display();
        }
        else if(i == N/2){
            Wait.setString("Calculating..");
            window.draw(Wait);
            window.display();
        }
        else if(i == 3 * N /4){
            Wait.setString("Calculating...");
            window.draw(Wait);
            window.display();
        }

    }

    std::vector<std::vector<std::vector<sf::Vector2f> > > RK4result(body_N, std::vector<std::vector<sf::Vector2f> >(N, std::vector<sf::Vector2f>(2)));

    for(int i = 0; i < body_N; i++) {
        for(int j = 0; j < N; j++) {
            RK4result[i][j][0] = R[j][i];
            RK4result[i][j][1] = V[j][i];
        }
    }

    return RK4result;
}