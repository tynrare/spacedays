#ifndef RESOURCES_H
#define RESOURCES_H

#define RESOURCES_PATH "resources/"
#define SHADERS_PATH  RESOURCES_PATH "shaders/"
#define TEXTURES_PATH  RESOURCES_PATH "textures/"

static bool file_modified(const char *filename, int filetime) {
    int file_timestamp = GetFileModTime(filename);
    return file_timestamp != filetime;
}

#endif