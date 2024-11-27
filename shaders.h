#include "raylib.h"
#include "resources.h"

#define SHADERS_AUTO_RELOAD
#define SHADERS_APP_COUNT 16
#define SHADERS_INDEX_SDF 0

static int shaders_loaded = 0;
static const char* const shaders_filenames[] = {
    SHADERS_PATH "sprite_sdf.fs"
};

typedef struct TynShaderOutline {
    Shader shader;
} TynShaderOutline;

typedef struct TynShaderSDF {
    Shader *shader;
     int time_loc;
} TynShaderSDF;

typedef struct TynShaders {
    Shader shaders[SHADERS_APP_COUNT];
    int file_mod_times[SHADERS_APP_COUNT];
    TynShaderOutline outline;
    TynShaderSDF sdf;
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

static void load_sdf_shader(TynShaders *tynshaders, int index) {
    const char *filename = shaders_filenames[index];
    Shader shader = LoadShader(0,  filename);
     if (shader.id == rlGetShaderIdDefault()) { 
            return;
     }
     if (tynshaders->file_mod_times[index] != 0) {
        UnloadShader(tynshaders->shaders[index]);
     }
    tynshaders->shaders[index] = LoadShader(0,  filename);
    tynshaders->file_mod_times[index] = GetFileModTime(filename);
    
    TynShaderSDF sdf = { 0 };
    sdf.shader = &tynshaders->shaders[index];
    sdf.time_loc = GetShaderLocation(*sdf.shader , "time");
     tynshaders->sdf = sdf;
}

void  update_shaders(TynShaders *tynshaders) {
 #ifdef SHADERS_AUTO_RELOAD
    if (file_modified(shaders_filenames[SHADERS_INDEX_SDF], tynshaders->file_mod_times[SHADERS_INDEX_SDF])) {
        load_sdf_shader(tynshaders, SHADERS_INDEX_SDF);
    }
 #endif
 
    float time = (float)GetTime();
    SetShaderValue(*tynshaders->sdf.shader, tynshaders->sdf.time_loc, &time, SHADER_UNIFORM_FLOAT);
}

TynShaders *load_shaders(TynShaders *tynshaders) {
    shaders_loaded = 0;
    for (int i = 0; i < SHADERS_APP_COUNT; i++) {
        tynshaders->file_mod_times[i] = 0;
    }
    load_sdf_shader(tynshaders, SHADERS_INDEX_SDF);
    shaders_loaded = 1;
     
    return tynshaders;
}