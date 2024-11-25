/*******************************************************************************************
*
*   Welcome into tynspace days!
*
*   Welcome to raylib!
*
********************************************************************************************/

#include "tynspacedays.h"
#include "demo_heightmap.h"

void tsd_state_init(TynspaceDaysState *tsd_state) {
    while (tsd_state->bpool->idle) {
        TynPoolCell *cell = tynpool_cell_alloc(tsd_state->bpool);
        float *x = cell->point;
        float *y = x + 1;
        *x += GetRandomValue(viewport_w * 0.5 - 32, viewport_w * 0.5 + 32);
        *y += GetRandomValue(viewport_h * 0.5 - 32, viewport_h * 0.5 + 32);
    }
}

TynspaceDaysState *tsd_state_allocate() {
    TynspaceDaysState *tsd_state = malloc(sizeof(TynspaceDaysState));
    tsd_state->bpool = typool_allocate(1024, sizeof(float) * 4);
    
    tsd_state_init(tsd_state);
    
    return tsd_state;
}



void tsd_state_step(TynspaceDaysState *tsd_state) {
    float watts = 1;
    float frec = 1;
    Vector2 mp = getmp();
    Vector2 bp = { 0 };
    Color fblue = Fade(BLUE, 1);
    float st = sinf(GetTime() * frec);
    
    DrawText(st > 0 ? "a!" : "b!", 190, 200, 20, RED);
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

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(viewport_w, viewport_h, "tynspace days.");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    
    TynspaceDaysState *tsd_state = tsd_state_allocate();
    //DemoHeightmapState demo_heightmap_state = demo_heightmap_state_init();
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
       
       // ч происходит леш хх
       
        BeginDrawing();

        ClearBackground(GREEN);
            
        tsd_state_step(tsd_state);
        
        /*
        BeginMode3D(demo_heightmap_state.camera);

            DrawModel(demo_heightmap_state.model, demo_heightmap_state.position, 1.0f, R/
        EndMode3D();
        */
      
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
