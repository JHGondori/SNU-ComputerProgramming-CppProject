#include "Number.hpp"

Number::Number(float x, float y, int size, int r, int g, int b, const sf::Font &font) 
    : text(font) {
    text.setString("0");
    text.setCharacterSize(size);

    color.r = r;
    color.g = g;
    color.b = b;

    sf::FloatRect rc = text.getLocalBounds();
    text.setOrigin({0, rc.size.y / 2});

    text.setPosition({x, y});
    text.setFillColor(color);
    text.setStyle(sf::Text::Regular);

    value = 0;
    strcpy(number, "0");
}


bool Number::correction(){
    int len = strlen(number);
    int ecount = 0;
    bool minus = false;
    //맞는 숫자인지 확인
    for(int i = 0; i < len; i++) {
        if(number[i] == '.') ecount++;
        if(number[i] == '-' && i!=0){
            color = sf::Color::Red;
            text.setFillColor(color);
            return false;
        }
    }
    if(ecount > 1){
        color = sf::Color::Red;
        text.setFillColor(color);
        return false;
    }
    float checkvalue = 0;
    int point = -1;
    for(int i = 0; i < len; i++){
        if(number[i] >= '0' && number[i] <= '9'){
            if(point < 0){
                checkvalue *= 10;
                checkvalue += number[i] - '0';
            }
            else checkvalue += (number[i] - '0') * pow(0.1, i - point);
        }
        else if(number[i] == '-' && i == 0) minus = true;
        else point = i;
    }
    if(minus) checkvalue *= -1;
    value = checkvalue;
    color = sf::Color::White;
    text.setFillColor(color);
    return true;
}

void Number::addvalue(const char * n){
    int len = strlen(number);
    number[len] = n[0];
    number[len + 1] = '\0';
    text.setString(number);
    correction();
}

void Number::delvalue(){
    int len = strlen(number);
    if(len == 0) return;
    number[len - 1] = '\0';
    text.setString(number);
    correction();
}