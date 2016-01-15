#include "enemy.h"
#include <stack>
extern stack<Sprite *> unusedSprites;
extern Texture *textures[TEX_COUNT];
extern Sprite *playerSprite;
extern Sprite *bulletsprite;
extern int playerlives;
static Vector2f GetNewEnemyPos() { return Vector2f((float)(rand() % GAME_WORLD_X), -128.0f); }

Vector2f FlyBehaviors::ZigZag::move(Vector2f currentPos) {

  if (dir) {
    if (currentPos.x > (GAME_WORLD_X - 100)) {
      dir = !dir;
    }
    return Vector2f(1.0f, 1.0f);
  } else {
    if (currentPos.x < 100) {
      dir = !dir;
    }
    return Vector2f(-1.0f, 1.0f);
  }
}

Vector2f FlyBehaviors::Standard::move(Vector2f currentPos) { return Vector2f(0, 1.0f); }
Vector2f FlyBehaviors::Line::move(Vector2f currentPos) { return Vector2f(0, 1.0f); }
Vector2f FlyBehaviors::Homing::move(Vector2f currentPos) { return Vector2f(0, 1.0f); }

void EnemyShip::Update(float deltaSeconds) {
  spr->setPosition(spr->getPosition() + flyB->move(spr->getPosition()) * deltaSeconds * 50.0f);
  if (spr->getPosition().y > GAME_WORLD_Y) {
    alive = false;
  } else if (spr->getGlobalBounds().intersects(playerSprite->getGlobalBounds())) {
    alive = false;
    --playerlives;
  } else if (spr->getGlobalBounds().intersects(bulletsprite->getGlobalBounds())) {
    alive = false;
  }
}

EnemyShip::EnemyShip(float h, float d, float s, FlyBehavior *f, Texture *t) {
  flyB = f;
  speed = s;
  damageOnCollide = d;
  health = h;
  spr = unusedSprites.top();
  unusedSprites.pop();
  spr->setTexture(*t);
  spr->setPosition(GetNewEnemyPos());
  // spr->setColor(Color::Color());
  spr->setRotation(0);
  alive = true;
}

EnemyShip::~EnemyShip() {
  alive = false;
  unusedSprites.push(spr);
  spr = nullptr;
  delete flyB;
  flyB = nullptr;
}

namespace Ships {
Frigate::Frigate() : EnemyShip(1.0f, 10.0f, 50.0f, new FlyBehaviors::ZigZag(), textures[enemy3]) {}
Destroyer::Destroyer()
    : EnemyShip(10.0f, 10.0f, 80.0f, new FlyBehaviors::Standard(), textures[enemy5]) {}
BattleCruiser::BattleCruiser()
    : EnemyShip(20.0f, 10.0f, 50.0f, new FlyBehaviors::Standard(), textures[enemy2]) {}
Cruiser::Cruiser()
    : EnemyShip(50.0f, 10.0f, 60.0f, new FlyBehaviors::Standard(), textures[enemy1]) {}
BattleShip::BattleShip()
    : EnemyShip(100.0f, 10.0f, 40.0f, new FlyBehaviors::Standard(), textures[enemy4]) {}
}