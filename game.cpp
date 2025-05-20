#include "Header.h"

void Platform::Create(int i) {
    platform.setSize(sf::Vector2f(i, 12));
    platform.setPosition(window_x / 2 - i / 2, window_y - 50);
    x1_pos = window_x / 2 - i / 2;
    x2_pos = x1_pos + i;
    y1_pos = window_y - 50;
    y2_pos = y1_pos + 12;
    platform.setFillColor(sf::Color::White);
}

void Brick::ColorConverter() {
    if (brick_type == 9) {
        brick.setFillColor(sf::Color(128, 128, 128));
    }
    else if (brick_type == 10) {
        brick.setFillColor(sf::Color::Blue);
    }
    else {
        if (brick_life == 3) {
            brick.setFillColor(sf::Color::Green);
        }
        else if (brick_life == 2) {
            brick.setFillColor(sf::Color::Yellow);
        }
        else {
            brick.setFillColor(sf::Color::Red);
        }
    }
}

void Brick::Fill(int number) {
    int i = number % 12;
    int j = number / 12;
    brick_type = (j == 0 && i % 2 == 0) ? 9 : (rand() % 11);
    brick_life = (brick_type == 9 || brick_type == 10) ? 1 : 3;

    brick.setSize(sf::Vector2f(60, 20));
    brick.setPosition(i * 60 + 20, j * 20 + 40);
    x1_pos = i * 60 + 20;
    x2_pos = x1_pos + 60;
    y1_pos = j * 20 + 40;
    y2_pos = y1_pos + 20;
    ColorConverter();
    brick.setOutlineThickness(1);
    brick.setOutlineColor(sf::Color::Black);//выделил окантовочку кирпичика
}

void Ball::Create() {
    ball.setRadius(8);
    ball.setPosition(window_x / 2 - 10, window_y / 2 - 10);
    x1_pos = window_x / 2 - 8;
    x2_pos = window_x / 2 + 8;
    y1_pos = window_y / 2 - 8;
    y2_pos = window_y / 2 + 8;
    ball.setFillColor(sf::Color::White);
}

void Ball::Update() {
    ball.move(x_speed, y_speed);
    x1_pos += x_speed;
    x2_pos += x_speed;
    y1_pos += y_speed;
    y2_pos += y_speed;
    if (y2_pos >= window_y) {
        if (bottomActive) {
            y_speed *= -1;
            bottomActive = false;
        }
        else {
            Life--;
            y_speed *= -1;
        }
        return;
    }
    if (y1_pos <= 0) {
        y_speed *= -1;
        return;
    }
    if ((x1_pos <= 0) || (x2_pos >= window_x)) {
        x_speed *= -1;
        return;
    }
}

void ChangeSpeed(Ball& _ball) {
    if (!_ball.SpeedUp) {
        _ball.x_speed *= 1.25;
        _ball.y_speed *= 1.25;
        _ball.SpeedUp = true;
    }
    else {
        _ball.x_speed /= 1.25;
        _ball.y_speed /= 1.25;
        _ball.SpeedUp = false;
    }
}

void ChangeSize(Platform& _plat) {
    if (_plat.platform.getSize().x == 150) {
        _plat.Create(200);
    }
    else {
        _plat.Create(150);
    }
}

void RandomBall(Ball& _ball) {
    double angle = (rand() % 360) * 3.14159 / 180;
    _ball.x_speed = _ball.x_speed * cos(angle);
    _ball.y_speed = _ball.y_speed * sin(angle);
}

void Bonuses::ApplyBonus(Platform& _plat, Ball& _ball) {
    switch (type) {
    case 0: ChangeSize(_plat); break;
    case 1: ChangeSpeed(_ball); break;
    case 2: _ball.Life++; break;
    case 3: RandomBall(_ball); break;
    case 4: _ball.bottomActive = true; break;
    }
}

void Bonuses::ColorConvert() {
    switch (type) {
    case 0: Bonus.setFillColor(sf::Color::Blue); break;
    case 1: Bonus.setFillColor(sf::Color::Yellow); break;
    case 2: Bonus.setFillColor(sf::Color::Magenta); break;
    case 3: Bonus.setFillColor(sf::Color::Cyan); break;
    case 4: Bonus.setFillColor(sf::Color::Green); break;
    }
}

void Bonuses::Insert(const Brick& _brick) {
    type = rand() % 5;
    x1_pos = _brick.x1_pos;
    x2_pos = _brick.x2_pos;
    y1_pos = _brick.y1_pos;
    y2_pos = _brick.y2_pos;
    Bonus.setRadius(8);
    Bonus.setPosition(x1_pos + 30 - 8, y1_pos + 10 - 8);
    ColorConvert();
}

void Bonuses::Update() {
    Bonus.move(0, fallSpeed);
    y1_pos += fallSpeed;
    y2_pos += fallSpeed;
}

bool CheckCollision(Figures& A, Figures& B) {
    return A.x2_pos >= B.x1_pos && A.x1_pos <= B.x2_pos &&
        A.y2_pos >= B.y1_pos && A.y1_pos <= B.y2_pos;
}

void HandleBonusCollisions(vector<Bonuses>& _bonus, Platform& _plat, Ball& _ball) {
    for (int i = 0; i < _bonus.size(); i++) {
        if (CheckCollision(_bonus[i], _plat)) {
            _bonus[i].ApplyBonus(_plat, _ball);
            _bonus.erase(_bonus.begin() + i);
            i--;//компенсируем удаление бонуса
        }
        else if (_bonus[i].y1_pos > window_y) {
            _bonus.erase(_bonus.begin() + i);
            i--;//аналог
        }
    }
}

void HandleBallPlatformCollision(Ball& _ball, Platform _plat) {
    if (!CheckCollision(_ball, _plat)) return;

    double ball_center = _ball.x1_pos + 8;
    double plat_center = _plat.x1_pos + _plat.platform.getSize().x / 2;

    _ball.y_speed *= -1;
    if (ball_center < plat_center) {
        _ball.x_speed = -abs(_ball.x_speed);
    }
    else {
        _ball.x_speed = abs(_ball.x_speed);
    }
}

void HandleBallBrickCollisions(Ball& _ball, vector<Brick>& _bricks) {
    for (int i = 0; i < _bricks.size(); i++) {
        if (CheckCollision(_ball, _bricks[i])) {
            double IntersectsLeft = _ball.x2_pos - _bricks[i].x1_pos;
            double IntersectsRight = _bricks[i].x2_pos - _ball.x1_pos;
            double IntersectsTop = _ball.y2_pos - _bricks[i].y1_pos;
            double IntersectsBottom = _bricks[i].y2_pos - _ball.y1_pos;

            bool DirectionRight = abs(IntersectsLeft) > abs(IntersectsRight);
            bool DirectionTop = abs(IntersectsTop) < abs(IntersectsBottom);

            if (abs(DirectionRight ? IntersectsRight : IntersectsLeft) <
                abs(DirectionTop ? IntersectsTop : IntersectsBottom)) {
                _ball.x_speed = DirectionRight ? abs(_ball.x_speed) : -abs(_ball.x_speed);
            }
            else {
                _ball.y_speed = DirectionTop ? -abs(_ball.y_speed) : abs(_ball.y_speed);
            }

            if (_bricks[i].brick_type == 10) {
                _ball.x_speed *= 1.1;
                _ball.y_speed *= 1.1;
                _bricks[i].brick_life = 0;
            }
            else if (_bricks[i].brick_type != 9) {
                _bricks[i].brick_life--;
                _bricks[i].ColorConverter();
            }
        }
    }
}

void DrawBottom(sf::RenderWindow& window) {
    sf::RectangleShape barrier(sf::Vector2f(window_x, 10));
    barrier.setPosition(0, window_y - 10);
    barrier.setFillColor(sf::Color::Green);
    window.draw(barrier);
}