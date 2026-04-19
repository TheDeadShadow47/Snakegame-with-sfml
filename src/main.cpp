#include <SFML/Graphics.hpp>
#include <optional>
#include <ctime>
int N = 30, M = 20;
int size = 16;
int dir = 2, num = 4; // Start moving right
struct Snake { int x, y; } s[100];
struct Fruit { int x, y; } f;
void Tick() {
    // Move body
    for (int i = num; i > 0; --i) {
        s[i].x = s[i - 1].x; 
        s[i].y = s[i - 1].y;
    }

    // Move head
    if (dir == 0) s[0].y += 1; // Down
    if (dir == 1) s[0].x -= 1; // Left
    if (dir == 2) s[0].x += 1; // Right
    if (dir == 3) s[0].y -= 1; // Up

    // Fruit collision
    if ((s[0].x == f.x) && (s[0].y == f.y)) {
        num++; 
        f.x = rand() % N; 
        f.y = rand() % M;
    }

    // Boundary wrapping
    if (s[0].x >= N) s[0].x = 0;  if (s[0].x < 0) s[0].x = N - 1;
    if (s[0].y >= M) s[0].y = 0;  if (s[0].y < 0) s[0].y = M - 1;

    // Self-collision
    for (int i = 1; i < num; i++) {
        if (s[0].x == s[i].x && s[0].y == s[i].y) num = i; 
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(N * size), static_cast<unsigned int>(M * size) }), "Snake Game");
    window.setFramerateLimit(60);
    sf::Texture t1, t2;
    if (!t1.loadFromFile("images/white.png") || !t2.loadFromFile("images/red.png")) {
       
    }

    sf::Sprite sprite1(t1); // Grid/Background
    sf::Sprite sprite2(t2); // Snake/Fruit

    sf::Clock clock;
    float timer = 0, delay = 0.1f;

    f.x = 10; f.y = 10;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        // Input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && dir != 2)  dir = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && dir != 1) dir = 2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && dir != 0)    dir = 3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && dir != 3)  dir = 0;

        // Game Logic Timer
        if (timer > delay) {
            timer = 0;
            Tick();
        }
        // render
        window.clear();
        // Draw Background Grid
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                sprite1.setPosition({ static_cast<float>(i * size), static_cast<float>(j * size) });
                window.draw(sprite1);
            }
        }
        // Draw the snake
        for (int i = 0; i < num; i++) {
           sprite2.setPosition({ static_cast<float>(s[i].x * size), static_cast<float>(s[i].y * size) });
            window.draw(sprite2);
        }
        // Draw the fruit 
        sprite2.setPosition({ static_cast<float>(f.x * size), static_cast<float>(f.y * size) });
        window.draw(sprite2);
        window.display();
    }
    return 0;
}