#include "raylib.h"
#include "raymath.h"

#ifndef INDEX_H
#define INDEX_H

const int viewport_w = 512;
const int viewport_h = 512;

const Vector2 vleft = { -1, 0 };
const Vector2 vright = { 1, 0 };
const Vector2 vup = { 0, -1 };
const Vector2 vdown = { 0, 1 };

Vector2 getmp() {
    Vector2 mp = GetMousePosition();
    //Vector2 mp = { GetScreenWidth() * 0.5, GetScreenHeight() * 0.5 };
    return mp;
};

#endif