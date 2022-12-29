#ifndef CONWAY_S_GAME_LIFE_GAMELIFE_H
#define CONWAY_S_GAME_LIFE_GAMELIFE_H

#include "BMP.h"
#include "windows.h"

void GameLife(char* input_filename, char* output_filename, char* directory, int generations_count);
int GameIteration(int ROWS, int COLS, int gen_after[][COLS], int gen_before[][COLS]);
void add_elements(int x, int y, int ROWS, int COLS, int gen_after[][COLS], int gen_before[][COLS]);
void delete_elements(int ROWS, int COLS, int gen_after[][COLS], int gen_before[][COLS]);
void initialization(BMPFile* bmp, int ROWS, int COLS, int gen_before[][COLS]);
int count_comrades(int x, int y, int ROWS, int COLS, int gen_before[][COLS]);
void draw(int ROWS, int COLS, int generation[][COLS]);
void print_pixels(BMPFile* bmp);

#endif