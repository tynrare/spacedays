#include "index.h"
#include "tyngame_locomotions.h"
#include <math.h>

#ifndef TYNTBOX_H
#define TYNTBOX_H

#define TYNCHANT_DZENSUN_POOLD_ 1024
#define TYNCHANT_DZENSUN_AIMSD_  8
#define TYNCHANT_DZENSUN_SUMMONDIST_  64
#define TYNCHANT_DZENSUN_FREC_  0.001

typedef struct TynChantDzenSun {
    struct TynPool *pool;
    struct TynPool *aims;
    double elapsed;
} TynChantDzenSun;

void chant_dzensun_init(TynChantDzenSun *tchant_dzen_sun) {
    tchant_dzen_sun->pool = typool_allocate(TYNCHANT_DZENSUN_POOLD_,  sizeof(float) * 6);
    tchant_dzen_sun->aims = typool_allocate(TYNCHANT_DZENSUN_AIMSD_,  sizeof(float) * 2);
    
    TynPoolCell *cell = tynpool_cell_alloc(tchant_dzen_sun->aims);
     float *r = cell->point;
     float *x = r + 0;
     float *y = r + 1;
}


#define PAD_LVBT 1024
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


void chant_dzensun_step(
    TynChantDzenSun *tchant_dzen_sun, 
    void (*callback)(float *, float *,float *, float *, float *)) 
    {
        tchant_dzen_sun->elapsed += GetFrameTime();
        
        
        TynPoolCell *aims_p = tchant_dzen_sun->aims->active;
        float *aims_x = aims_p->point + 0;
        float *aims_y = aims_p->point + 1;
        Vector2 vaims = { *aims_x, *aims_y };
        
        float watts = 1.0;
        
        for (TynPoolCell *p = tchant_dzen_sun->pool->active; p; p = p->next) {
            float *x = p->point;
            float *y = x + 1;
            float *dx = x + 2;
            float *dy = x + 3;
            float *e = x + 4;
            float *intensity = x + 5;
            
            *e += GetFrameTime();
            *intensity = fminf(*e, 1.0);
 
            if (*e > 0.4) {
                locomotion_pull(x, y, dx, dy, vaims); // pull in
            } else {
                watts = 2.0;
                locomotion_push(x, y, dx, dy, vaims); // push out
            }
        
            *x += *dx * watts;
            *y += *dy * watts;
            
            Vector2 delta = { vaims.x - *x,  vaims.y - *y };
            float dist = Vector2Length(delta);
            float deta = dist - TYNCHANT_DZENSUN_SUMMONDIST_ * 0.5;
            *intensity = fminf(*intensity, deta);
            
            if (deta < 0) {
                tynpool_cell_free(tchant_dzen_sun->pool, p);
                continue;
            }
            
            loc_viewport_bound_teleport(x, y);
            callback(x, y, dx, dy, intensity);
        }
        
        
        
        if ( 
            tchant_dzen_sun->pool->idle 
            && 
            tchant_dzen_sun->elapsed > TYNCHANT_DZENSUN_FREC_)
            {
                tchant_dzen_sun->elapsed = 0;
                TynPoolCell *cell = tynpool_cell_alloc(tchant_dzen_sun->pool);
                float *r = cell->point;
                float *x = r + 0;
                float *y = r + 1;
                float *dx = r + 2;
                float *dy = r + 3;
                float *e = r + 4;
                float *intensity = x + 5;
                float rx = GetRandomValue(-256, 256);
                float ry = GetRandomValue(-256, 256);
                Vector2 dist = { rx, ry };
                Vector2 d = Vector2Normalize(dist);
                 *x = *aims_x + d.x* TYNCHANT_DZENSUN_SUMMONDIST_;
                *y = *aims_y + d.y * TYNCHANT_DZENSUN_SUMMONDIST_;
                *dx = d.x;
                *dy = d.y;
                *e = 0.0;
                *intensity = 0.0;
        }
}

typedef struct TynspaceChantbox {
    TynChantDzenSun dzen_sun;
} TynspaceChantbox;

typedef struct TynspaceToolbox {
    bool hold;
    bool compas;
    bool scaner;
    bool camera;
    bool greenscreen;
    bool texture_preview;
    
    TynspaceChantbox chantbox;
} TynspaceToolbox;


void tyntbox_init_default(TynspaceToolbox *tyntbox) {
    tyntbox->hold = false;
    tyntbox->compas = false;
    tyntbox->camera = false;
    tyntbox->greenscreen = false;
    tyntbox->texture_preview = true;
    
     chant_dzensun_init(&tyntbox->chantbox.dzen_sun);
}

#endif
