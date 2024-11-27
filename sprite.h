#include "raylib.h"
#include "raymath.h"
#include "assets.h"

#ifndef SPRITE_H
#define SPRITE_H

typedef struct Sprite {
    int index; // path to position and other possible data.
	unsigned int texture;
	float rotation;
	float scale;
} Sprite;

Sprite SpriteCreate(int index, int texture) {
  Sprite s = {0};
  SpriteInit(&s, index, texture);

  return s;
}

void SpriteInit(Sprite *s, int index, int texture) {
  s->rotation = 0.0;
  s->scale = 1.0;
  s->texture = texture;
  s->index = index;
}

void SpriteDraw(TynAssets *assets, Sprite *sprite, Vector2 *position) {
  const Texture *texture = &assets->textures[sprite->texture];
   
  const float x = texture->width * sprite->scale * 0.5;
  const float y = texture->height * sprite->scale ** 0.5;
  const Vector2 v0 = Vector2Rotate((Vector2){x, y}, sprite->rotation * DEG2RAD);
  const Vector2 v1 =
      (Vector2){position->x - v0.x, position->y - v0.y};

  DrawTextureEx(*texture, v1, sprite->rotation, sprite->scale, WHITE);
}

#endif