#include "Header.h"
sf::RenderWindow window(sf::VideoMode(800, 600), "Arkanoid");

const float Bonuses::fallSpeed = 2.0f;



class Game {
public:
    vector<Brick> Bricks;
    Ball _ball;
    Platform _plat;
    vector<Bonuses> Bonus;

    void Play() {
        window.setFramerateLimit(60);
        Creation();

        while (window.isOpen()) {
            handleEvents();
            Update();
            Drawing();

            if (_ball.Life <= 0) window.close();
        }
    }

private:
    void Creation() {
        _ball.Create();
        _plat.Create(150);
        Bricks.resize(72);

        for (int i = 0; i < 72; i++) {
            Bricks[i].Fill(i);
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        float moveSpeed = 8.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (_plat.x1_pos > 0) {
                _plat.platform.move(-moveSpeed, 0);
                _plat.x1_pos -= moveSpeed;
                _plat.x2_pos -= moveSpeed;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (_plat.x2_pos < window_x) {
                _plat.platform.move(moveSpeed, 0);
                _plat.x1_pos += moveSpeed;
                _plat.x2_pos += moveSpeed;
            }
        }
    }

    void Update() {
        _ball.Update();
        HandleBonusCollisions(Bonus, _plat, _ball);
        HandleBallPlatformCollision(_ball, _plat);
        HandleBallBrickCollisions(_ball, Bricks);

        for (int i = 0; i < Bricks.size(); i++) {
            if (Bricks[i].brick_life <= 0) {
                int random = rand() % 10;
                if (random > 5) {
                    Bonuses temp_bon;
                    temp_bon.Insert(Bricks[i]);
                    Bonus.push_back(temp_bon);
                }
                Bricks.erase(Bricks.begin() + i);
                i--;
            }
        }
    }

    void Drawing() {
        window.clear();

        for (auto& brick : Bricks) window.draw(brick.brick);
        for (auto& bonus : Bonus) {
            window.draw(bonus.Bonus);
            bonus.Update();
        }

        window.draw(_ball.ball);
        window.draw(_plat.platform);
        if (_ball.bottomActive) {
            DrawBottom(window);
        }
        window.display();
    }
};

int main() {
    srand(time(0));
    Game game;
    game.Play();
    return 0;
}