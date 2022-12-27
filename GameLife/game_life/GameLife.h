#ifndef CONWAY_S_GAME_LIFE_GAMELIFE_H
#define CONWAY_S_GAME_LIFE_GAMELIFE_H

#include "BMP.h"

int GameLife();
void draw();
void add_elements(int x, int y);
void delete_elements();
void initialization(BMPFile* bmpf);
int count_comrades(int x, int y);
int** get_gris();
int check_change();

#endif