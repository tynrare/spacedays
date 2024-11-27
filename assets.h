#include "raylib.h"
#include "shaders.h"

#define TEXTURES_AUTO_RELOAD

#ifndef ASSETS_H
#define ASSETS_H

#define TEXTURES_APP_COUNT 1

static const char* const assets_filenames[] = {
    TEXTURES_PATH "shipstest0.png"
};
static int textures_loaded = 0;

typedef struct TynAssets {
        Texture textures[TEXTURES_APP_COUNT];
        int file_mod_times[TEXTURES_APP_COUNT];
        TynShaders shaders;
} TynAssets;

static void load_texture(TynAssets *tynassets, int index) {
    if (tynassets->file_mod_times[index] != 0) {
         UnloadTexture(tynassets->textures[index]);
    }        
    tynassets->textures[index] = LoadTexture(assets_filenames[index]);
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
 
    update_shaders(&tynassets->shaders);
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