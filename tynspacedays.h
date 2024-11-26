#include "index.h"


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
        *x += GetRandomValue(viewport_w * 0.5 - 32, viewport_w * 0.5 + 32);
        *y += GetRandomValue(viewport_h * 0.5 - 32, viewport_h * 0.5 + 32);
    }
}

TynspaceDaysState *tsd_state_init() {
    TynspaceDaysState *tsd_state = malloc(sizeof(TynspaceDaysState));
    tsd_state->bpool = typool_allocate(1024, sizeof(float) * 4);
    
    return tsd_state;
}

void tsd_state_dispose(TynspaceDaysState *tsd_state) {
    tynpool_dispose(tsd_state->bpool);
    free(tsd_state);
}

#endif