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

#include "include/tynspacedays.h"
#include "include/demo_heightmap.h"
#include "include/assets.h"

#define WATTS 0.3
#define FREC 0.1
#define WAVE 0.7

#define TITLE "Tynspace days. wit"

#define PRINT_VALUE(value) printf("Value of %s is %d", #value, value);


typedef struct TynsdApp {
    int a;
    int b;
    int d;
    int r;
    int c;
    TynspaceDaysState *tsds;
    TynAssets assets;
    
    RenderTexture2D render_tex0;
    RenderTexture2D render_tex1;
} TynsdApp;

TynsdApp *tsda = { 0 };


void tsd_state_step(TynspaceDaysState *tsd_state) {
    float watts = WATTS;
    float frec = FREC;
    Vector2 mp = getmp();
    Vector2 bp = { 0 };
    int st = 0;
    const Color stcolors[] = {RED, WHITE, BLUE};
    
    
    
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        st = 1;
        watts *= 1.2;
    }
     if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        st = -1;
        watts *= 1.2;
    }
    
    DrawCircle(mp.x, mp.y, 8, stcolors[st + 1]);
    
    draw_text_ru(RSCANNER, 18, 18 + rufont_size * 0, st == -1 ? BLUE : WHITE);
    draw_text_ru("hold. ", 18, 18 + rufont_size * 1, st ==  0 ? RED  : WHITE);
    draw_text_ru(RCOMPAS , 18, 18 + rufont_size * 2, st ==  1 ? RED  : WHITE);

    for (TynPoolCell *p = tsd_state->bpool->active; p; p = p->next) {
        float *x = p->point;
        float *y = x + 1;
        bp.x = *x;
        bp.y = *y;
        Vector2 delta = Vector2Subtract(mp, bp);
        Vector2 dn = Vector2Normalize(delta);
        float distance = Vector2Length(delta);
        float ldist = logf(distance);
        float wld = powf(2, watts);
        Vector2 sdn = Vector2Scale(dn, st * ldist * wld);
        
        const float dx = ((float)GetRandomValue(-1, 1) + sdn.x) * watts;
        const float dy = ((float)GetRandomValue(-1, 1) + sdn.y) * watts;
        *x += dx;
        *y += dy;
        
        float angle = Vector2Angle(vup, (Vector2) { dx, dy }) - 0;
        
        /*
       const Texture tex = tsda->render_tex1.texture;

        DrawTexturePro(
            tex, 
            (Rectangle){ 0, 0, tex.width, -tex.height }, 
            (Rectangle){ *x, *y, tex.width, tex.height }, 
            (Vector2) { 128, 128 }, 
            angle * RAD2DEG, WHITE);
            */
        DrawRectangle(*x, *y, 2, 2, RED);
    }

}

void init() {
    tsda = malloc(sizeof(TynsdApp));
    tsda->c = 0;
    tsda->a = 1;
    tsda->b = 0;
    tsda->d = 0;
    tsda->r = 0;
}

int test_ship_parts = 0b1111;

void tynspaceship_step() {
   if (IsKeyPressed(KEY_SPACE)) {
        tsda->c = (tsda->c + 1) % TEXTURES_APP_COUNT;
       }
       
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
}

void tynspaceship_draw() {
      
    BeginTextureMode(tsda->render_tex0);
        ClearBackground(BLANK);
            
        BeginShaderMode(*tsda->assets.shaders.sdf.shader);
    
            const index = (TEXTURES_INDEX_SHIP + tsda->c) % TEXTURES_APP_COUNT;
            DrawTexture(tsda->assets.textures[index], 0, 0, WHITE);
        
        EndShaderMode();
    EndTextureMode();

    
   BeginTextureMode(tsda->render_tex1);
        ClearBackground(BLANK);
        BeginShaderMode(*tsda->assets.shaders.ship.shader);

        const Texture tex = tsda->render_tex0.texture;
        //const angle = GetTime() * 16;
        const angle = 0.0f;
        DrawTexturePro(
            tex, 
            (Rectangle){ 0, 0, tex.width, -tex.height }, 
            (Rectangle){ 128, 128, tex.width * 0.25, tex.height }, 
            (Vector2) { 128, 128 }, 
            angle, WHITE);
        EndShaderMode();
    EndTextureMode();
}

void draw() {
    tsd_state_step(tsda->tsds);
  
    
    
    /*
    BeginMode3D(demo_heightmap_state.camera);
        DrawModel(demo_heightmap_state.model, demo_heightmap_state.position, 1.0f, R/
    EndMode3D();
    */
}

void step() {
    tsda->b += 1;
    // Update

    
        tynspaceship_step();
        
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
    tsda->render_tex1 = LoadRenderTexture(256, 256);
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
