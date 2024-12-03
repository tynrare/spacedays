#include "raylib.h"
#include "shaders.h"

#define TEXTURES_AUTO_RELOAD

#ifndef ASSETS_H
#define ASSETS_H

#define TEXTURES_APP_COUNT 5

#define TEXTURES_INDEX_SHIP 1
#define TEXTURES_INDEX_GRADIENT 0

static const char* const assets_filenames[] = {
    TEXTURES_PATH "gradient.png",
    TEXTURES_PATH "ship_b_0.png",
    TEXTURES_PATH "ship_b_0_sdf.png",
    TEXTURES_PATH "shipstest0.png",
    TEXTURES_PATH "ship_a_sdf.png",
    TEXTURES_PATH "ship_a_sdf-8b-0n.png",
    TEXTURES_PATH "gradient-32px.png",
    TEXTURES_PATH "gradient-32px-b.png"
    "ship_b_0.png"
};
static int textures_loaded = 0;

typedef struct TynAssets {
        Texture textures[TEXTURES_APP_COUNT];
        int file_mod_times[TEXTURES_APP_COUNT];
        TynShaders shaders;
} TynAssets;

static void load_texture(TynAssets *tynassets, int index) {
    Texture texture = LoadTexture(assets_filenames[index]);
    if (texture.id == 0) {
        return;
    }
    if (tynassets->file_mod_times[index] != 0) {
         UnloadTexture(tynassets->textures[index]);
    }
    tynassets->textures[index] = texture;
    tynassets->file_mod_times[index] = GetFileModTime(assets_filenames[index]);
}

void update_assets(TynAssets *tynassets) {
 #ifdef TEXTURES_AUTO_RELOAD
  for (int i = 0; i < textures_loaded; i++) {
      if (file_modified(assets_filenames[i], tynassets->file_mod_times[i])) {
         load_texture(tynassets, i);
      }
  }
 #endif

    TynShaders *tynshaders = &tynassets->shaders;
    update_shaders(tynshaders);
     
    Texture *texture1_gradient = &tynassets->textures[TEXTURES_INDEX_GRADIENT];
    
    SetShaderValue(*tynshaders->sdf.shader, tynshaders->sdf.texture1_gradient_loc, texture1_gradient, SHADER_UNIFORM_SAMPLER2D);
}

TynAssets *load_assets(TynAssets *tynassets) {
     textures_loaded = 0;
    for (int i = 0; i < TEXTURES_APP_COUNT; i++) {
        tynassets->file_mod_times[i] = 0;
         load_texture(tynassets, i);
        textures_loaded += 1;
    }
    
     load_shaders(&tynassets->shaders);
     return tynassets;
}

#endif