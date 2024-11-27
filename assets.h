#include "raylib.h"
#include "shaders.h"

#define TEXTURES_APP_COUNT 16

#ifndef ASSETS_H
#define ASSETS_H
typedef struct TynAssets {
        Texture textures[TEXTURES_APP_COUNT];
        TynShaders shaders;
} TynAssets;

TynAssets *load_assets(TynAssets *tynassets) {
      tynassets->textures[0] = LoadTexture(TEXTURES_PATH "shipstest0.png");
      load_shaders(&tynassets->shaders);
      return tynassets;
}

#endif