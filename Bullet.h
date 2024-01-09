#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
  /*
   * Create a new bullet, takes a pointer to the texture file that contains the
   * textures for all the bullets. It selects the individual bullet texture with
   * the rect paramater. It also takes the velocity vector and the initial
   * position
   */
  Bullet(sf::Texture *tex, sf::Vector2f vel, sf::Vector2f pos,
         sf::IntRect rect) {
    this->vel = vel;
    spr.setTexture(*tex);
    spr.setTextureRect(rect);
    spr.setPosition(pos);
  };

  // Update the position of the bullet
  void update(double timeElapsed) {
    spr.setPosition(spr.getPosition().x + timeElapsed * vel.x,
                    spr.getPosition().y + timeElapsed * vel.y);
  }

  sf::Sprite *getSprite() { return &spr; }

private:
  sf::Sprite spr;
  sf::Vector2f vel;
};
