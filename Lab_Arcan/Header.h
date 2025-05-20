#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#define window_x 800
#define window_y 600
using namespace std;

class Moving {
public:
    double x_speed = 5, y_speed = -5;
    bool SpeedUp = false;
};

class Place {
public:
    double x1_pos, x2_pos, y1_pos, y2_pos;
};

class Figures : public Place {
public:
    void Fill();//заполн€ем объект
    void Create();//создаем его
};

class Platform : public Figures {
public:
    sf::RectangleShape platform;
    void Create(int);
};

class Brick : public Figures, public Moving {
public:
    sf::RectangleShape brick;
    int brick_type/* 9 = стенка, 10 = ускор€ющий, остальные обычные */, brick_life;
    void ColorConverter(); //цвет кирпичика в зависимости от кол-ва жизней
    void Fill(int number);
};

class Ball : public Figures, public Moving {
public:
    int Life = 3;
    bool bottomActive = false;
    sf::CircleShape ball;
    void Create();
    void Update();
};

class Bonuses : public Figures, public Moving {
public:
    static const float fallSpeed;//скорость бонуса 
    sf::CircleShape Bonus;
    int type;
    void Update();
    void Insert(const Brick& _brick);//создаем бонус на месте кирпичика
    void ApplyBonus(Platform&, Ball&);//эффект при касании платформы
    void ColorConvert();//цвет в зависимости от действи€€
};

bool CheckCollision(Figures& A, Figures& B);
void HandleBonusCollisions(vector<Bonuses>& _bonus, Platform& _plat, Ball& _ball);
void HandleBallPlatformCollision(Ball& _ball, Platform _plat);
void HandleBallBrickCollisions(Ball& _ball, vector<Brick>& _bricks);
void DrawBottom(sf::RenderWindow&);