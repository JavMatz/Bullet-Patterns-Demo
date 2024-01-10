#include "Bullet.h"
#include "SFML/Graphics/Vertex.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

#define PI 3.14159265358979323846
#define WINW 1366
#define WINH 768
#define MAXBULLETS 3000

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

void RedBulletPattern(sf::Texture tex, Bullet *bullets[], size_t numBullet,
                      sf::VertexArray *vertices) {
  float x_vel = cos((size_t)(numBullet % 180) / PI) * 0.15;
  float y_vel = sin((size_t)(numBullet % 180) / PI) * 0.15;
  float x_ini = WINW / 2.0 - 8;
  float y_ini = WINH / 2.0 - 8;

  Bullet *bul =
      new Bullet(&tex, sf::Vector2f(x_vel, y_vel), sf::Vector2f(x_ini, y_ini),
                 sf::IntRect(0, 64, 16, 16));

  bullets[numBullet] = bul;
  addBullet(vertices, bul, numBullet);
}

void GreenBulletPattern(sf::Texture tex, Bullet *bullets[], size_t numBullet,
                        sf::VertexArray *vertices) {
  float x_ini = WINW / 2.0 - 32;
  float y_ini = WINH / 2.0 - 32;

  for (size_t i = 0; i < 45; i++) {
    float y_vel = 20 * cos((size_t)numBullet / (PI / 2));
    float x_vel = 20 * sin((size_t)numBullet / (PI / 2));
    Bullet *bul = new Bullet(&tex, sf::Vector2f(x_vel * 0.01, y_vel * 0.01),
                             sf::Vector2f(WINW / 2 - 32, WINH / 2 - 32),
                             sf::IntRect(16, 64, 16, 16));

    if (bullets[MAXBULLETS + numBullet] != NULL) {
      delete (bullets[MAXBULLETS + numBullet]);
      bullets[MAXBULLETS + numBullet] = NULL;
    }

    bullets[MAXBULLETS + numBullet] = bul;
    addBullet(vertices, bul, MAXBULLETS + numBullet);
  }
}

int main() {
  sf::RenderWindow window(sf::VideoMode(WINW, WINH), "Bullet Patter Demo");

  // Create an array to hold the bullets
  const size_t totalBullets = 16 * MAXBULLETS;

  const double radius = 20;

  // Create variables for the bullets patterns
  double counterTime = 0;
  double counterTime2 = 0;
  double counterTime3 = 0;

  double counterWave2 = 0;
  double counterWave3 = 0;

  double bulletTime2 = 40;
  double bulletTime3 = 100;

  size_t numBullet = 0;
  size_t numBullet2 = 0;
  size_t numBullet3 = 0;

  Bullet *bullets[totalBullets];
  for (size_t i = 0; i < totalBullets; i++) {
    bullets[i] = NULL;
  }

  // Use a vertex array for the rendering of the bullets
  sf::VertexArray vertices(sf::Quads, 4 * totalBullets);

  // Load the texture of the bullets
  sf::Texture tex;
  tex.loadFromFile("data/sprites.png");

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

    // RED BULLETS
    if (numBullet == MAXBULLETS) {
      numBullet = 0;
    }

    if (bullets[numBullet] != NULL) {
      delete (bullets[numBullet]);
      bullets[numBullet] = NULL;
    }

    // RedBulletPattern(tex, bullets, numBullet, &vertices);
    GreenBulletPattern(tex, bullets, numBullet, &vertices);

    // addBullet(&vertices, bul, numBullet);
    numBullet++;

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

    window.clear();

    sf::Transform transform;

    sf::RenderStates states;
    states.transform = transform;
    states.texture = &tex;

    window.draw(vertices, states);

    window.display();

    counterTime = counterTime + timeElapsed.asSeconds();
  }

  for (size_t i = 0; i < totalBullets; i++) {
    if (bullets[i] != NULL) {
      delete (bullets[i]);
    }
  }

  return 0;
}
