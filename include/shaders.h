#include "raylib.h"
#include "resources.h"
#include "rlgl.h"

#define SHADERS_AUTO_RELOAD
#define SHADERS_APP_COUNT 16
#define SHADERS_INDEX_SDF 0
#define SHADERS_INDEX_SPRITEGENERIC 1
#define SHADERS_INDEX_SPACESHIP 2

static int shaders_loaded = 0;
static const char* const shaders_filenames[] = {
    SHADERS_PATH "sprite_sdf.fs",
    SHADERS_PATH "sprite_generic.fs",
    SHADERS_PATH "sprite_ship.fs",
};

typedef struct TynShaderOutline {
    Shader shader;
} TynShaderOutline;

typedef struct TynShaderSDF {
    Shader *shader;
    int time_loc;
    int texture1_gradient_loc; 
} TynShaderSDF;

typedef struct TynShaderSpriteGeneric {
    Shader *shader;
    int time_loc;
} TynShaderSpriteGeneric;

typedef struct TynShaderShip {
    Shader *shader;
    int parts_loc;
} TynShaderShip;

typedef struct TynShaders {
    Shader shaders[SHADERS_APP_COUNT];
    int file_mod_times[SHADERS_APP_COUNT];
    TynShaderOutline outline;
    TynShaderSDF sdf;
    TynShaderSpriteGeneric spritegeneric;
    TynShaderShip ship;
} TynShaders;

static void load_outline_shader(TynShaders *tynshaders, Vector2 texsize) {
    TynShaderOutline outline = { 0 };
    outline.shader = LoadShader(0, SHADERS_PATH "outline.fs");
    
    float outlineSize = 2.0f;
    float outlineColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };     // Normalized RED color
    float textureSize[2] = { (float)texsize.x, (float)texsize.y };
    
     int outlineSizeLoc = GetShaderLocation(outline.shader , "outlineSize");
    int outlineColorLoc = GetShaderLocation(outline.shader , "outlineColor");
    int textureSizeLoc = GetShaderLocation(outline.shader , "textureSize");

    // Set shader values (they can be changed later)
    SetShaderValue(outline.shader, outlineSizeLoc, &outlineSize, SHADER_UNIFORM_FLOAT);
    SetShaderValue(outline.shader, outlineColorLoc, outlineColor, SHADER_UNIFORM_VEC4);
    SetShaderValue(outline.shader, textureSizeLoc, textureSize, SHADER_UNIFORM_VEC2);
    
    tynshaders->outline = outline;
}

static Shader load_shader(TynShaders *tynshaders, int index) {
    const char *filename = shaders_filenames[index];
    Shader shader = LoadShader(0,  filename);
     if (shader.id == rlGetShaderIdDefault()) { 
            return shader;
     }
     if (tynshaders->file_mod_times[index] != 0) {
        UnloadShader(tynshaders->shaders[index]);
     }
    tynshaders->shaders[index] = LoadShader(0,  filename);
    tynshaders->file_mod_times[index] = GetFileModTime(filename);
    
    return shader;
}

static void load_sdf_shader(TynShaders *tynshaders, int index) {
    load_shader(tynshaders, index);
    TynShaderSDF sdf = { 0 };
    sdf.shader = &tynshaders->shaders[index];
    sdf.time_loc = GetShaderLocation(*sdf.shader , "time");
     tynshaders->sdf = sdf;
    sdf.texture1_gradient_loc = GetShaderLocation(*sdf.shader , "texture1_gradient");
     tynshaders->sdf = sdf;
}

static void load_spritegeneric_shader(TynShaders *tynshaders, int index) {
    load_shader(tynshaders, index);
    TynShaderSpriteGeneric spritegeneric = { 0 };
    spritegeneric.shader = &tynshaders->shaders[index];
    spritegeneric.time_loc = GetShaderLocation(*spritegeneric.shader , "time");
     tynshaders->spritegeneric = spritegeneric;
}

static void load_ship_shader(TynShaders *tynshaders, int index) {
    load_shader(tynshaders, index);
    TynShaderShip ship = { 0 };
    ship.shader = &tynshaders->shaders[index];
    ship.parts_loc = GetShaderLocation(*ship.shader , "parts");
     tynshaders->ship = ship;
}

void  update_shaders(TynShaders *tynshaders) {
 #ifdef SHADERS_AUTO_RELOAD
     if (
        file_modified(
            shaders_filenames[SHADERS_INDEX_SPRITEGENERIC],
            tynshaders->file_mod_times[SHADERS_INDEX_SPRITEGENERIC]
            )
        ) 
    {
        load_spritegeneric_shader(tynshaders, SHADERS_INDEX_SPRITEGENERIC);
    }
    
    if (file_modified(shaders_filenames[SHADERS_INDEX_SDF],                 
        tynshaders->file_mod_times[SHADERS_INDEX_SDF])) 
    {
        load_sdf_shader(tynshaders, SHADERS_INDEX_SDF);
    }

    if (file_modified(shaders_filenames[SHADERS_INDEX_SPACESHIP],                 
        tynshaders->file_mod_times[SHADERS_INDEX_SPACESHIP])) 
    {
        load_ship_shader(tynshaders, SHADERS_INDEX_SPACESHIP);
    }
 #endif
 
    float time = (float)GetTime();
    SetShaderValue(*tynshaders->sdf.shader, tynshaders->sdf.time_loc, &time, SHADER_UNIFORM_FLOAT);
        SetShaderValue(*tynshaders->spritegeneric.shader, tynshaders->spritegeneric.time_loc, &time, SHADER_UNIFORM_FLOAT);
}

TynShaders *load_shaders(TynShaders *tynshaders) {
    shaders_loaded = 0;
    for (int i = 0; i < SHADERS_APP_COUNT; i++) {
        tynshaders->file_mod_times[i] = 0;
    }
    load_sdf_shader(tynshaders, SHADERS_INDEX_SDF);
    load_ship_shader(tynshaders, SHADERS_INDEX_SPACESHIP);
    load_spritegeneric_shader(tynshaders, SHADERS_INDEX_SPRITEGENERIC);
    shaders_loaded = 3;
     
    return tynshaders;
}
