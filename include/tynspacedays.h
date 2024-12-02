#include "index.h"
#include "resources.h"

#ifndef TYNSPACEDAYS_H
#define TYNSPACEDAYS_H
#define bood bool

typedef struct TunspaceToolbox {
    bood hold;
    bool compas;
    bool scaner;
} TunspaceToolbox;

typedef struct TynspaceDaysState {
    struct TynPool *bpool;
    Camera2D camera;
    TunspaceToolbox tyntbox;
} TynspaceDaysState;

void tsd_state_stop(TynspaceDaysState *tsd_state) {
    while (tsd_state->bpool->idle) {
        TynPoolCell *cell = tynpool_cell_alloc(tsd_state->bpool);
        float *x = cell->point;
        float *y = x + 1;
        *x = 0;
        *y = 0;
    }
}

void tsd_state_run(TynspaceDaysState *tsd_state) {
    while (tsd_state->bpool->idle) {
        TynPoolCell *cell = tynpool_cell_alloc(tsd_state->bpool);
        float *x = cell->point;
        float *y = x + 1;
        float *dx = x + 2;
        float *dy = x + 3;
        *x += GetRandomValue(- 32, + 32);
        *y += GetRandomValue(- 32, + 32);
        *dx = 0.0f;
        *dy = 1.0f;
    }
}

TynspaceDaysState *tsd_state_init() {
    TynspaceDaysState *tsd_state = malloc(sizeof(TynspaceDaysState));
    tsd_state->bpool = typool_allocate(1024, sizeof(float) * 4);
    
    Camera2D *camera = &tsd_state->camera;
    camera->offset = (Vector2){ viewport_w * 0.5, viewport_h * 0.5 };
    camera->target = (Vector2){ 0.0f, 0.0f };
    camera->rotation = 0.0f;
    camera->zoom = 0.7f;
    
    return tsd_state;
}

void tsd_state_dispose(TynspaceDaysState *tsd_state) {
    tynpool_dispose(tsd_state->bpool);
    free(tsd_state);
}

#endif