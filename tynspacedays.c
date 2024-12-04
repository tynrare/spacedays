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


#define TITLE "Tynspace days. wit"

#define PRINT_VALUE(value) printf("Value of %s is %d", #value, value);


typedef struct TynsdApp {
    int a;
    int b;
    int d;
    int r;
    int c;
    float decay;
    TynspaceDaysState *tsds;
    TynAssets assets;
    
    RenderTexture2D render_tex0;
    RenderTexture2D render_tex1;
    RenderTexture2D render_tex_f0;
} TynsdApp;

TynsdApp *tsda = { 0 };

void draw_particle(
    float *x, float*y, 
    float *dx, float *dy,
    float *intensity
    ) {
        DrawRectangle(*x - 2, *y - 2, 4, 4, MAGENTA);
        DrawLine(*x, *y, *x + *dx * 10, *y + *dy * 10, MAGENTA);
        
        const Texture tex = tsda->render_tex1.texture;
        float angle = Vector2Angle(vup, (Vector2) { *dx, *dy }) - 0;
        const float scale = 0.4f;
        DrawTexturePro(
            tex, 
            (Rectangle){ 0, 0, tex.width, -tex.height }, 
            (Rectangle){ *x, *y, tex.width * scale, tex.height * scale }, 
            (Vector2) { 128 * scale, 128 * scale }, 
            angle * RAD2DEG, 
            Fade(WHITE, *intensity));

}

void tsd_state_step(TynspaceDaysState *tsd_state) {
    float watts = WATTS;
    float frec = FREC;
    Vector2 mp = getmp();
    Vector2 target = mp;
    if (tsd_state->tyntbox.camera) {
        target = Vector2Subtract(mp, tsd_state->camera.offset);
    }
    
    int st = 1;
    const Color stcolors[] = {RED, WHITE, BLUE};
    
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        st = 1;
        watts *= 1.2;
    }
     if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        st = -1;
        watts *= 0.4;
    }
    
    draw_text_ru("Подвязать поведения\n движений к предметам\n инвентаря", 18, 18, WHITE);
    draw_text_ru("- Телепортатор краев экрана", 18, 18 + rufont_size * 3, WHITE);
    draw_text_ru("- Разные двигатели", 18, 18 + rufont_size * 4, WHITE);
    draw_text_ru("- Коллизии", 18, 18 + rufont_size * 5, WHITE);
    
    if (tsd_state->tyntbox.camera) {
        BeginMode2D(tsd_state->camera);
    }
    
    draw_text_ru(RSCANNER, 18, 18 + rufont_size * 0, st == -1 ? BLUE : WHITE);
    draw_text_ru("hold. ", 18, 18 + rufont_size * 1, st ==  0 ? RED  : WHITE);
    draw_text_ru(RCOMPAS , 18, 18 + rufont_size * 2, st ==  1 ? RED  : WHITE);
   
    draw_text_ru(
        TextFormat("decay: %.02f", tsda->decay),
        0, 0,
        RED
    );
    
    BeginTextureMode( tsda->render_tex_f0);
    ClearBackground(BLACK);
    //DrawRectangle(2, 2, viewport_w - 4, viewport_h - 4, Fade(BLACK, tsda->decay * GetFrameTime()));
    BeginBlendMode(BLEND_ALPHA);

    TynPoolCell *rand_cp = tsd_state->bpool->active;

    TynChantDzenSun *tchant_dzen_sun = 
        &tsd_state->tyntbox.chantbox.dzen_sun;
     TynPoolCell *aims_p = tchant_dzen_sun->aims->active;
        float *aims_x = aims_p + 0;
        float *aims_y = aims_p + 1;
        *aims_x = mp.x;
        *aims_y = mp.y;

    chant_dzensun_step(tchant_dzen_sun , draw_particle);
    
    EndBlendMode();
    EndTextureMode();
 
    if (tsd_state->tyntbox.camera) {
        EndMode2D();
    }
    
    BeginShaderMode(*tsda->assets.shaders.sdf.shader);
            const Texture tex = tsda->render_tex_f0.texture;
            DrawTexturePro(
            tex, 
            (Rectangle){ 0, 0, tex.width, -tex.height }, 
            (Rectangle){ 0, 0, tex.width, tex.height }, 
            (Vector2) { 0,0 }, 
            0 * RAD2DEG, WHITE);
        
    EndShaderMode();
   
    DrawCircle(mp.x, mp.y, 8, stcolors[st + 1]);
}

void init() {
    tsda = malloc(sizeof(TynsdApp));
    tsda->c = 0;
    tsda->a = 1;
    tsda->b = 0;
    tsda->d = 0;
    tsda->r = 0;
    tsda->decay = 0.24f;
}

int test_ship_parts = 0b0011;

void tynspaceship_step() {
   if (IsKeyPressed(KEY_SPACE)) {
        tsda->c = (tsda->c + 1) % TEXTURES_APP_COUNT;
       }
       
       if (IsKeyDown(KEY_UP)) {
           tsda->decay += 
            dlerp(0.0, 1.0, 0.2, GetFrameTime());
       } else if (IsKeyDown(KEY_DOWN)) {
            tsda->decay -= 
            dlerp(0.0, 1.0, 0.2, GetFrameTime());
            
       }
     tsda->decay = Clamp(tsda->decay, 0.0, 1.0);
        
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

void draw_lint_x(y) {
    DrawLine(0, y, viewport_w, y, RED);    
}

void draw_lint_y(x) {
    DrawLine(x, 0, x, viewport_h, RED);    
}

void draw_grid() {
    draw_lint_x(2);
    draw_lint_x(viewport_h - 2);
    draw_lint_y(2);
    draw_lint_y(viewport_w - 2);
    
    //DrawLine(2, 0, 2, viewport_h, RED);
    //DrawLine(0, 2, viewport_w, 2, RED);
    //DrawLine(viewport_w - 2, 0, viewport_w - 2, viewport_h, RED);
    //DrawLine(0, viewport_h - 2, viewport_w, viewport_h - 2, RED);
}

void tynspaceship_draw() {
      
    BeginTextureMode(tsda->render_tex0);
        ClearBackground(BLANK);
            BeginShaderMode(*tsda->assets.shaders.spritegeneric.shader);
    
            const index = (TEXTURES_INDEX_SHIP + tsda->c) % TEXTURES_APP_COUNT;
            DrawTexture(tsda->assets.textures[index], 0, 0, WHITE);
        
        EndShaderMode();
    EndTextureMode();

    
   BeginTextureMode(tsda->render_tex1);
        ClearBackground(BLANK);
        //BeginShaderMode(*tsda->assets.shaders.ship.shader);

        const Texture tex = tsda->render_tex0.texture;
        //const angle = GetTime() * 16;
        const angle = 0.0f;
        DrawTexturePro(
            tex, 
            (Rectangle){ 0, 0, tex.width, -tex.height }, 
            (Rectangle){ 128, 128, tex.width * 0.25, tex.height }, 
            (Vector2) { 128, 128 }, 
            angle, WHITE);
        //EndShaderMode();
    EndTextureMode();
    
    if (tsda->tsds->tyntbox.texture_preview) {
        DrawTexture(tsda->render_tex1.texture, 2, 2, WHITE);
        DrawTexture(tsda->render_tex0.texture, tsda->render_tex1.texture.width + 2, 2, WHITE);
    }
}

void draw() {
    tynspaceship_draw();
    draw_grid();
    tsd_state_step(tsda->tsds);
  
    
    
    /*
    BeginMode3D(demo_heightmap_state.camera);
        DrawModel(demo_heightmap_state.model, demo_heightmap_state.position, 1.0f, R/
    EndMode3D();
    */
}

void resized(int w, int h) {
    viewport_w = w;
    viewport_h = h;
    UnloadRenderTexture(tsda->render_tex_f0);
     tsda->render_tex_f0  = LoadRenderTexture(viewport_w, viewport_h);
}

void step() {
    tsda->b += 1;
    // Update
    
        tynspaceship_step();
        
       update_assets(&tsda->assets);
       
       const w = GetScreenWidth();
       const h = GetScreenWidth();
       if (w != viewport_w || h != viewport_h) {
           resized(w, h);
       }
       
        // ч происходит леш хх
       
        BeginDrawing();

        //const Color b = { 186, 156, 128, 255 };
        const Color b = RAYWHITE;
        ClearBackground(tsda->tsds->tyntbox.greenscreen ? GREEN  :  b);
        
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
    tsda->render_tex_f0  = LoadRenderTexture(viewport_w, viewport_h);
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
