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

#define WATTS 3
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

void locomotion_pull(float *x, float *y, float *dirx, float *diry, Vector2 goal) {
    Vector2 pos = { *x, *y };
    Vector2 dir = { *dirx, *diry };
    Vector2 goaldelta = Vector2Subtract(goal, pos);
    Vector2 ngoaldelta = Vector2Normalize(goaldelta);
    Vector2 ndir = Vector2Normalize(dir);
    float angle = Vector2Angle(ndir, ngoaldelta);
    Vector2 newdir = Vector2Rotate(dir, angle * GetFrameTime());
    *dirx = newdir.x;
    *diry = newdir.y;
}

void locomotion_push(float *x, float *y, float *dirx, float *diry, Vector2 goal) {
    Vector2 pos = { *x, *y };
    Vector2 dir = { *dirx, *diry };
    Vector2 goaldelta = Vector2Subtract(goal, pos);
    Vector2 ngoaldelta = Vector2Negate(Vector2Normalize(goaldelta));
    Vector2 ndir = Vector2Normalize(dir);
    float angle = Vector2Angle(ndir, ngoaldelta);
    Vector2 newdir = Vector2Rotate(dir, angle * GetFrameTime());
    *dirx = newdir.x;
    *diry = newdir.y;
}

#define PAD_LVBT 16

void loc_viewport_bound_teleport(float *x, float *y) {
    if (*x < -PAD_LVBT) {
        *x = viewport_w + PAD_LVBT;
    } else if (*x > viewport_w + PAD_LVBT) {
        *x = - PAD_LVBT;
    }
    
    if (*y < -PAD_LVBT) {
        *y = viewport_h + PAD_LVBT;
    } else if (*y > viewport_h + PAD_LVBT) {
        *y = - PAD_LVBT;
    }
}

void tsd_state_step(TynspaceDaysState *tsd_state) {
    float watts = WATTS;
    float frec = FREC;
    Vector2 mp = getmp();
    Vector2 target = mp;
    if (tsd_state->tyntbox.camera) {
        target = Vector2Subtract(mp, tsd_state->camera.offset);
    }
    
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

    for (TynPoolCell *p = tsd_state->bpool->active; p; p = p->next) {
        float *x = p->point;
        float *y = x + 1;
        float *dx = x + 2;
        float *dy = x + 3;
        
        if (st == 1) {
            locomotion_pull(x, y, dx, dy, target); // pull in
        } else if (st == -1) {
            locomotion_push(x, y, dx, dy, target); // push out
        }
        
        *x += *dx * watts;
        *y += *dy * watts;
        
        loc_viewport_bound_teleport(x, y);
        
        float angle = Vector2Angle(vup, (Vector2) { *dx, *dy }) - 0;
        DrawRectangle(*x, *y, 2, 2, RED);
        DrawLine(*x, *y, *x + *dx * 10, *y + *dy * 10, BLUE);
        

        const Texture tex = tsda->render_tex1.texture;

        DrawTexturePro(
            tex, 
            (Rectangle){ 0, 0, tex.width, -tex.height }, 
            (Rectangle){ *x, *y, tex.width * 0.5, tex.height * 0.5 }, 
            (Vector2) { 64, 64 }, 
            angle * RAD2DEG, WHITE);


    }

    if (tsd_state->tyntbox.camera) {
        EndMode2D();
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
    tynspaceship_draw();
    draw_grid();
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

        ClearBackground(tsda->tsds->tyntbox.greenscreen ? GREEN : WHITE);
        
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
