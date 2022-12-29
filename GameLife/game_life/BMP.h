#ifndef CONWAY_S_GAME_LIFE_BMP_H
#define CONWAY_S_GAME_LIFE_BMP_H

#include "stdint.h"
#include "malloc.h"
#include "stdio.h"
#include "string.h"

#pragma pack(push, 1)
typedef struct BMPHeader {
    char ID[2];
    uint32_t file_size;
    char unused1[2];
    char unused2[2];
    uint32_t offset;
} BMPHeader;

typedef struct DIBHeader {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t color_planes;
    uint16_t bits_per_pixel;
    uint32_t BI_RGB;
    uint32_t data_size;
    uint32_t pwidth;
    uint32_t pheight;
    uint32_t colors_count;
    uint32_t imp_colors_count;
} DIBHeader;

typedef struct BMPFile {
    BMPHeader bhdr;
    DIBHeader dhdr;
    unsigned char *data;
} BMPFile;
#pragma pack(pop)

void makeBMP(char* path, BMPFile* bmp_file, int generation[][bmp_file->dhdr.width]);
BMPFile* loadBMP(char* filename);
void freeBMP(BMPFile* bmp_file);

#endif
