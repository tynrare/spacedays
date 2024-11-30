#include "raylib.h"
#include "raymath.h"

#include "ru.h"
#include "tynpool.h"

#ifndef INDEX_H
#define INDEX_H

#define VIEWPORT_W 512
#define VIEWPORT_H 512

int viewport_w = VIEWPORT_W;
int viewport_h = VIEWPORT_H;

const Vector2 vleft = { -1, 0 };
const Vector2 vright = { 1, 0 };
const Vector2 vup = { 0, -1 };
const Vector2 vdown = { 0, 1 };
const Vector2 vzero = { 0, 0 };

Vector2 lmp = { -1, -1 };
Vector2 getmp() {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        lmp = GetMousePosition();
        return lmp;
    }
    
    if (lmp.x != -1 && lmp.y != -1) {
        return lmp;
    }
    
    Vector2 mp = { GetScreenWidth() * 0.5, GetScreenHeight() * 0.5 };
    return mp;
};

#endif