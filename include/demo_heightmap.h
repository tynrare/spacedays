#include "raylib.h"

#ifndef DEMO_HEIGHTMAP_H
#define DEMO_HEIGHTMAP_H

typedef struct DemoHeightmapState {
    Model model;
    Vector3 position;
    Camera camera;
} DemoHeightmapState;

DemoHeightmapState demo_heightmap_state_init() {
    DemoHeightmapState dhs = { 0 };
    
    Image image = LoadImage("resources/heightmap.png");     // Load heightmap image (RAM)
    Texture2D texture = LoadTextureFromImage(image);        // Convert image to texture (VRAM)

    Mesh mesh = GenMeshHeightmap(image, (Vector3){ 16, 8, 16 }); // Generate heightmap mesh (RAM and VRAM)
    dhs.model = LoadModelFromMesh(mesh);                  // Load model from generated mesh

    dhs.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Set map diffuse texture
    dhs.position = (Vector3){ -8.0f, 0.0f, -8.0f };           // Define model position

    UnloadImage(image);             // Unload heightmap image from RAM, already uploaded to VRAM
    
    dhs.camera.position = (Vector3){ 2.0f, 10.0f, 0.0f };       // Camera position
    dhs.camera.target = (Vector3){ 0.0f, 9.0f, 0.0f };          // Camera looking at point
    dhs.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
    dhs.camera.fovy = 45.0f;                                    // Camera field-of-view Y
    dhs.camera.projection = CAMERA_PERSPECTIVE;                 // Camera projection type
    
    return dhs;
};

#endif