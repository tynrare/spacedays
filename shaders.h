#include "raylib.h"
#include "resources.h"

typedef struct TynShaderOutline {
    Shader shader;
} TynShaderOutline;

typedef struct TynShaderSDF {
    Shader shader;
     int time_loc;
} TynShaderSDF;

typedef struct TynShaders {
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

static void load_sdf_shader(TynShaders *tynshaders) {
    TynShaderSDF sdf = { 0 };
    sdf.shader = LoadShader(0,  SHADERS_PATH "sprite_sdf.fs");
    
     sdf.time_loc = GetShaderLocation(sdf.shader , "time");
    
    tynshaders->sdf = sdf;
}

void  update_shaders(TynShaders *tynshaders) {
    float time = (float)GetTime();
    SetShaderValue(tynshaders->sdf.shader, tynshaders->sdf.time_loc, &time, SHADER_UNIFORM_FLOAT);
}

TynShaders *load_shaders(TynShaders *tynshaders) {
    load_sdf_shader(tynshaders);
    return tynshaders;
}