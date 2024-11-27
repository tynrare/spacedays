#include "raylib.h"
#include "raymath.h"

#ifndef SPRITE_H
#define SPRITE_H

typedef struct Sprite {
	Vector2 position;
	Vector2 anchor;
	int texture;
	float rotation;
	float scale;
	Color color;
} Sprite;

Sprite SpriteLoad(const char* fileName);
Sprite SpriteCreate(Texture2D texture);
void SpriteInit(Sprite *s, Texture2D texture);
void SpriteDraw(Sprite* sprite);

Sprite SpriteLoad(const char *fileName) {
  return SpriteCreate(LoadTexture(fileName));
}

Sprite SpriteCreate(Texture2D texture) {
  Sprite s = {0};
  SpriteInit(&s, texture);

  return s;
}

void SpriteInit(Sprite *s, Texture2D texture) {
  s->position = (Vector2){0.0, 0.0};
  s->anchor = (Vector2){0.5, 0.5};
  s->rotation = 0.0;
  s->scale = 1.0;
  s->texture = texture;
	s->color = WHITE;
}

void SpriteDraw(Sprite *sprite) {
  const float x = sprite->texture.width * sprite->scale * sprite->anchor.x;
  const float y = sprite->texture.height * sprite->scale * sprite->anchor.y;
  const Vector2 v0 = Vector2Rotate((Vector2){x, y}, sprite->rotation * DEG2RAD);
  const Vector2 v1 =
      (Vector2){sprite->position.x - v0.x, sprite->position.y - v0.y};

  DrawTextureEx(sprite->texture, v1, sprite->rotation, sprite->scale, sprite->color);
}

#endif