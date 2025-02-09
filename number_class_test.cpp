//number class unit test

#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstring>
#include <iostream>
#include "Number.hpp"

// Assuming the Number class code is included here or in a header

// Test case for the Number class
// Test the constructor
TEST(NumberClassTest, ConstructorTest) {
    sf::Font font;
    ASSERT_TRUE(font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Rounded Bold.ttf"));

    Number num(100.f, 200.f, 30, 255, 0, 0, font);
    EXPECT_EQ(num.text.getString(), "0");
    EXPECT_EQ(num.value, 0);
}
//add test
TEST(NumberClassTest, AddValueTest) {
    sf::Font font;
    ASSERT_TRUE(font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Rounded Bold.ttf"));

    Number num(100.f, 200.f, 30, 255, 0, 0, font);
    num.addvalue("12");
    EXPECT_EQ(num.text.getString(), "012");
    EXPECT_EQ(num.value, 12);
}
//delete test
TEST(NumberClassTest, DeleteValueTest) {
    sf::Font font;
    ASSERT_TRUE(font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Rounded Bold.ttf"));

    Number num(100.f, 200.f, 30, 255, 0, 0, font);
    num.addvalue("1");
    num.addvalue("2");
    num.delvalue();
    EXPECT_EQ(num.text.getString(), "01");
    EXPECT_EQ(num.value, 1);
    num.delvalue();
    EXPECT_EQ(num.text.getString(), "0");
    EXPECT_EQ(num.value, 0);
}
//point test
TEST(NumberClassTest, AddPointTest) {
    sf::Font font;
    ASSERT_TRUE(font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Rounded Bold.ttf"));

    Number num(100.f, 200.f, 30, 255, 0, 0, font);
    num.addvalue("1");
    num.addvalue(".");
    num.addvalue("5");
    EXPECT_EQ(num.text.getString(), "01.5");
    EXPECT_EQ(num.value, 1.5);
}
//point error test
TEST(NumberClassTest, AddPointErrorTest) {
    sf::Font font;
    ASSERT_TRUE(font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Rounded Bold.ttf"));

    Number num(100.f, 200.f, 30, 255, 0, 0, font);
    num.addvalue(".");
    num.addvalue(".");
    EXPECT_EQ(num.text.getString(), "0..");
    EXPECT_EQ(num.value, 0);
    EXPECT_EQ(num.correction(), false);
}
//minus test
TEST(NumberClassTest, AddMinusTest) {
    sf::Font font;
    ASSERT_TRUE(font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Rounded Bold.ttf"));

    Number num(100.f, 200.f, 30, 255, 0, 0, font);
    num.delvalue();
    num.addvalue("-");
    num.addvalue("1");
    EXPECT_EQ(num.text.getString(), "-1");
    EXPECT_EQ(num.value, -1);
    num.addvalue(".");
    num.addvalue("5");
    EXPECT_EQ(num.text.getString(), "-1.5");
    EXPECT_EQ(num.value, -1.5);
}
//minus error test
TEST(NumberClassTest, AddMinusErrorTest) {
    sf::Font font;
    ASSERT_TRUE(font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Rounded Bold.ttf"));

    Number num(100.f, 200.f, 30, 255, 0, 0, font);
    num.addvalue("-");
    EXPECT_EQ(num.text.getString(), "0-");
    EXPECT_EQ(num.value, 0);
    EXPECT_EQ(num.correction(), false);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}