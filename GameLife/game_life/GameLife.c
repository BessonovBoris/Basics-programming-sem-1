#include "GameLife.h"

#define ROWS 10
#define COLS 10

int gris_before[ROWS][COLS];
int gris_after[ROWS][COLS];

int GameLife() {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++)
            gris_after[i][j] = gris_before[i][j];
    }

    delete_elements();

    for(int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            add_elements(i, j);
        }
    }

    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(gris_before[i][j] == gris_after[i][j])
                return 0;
            gris_before[i][j] = gris_after[i][j];
        }
    }
    draw();

    return 1;
}

void draw() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf(" %d", gris_before[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------------------------------\n");
}

void add_elements(int x, int y) {
    int comrades = count_comrades(x, y);

    if(comrades == 3)
        gris_after[x][y] = 1;
}

void delete_elements() {
    for(int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int comrades = count_comrades(i, j);

            if(comrades <= 1 || comrades >= 4)
                gris_after[i][j] = 0;
        }
    }
}

void initialization(BMPFile* bmpf) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            gris_before[i][j] = 0;
        }
    }

    int r = bmpf->dhdr.height-1, c = 0;
    for(int i = 0; i < bmpf->dhdr.data_size;) {
        if(bmpf->data[i] == 0)
            gris_before[r][c] = 1;

        c++;
        i += 3;

        if(i % (bmpf->dhdr.width*3+2) == bmpf->dhdr.width*3) {
            c = 0;
            r--;
            i += 2;
        }
    }
}

int count_comrades(int x, int y) {
    int comrades = 0;

    if(gris_before[x][y-1] == 1 && y >= 1)
        comrades++;
    if(gris_before[x][y+1] == 1 && y < COLS-1)
        comrades++;
    if(gris_before[x-1][y] == 1 && x >= 1)
        comrades++;
    if(gris_before[x+1][y] == 1 && x < ROWS-1)
        comrades++;
    if(gris_before[x+1][y+1] == 1 && x < ROWS-1 && y < COLS-1)
        comrades++;
    if(gris_before[x+1][y-1] == 1 && x < ROWS-1 && y >= 1)
        comrades++;
    if(gris_before[x-1][y+1] == 1 && x >= 1 && y < COLS-1)
        comrades++;
    if(gris_before[x-1][y-1] == 1 && x >= 1 && y >= 1)
        comrades++;

    return comrades;
}

int** get_gris() {
    return gris_before;
}