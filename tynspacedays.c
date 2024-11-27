/*******************************************************************************************
*
*   Welcome into tynspace days!
*   l241125. github.com/tynrare/spacedays
*#  | 
*   l241126 7:55am;
*#  | 
*   l241127 19:58am;
*#  |  - sdf shaders; resources workflow. First ship parts test.
*   Welcome to raylib!
*
*
*#  wit.games
*
********************************************************************************************/

#include "tynspacedays.h"
#include "demo_heightmap.h"
#include "assets.h"

#define WATTS 4.2

#define TYCOMPAS "tyc."
#define TYSCANNER "tys!"

#define TITLE "Tynspace days. wit"

#define PRINT_VALUE(value) printf("Value of %s is %d", #value, value);

void tsd_state_step(TynspaceDaysState *tsd_state) {
    float watts = WATTS;
    float frec = 1;
    Vector2 mp = getmp();
    Vector2 bp = { 0 };
    Color fblue = Fade(BLUE, 1);
    float st = sinf(GetTime() * frec);
    
    draw_text_ru("компас. ", 18, 18, RED);
    DrawTextRu("компас. ", 16, 16);
    
    DrawTextEx(rufont, RSCANNER, (Vector2){ 18, 56 }, rufont_size, 2, st > 0 ? RED : BLUE);
    DrawTextEx(rufont, RSCANNER, (Vector2){ 16, 54 }, rufont_size, 2, st < 0 ? RED : BLUE);

    DrawText(st > 0 ? TYCOMPAS : TYSCANNER, viewport_w * 0.5, viewport_h * 0.5, 60, BLACK);
    for (TynPoolCell *p = tsd_state->bpool->active; p; p = p->next) {
        float *x = p->point;
        float *y = x + 1;
        bp.x = *x;
        bp.y = *y;
        Vector2 delta = Vector2Subtract(mp, bp);
        Vector2 dn = Vector2Normalize(delta);
        //float distance = Vector2Length(delta);
        Vector2 sdn = Vector2Scale(dn, st);
        *x += GetRandomValue(-watts + sdn.x, watts + sdn.x);
        *y += GetRandomValue(-watts + sdn.y, watts + sdn.y);
        DrawRectangle(*x, *y, 16, 16, fblue);
    }
}

typedef struct TynsdApp {
    int a;
    int b;
    int d;
    int r;
    TynspaceDaysState *tsds;
    TynAssets assets;
    
    RenderTexture2D render_tex0;
} TynsdApp;

TynsdApp *tsda = { 0 };

void init() {
    tsda = malloc(sizeof(TynsdApp));
    tsda->a = 1;
    tsda->b = 0;
    tsda->d = 0;
    tsda->r = 0;
}

int test_ship_parts = 0b1111;

void draw() {
    tsd_state_step(tsda->tsds);
    
    BeginTextureMode(tsda->render_tex0);
        ClearBackground(BLANK);
        BeginShaderMode(*tsda->assets.shaders.sdf.shader);
    
            DrawTexture(tsda->assets.textures[1], 0, 0, WHITE);
        
        EndShaderMode();
    EndTextureMode();

    if (IsKeyPressed(KEY_ONE)) {
        test_ship_parts ^= 0b0001;
    }
    if (IsKeyPressed(KEY_TWO)) {
        test_ship_parts ^= 0b0010;
    }
    if (IsKeyPressed(KEY_THREE)) {
        test_ship_parts ^= 0b0100;
    }
    if (IsKeyPressed(KEY_FOUR)) {
        test_ship_parts ^= 0b1000;
    }

    SetShaderValue(*tsda->assets.shaders.ship.shader, tsda->assets.shaders.ship.parts_loc, &test_ship_parts,
    SHADER_UNIFORM_INT);
    BeginShaderMode(*tsda->assets.shaders.ship.shader);

        const Texture tex = tsda->render_tex0.texture;

        DrawTexturePro(
            tex, 
            (Rectangle){ 0, 0, tex.width, -tex.height }, 
            (Rectangle){ 128, 128, tex.width * 0.25, tex.height }, 
            (Vector2) { 128, 128 }, 
            GetTime() * 16, WHITE);
    EndShaderMode();
    
    
    /*
    BeginMode3D(demo_heightmap_state.camera);
        DrawModel(demo_heightmap_state.model, demo_heightmap_state.position, 1.0f, R/
    EndMode3D();
    */
}

void step() {
        tsda->b += 1;
        // Update
        
       update_assets(&tsda->assets);
       
       viewport_w = GetScreenWidth();
       viewport_h = GetScreenHeight();
        // ч происходит леш хх
       
        BeginDrawing();

        ClearBackground(GREEN);
        
        draw();
        
        EndDrawing();
        //----------------------------------------------------------------------------------
}

bool loop() {
    tsda->a = 3;
    
    if (WindowShouldClose()) {
        tsda->a -= 2;
    }
    
    // Main game loop
    step();
    
    // Detect window close button or ESC key
    return tsda->a > 2;
}

void load() {
    load_assets(&tsda->assets);
    tsda->render_tex0 = LoadRenderTexture(1024, 256);
}

void run() {
    tsda->b = 0;
    
    InitWindow(viewport_w, viewport_h, TITLE);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    
    tsda->b = 1;
    load_rutopter();

    tsda->b = 2;
    TynspaceDaysState *tsd_state = tsd_state_init();
    tsda->tsds = tsd_state;
    
    tsda->b = 3;
    tsd_state_run(tsd_state);
    
    tsda->b = 4;
    load();
    
    tsda->b = 5;
    
    tsda->a = 2;
    while(tsda->a > 1 && loop()) {};
}

void stop() {
    tsda->a = 1;
    tsda->d = 1;
    tsd_state_stop(tsda->tsds);
    tsda->d = 2;
    tsd_state_dispose(tsda->tsds);
    tsda->d = 3;
    
    CloseWindow();
    
     tsda->d = 4;
}

void dispose() {
    tsda->a = 1;
    tsda->b = 0;
    tsda->d = 0;
    free(tsda);
}


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    
    init();
    run();
    
    //DemoHeightmapState demo_heightmap_state = demo_heightmap_state_init();
    
    

    // De-Initialization
    //--------------------------------------------------------------------------------------
            // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
