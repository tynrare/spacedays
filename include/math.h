#ifndef MATH_H
#define MATH_H

const Vector2 vleft = { -1, 0 };
const Vector2 vright = { 1, 0 };
const Vector2 vup = { 0, -1 };
const Vector2 vdown = { 0, 1 };
const Vector2 vzero = { 0, 0 };

float angle_diff(float angle1, float angle2) {
  double diff = ((int)angle2 - (int)angle1 + 180) % 360 - 180;
  return diff < -180 ? diff + 360 : diff;
}

float lerp(float a, float b, float t) { return a + (b - a) * t; }

float rlerp(float a, float b, float t) {
  float CS = (1.0f - t) * cosf(a) + t * cosf(b);
  float SN = (1.0f - t) * sinf(a) + t * sinf(b);

  return atan2f(SN, CS);
}

float dlerp(float a, float b, float decay, float dt) {
	return b + (a - b) * expf(-decay * dt);
}

#endif