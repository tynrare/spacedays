#include "index.h"
#include "resources.h"

#ifndef TYNSPACEDAYS_H
#define TYNSPACEDAYS_H
#define bood bool

#define SPACE_IBOUNDS 2048

typedef struct TunspaceToolbox {
    bood hold;
    bool compas;
    bool scaner;
    bool camera;
    bool greenscreen;
    bool texture_preview;
} TunspaceToolbox;

typedef struct TynspaceDaysState {
    struct TynPool *bpool;
    struct TynPool *rpool;
    Camera2D camera;
    TunspaceToolbox tyntbox;
} TynspaceDaysState;

void tsd_state_stop(TynspaceDaysState *tsd_state) {
    while (tsd_state->bpool->active) {
        TynPoolCell *cell = tsd_state->bpool->active;
        float *x = cell->point;
        float *y = x + 1;
        float *dx = x + 2;
        float *dy = x + 3;
        *x = 0;
        *y = 0;
        *dx = 0.0f;
        *dy = 0.0f;
        tynpool_cell_free(tsd_state->bpool, cell);
    }
    
    while (tsd_state->rpool->active) {
        TynPoolCell *cell = tsd_state->rpool->active;
        unsigned short int *r = cell->point;
        unsigned short int *g = r + 1;
        unsigned short int *b = r + 2;
        unsigned short int *a = r + 3;
        *r = 0;
        *g = 0;
        *b = 0;
        *a = 0;
        tynpool_cell_free(tsd_state->rpool, cell);
    }
}

void tsd_state_run(TynspaceDaysState *tsd_state) {
    while (tsd_state->bpool->idle) {
        TynPoolCell *cell = tynpool_cell_alloc(tsd_state->bpool);
        float *x = cell->point;
        float *y = x + 1;
        float *dx = x + 2;
        float *dy = x + 3;
        *x += GetRandomValue(-SPACE_IBOUNDS, SPACE_IBOUNDS);
        *y += GetRandomValue(-SPACE_IBOUNDS, SPACE_IBOUNDS);
        *dx = 0.0f;
        *dy = 1.0f;
    }
    
    while (tsd_state->rpool->idle) {
        TynPoolCell *cell = tynpool_cell_alloc(tsd_state->rpool);
        float *r = cell->point;
        float *g = r + 1;
        float *b = r + 2;
        float *a = r + 3;
        *r = (float)GetRandomValue(0, 255) / 255.0f;
        *g = (float)GetRandomValue(0, 255) / 255.0f;
        *b = (float)GetRandomValue(0, 255) / 255.0f;
        *a = (float)GetRandomValue(0, 255) / 255.0f;
    }
}

TynspaceDaysState *tsd_state_init() {
    TynspaceDaysState *tsd_state = malloc(sizeof(TynspaceDaysState));
    tsd_state->bpool = typool_allocate(1024, sizeof(float) * 4);
    tsd_state->rpool = typool_allocate(1024, sizeof(float) * 4);
    
    tsd_state->tyntbox.camera = false;
    tsd_state->tyntbox.greenscreen = false;
    tsd_state->tyntbox.texture_preview = false;
    
    Camera2D *camera = &tsd_state->camera;
    camera->offset = (Vector2){ viewport_w * 0.5, viewport_h * 0.5 };
    camera->target = (Vector2){ 0.0f, 0.0f };
    camera->rotation = 0.0f;
    camera->zoom = 0.7f;
    
    return tsd_state;
}

void tsd_state_dispose(TynspaceDaysState *tsd_state) {
    tynpool_dispose(tsd_state->bpool);
    tynpool_dispose(tsd_state->rpool);
    free(tsd_state);
}

#endif