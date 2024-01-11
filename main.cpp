#include "Bullet.h"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>
#include <thread>

#define PI 3.14159265358979323846
#define WINW 1366
#define WINH 768
#define MAXBULLETS 3000
#define RADIUS 20
#define MAXFREQ 101

void addBullet(sf::VertexArray *v, Bullet *bul, size_t index) {
  // Get the quad contained in the vertex array
  sf::Vertex *quad = &(*v)[index * 4];

  sf::Sprite *spr = bul->getSprite();

  quad[0].position = sf::Vector2f(spr->getPosition().x, spr->getPosition().y);
  quad[1].position = sf::Vector2f(
      spr->getPosition().x + spr->getTextureRect().width, spr->getPosition().y);
  quad[2].position =
      sf::Vector2f(spr->getPosition().x + spr->getTextureRect().width,
                   spr->getPosition().y + spr->getTextureRect().height);
  quad[3].position =
      sf::Vector2f(spr->getPosition().x,
                   spr->getPosition().y + spr->getTextureRect().height);

  quad[0].texCoords =
      sf::Vector2f(spr->getTextureRect().left, spr->getTextureRect().top);
  quad[1].texCoords =
      sf::Vector2f(spr->getTextureRect().left + spr->getTextureRect().width,
                   spr->getTextureRect().top);
  quad[2].texCoords =
      sf::Vector2f(spr->getTextureRect().left + spr->getTextureRect().width,
                   spr->getTextureRect().top + spr->getTextureRect().height);
  quad[3].texCoords =
      sf::Vector2f(spr->getTextureRect().left,
                   spr->getTextureRect().top + spr->getTextureRect().height);
}

void cleanBulletsOffWindow(size_t totalBullets, Bullet *bullets[]) {
  // CLEAN BULLETS IF THEY LEAVE SCREEN AREA
  for (size_t i = 0; i < totalBullets; i++) {
    if (bullets[i] != NULL) {
      if (bullets[i]->getSprite()->getPosition().x > WINW ||
          bullets[i]->getSprite()->getPosition().y > WINH ||
          (bullets[i]->getSprite()->getPosition().y +
           bullets[i]->getSprite()->getTextureRect().height) < 0 ||
          (bullets[i]->getSprite()->getPosition().x +
           bullets[i]->getSprite()->getTextureRect().width) < 0) {
        delete (bullets[i]);
        bullets[i] = NULL;
      }
    }
  }
}

void SpreadPattern(size_t frequency, size_t counterTime,
                   sf::VertexArray *vertices, Bullet *bullets[],
                   sf::Texture *tex, sf::IntRect sprite) {
  // Spread pattern
  if (counterTime % (MAXFREQ - frequency) == 0) {
    Bullet *spr1 = new Bullet(tex, sf::Vector2f(1.0, 1.0),
                              sf::Vector2f(WINW / 2.0, WINH / 2.0), sprite);

    bullets[1] = spr1;
    addBullet(vertices, spr1, 1);

    Bullet *spr2 = new Bullet(tex, sf::Vector2f(0.0, 1.0),
                              sf::Vector2f(WINW / 2.0, WINH / 2.0), sprite);

    bullets[2] = spr2;
    addBullet(vertices, spr2, 2);

    Bullet *spr3 = new Bullet(tex, sf::Vector2f(-1.0, 1.0),
                              sf::Vector2f(WINW / 2.0, WINH / 2.0), sprite);

    bullets[3] = spr3;
    addBullet(vertices, spr3, 3);
  }
}

int main() {
  sf::RenderWindow window(sf::VideoMode(WINW, WINH), "Bullet Patter Demo");
  window.setFramerateLimit(60);

  // Create an array to hold the bullets
  const size_t totalBullets = 16 * MAXBULLETS;
  const sf::Vector2f centerOfScreen =
      sf::Vector2f(WINW / 2.0 - 8, WINH / 2.0 - 8);

  // Create variables for the bullets patterns
  size_t counterTime = 0;
  size_t numBullet = 0;

  Bullet *bullets[totalBullets];
  for (size_t i = 0; i < totalBullets; i++) {
    bullets[i] = NULL;
  }

  // Use a vertex array for the rendering of the bullets
  sf::VertexArray vertices(sf::Quads, 4 * totalBullets);

  // Load the texture of the bullets
  sf::Texture tex;
  tex.loadFromFile("data/sprites.png");

  // Different textures
  const sf::IntRect redSprite = sf::IntRect(0, 64, 16, 16);
  const sf::IntRect greenSprite = sf::IntRect(16, 64, 16, 16);
  const sf::IntRect aquaSprite = sf::IntRect(32, 64, 16, 16);
  const sf::IntRect pinkSprite = sf::IntRect(48, 64, 16, 16);
  const sf::IntRect orangeSprite = sf::IntRect(64, 64, 16, 16);

  sf::Clock clock;
  while (window.isOpen()) {
    sf::Event event;
    sf::Time timeElapsed = clock.restart();

    // GET KEYPRESSES AND OTHER EVENTS
    while (window.pollEvent(event)) {
      // Exit when clicking 'x' on window
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      // Exit when pressing 'Escape'
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
          window.close();
        }
      }
    }

    // Update bullets
    for (size_t i = 0; i < totalBullets; i++) {
      if (bullets[i] != NULL) {
        bullets[i]->update(timeElapsed.asMilliseconds());
        sf::Vertex *quad = &vertices[i * 4];
        sf::Sprite *spr = bullets[i]->getSprite();

        quad[0].position =
            sf::Vector2f(spr->getPosition().x, spr->getPosition().y);
        quad[1].position =
            sf::Vector2f(spr->getPosition().x + spr->getTextureRect().width,
                         spr->getPosition().y);
        quad[2].position =
            sf::Vector2f(spr->getPosition().x + spr->getTextureRect().width,

                         spr->getPosition().y + spr->getTextureRect().height);
        quad[3].position =
            sf::Vector2f(spr->getPosition().x,
                         spr->getPosition().y + spr->getTextureRect().height);
      }
    }

    if (numBullet == totalBullets) {
      numBullet = 0;
    }

    // Delete previous bullet

    /* if (bullets[numBullet] != NULL) {
      delete (bullets[numBullet]);
      bullets[numBullet] = NULL;
    } */

    // Speed
    const size_t frequency = 80;
    const size_t degree = 90;

    // SpreadPattern(frequency, counterTime, &vertices, bullets, &tex,
    // redSprite);

    if (counterTime % (MAXFREQ - frequency) == 0) {
      Bullet *spr1 =
        new Bullet(&tex, sf::Vector2f(1.0e-1, 1.0e-1),
            sf::Vector2f(WINW / 2.0, WINH / 2.0), redSprite);

      bullets[0+numBullet*3] = spr1;
      addBullet(&vertices, spr1, 0+numBullet*3);

      Bullet *spr2 =
        new Bullet(&tex, sf::Vector2f(0.0, 1.0e-1),
            sf::Vector2f(WINW / 2.0, WINH / 2.0), redSprite);

      bullets[1+numBullet*3] = spr2;
      addBullet(&vertices, spr2, 1+numBullet*3);

      Bullet *spr3 =
        new Bullet(&tex, sf::Vector2f(-1.0e-1, 1.0e-1),
            sf::Vector2f(WINW / 2.0, WINH / 2.0), redSprite);

      bullets[2+numBullet*3] = spr3;
      addBullet(&vertices, spr3, 2+numBullet*3);
    }

    /* float x_ini = centerOfScreen.x;
    float y_ini = centerOfScreen.y;

    if (counterTime % (MAXFREQ - frequency) == 0) {
      Bullet *redBullet = new Bullet(&tex, sf::Vector2f(x_vel, y_vel),
                                     sf::Vector2f(x_ini, y_ini), redSprite);

      bullets[numBullet] = redBullet;
      addBullet(&vertices, redBullet, numBullet);
    } */

    counterTime++;
    numBullet++;

    cleanBulletsOffWindow(totalBullets, bullets);

    window.clear();

    sf::Transform transform;

    sf::RenderStates states;
    states.transform = transform;
    states.texture = &tex;

    window.draw(vertices, states);

    window.display();

    counterTime = counterTime + timeElapsed.asSeconds();
  }

  // Clean bullets
  for (size_t i = 0; i < totalBullets; i++) {
    if (bullets[i] != NULL) {
      delete (bullets[i]);
    }
  }

  return 0;
}
