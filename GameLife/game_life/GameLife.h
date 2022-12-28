#ifndef CONWAY_S_GAME_LIFE_GAMELIFE_H
#define CONWAY_S_GAME_LIFE_GAMELIFE_H

#include "BMP.h"

void GameLife(char* input_filename, char* output_filename, char* directory);
int GameIteration(int ROWS, int COLS, int** gris_after, int** gris_before);
void draw(int ROWS, int COLS, int** gris_before);
void add_elements(int x, int y, int ROWS, int COLS, int** gris_after, int** gris_before);
void delete_elements(int ROWS, int COLS, int** gris_after, int** gris_before);
void initialization(BMPFile* bmpf, int ROWS, int COLS, int** gris_after, int** gris_before);
int count_comrades(int x, int y, int ROWS, int COLS, int** gris_before);

#endif