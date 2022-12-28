#include "GameLife.h"

void GameLife(char* input_filename, char* output_filename, char* directory) {
    BMPFile* bmpf = loadBMP(input_filename);

    int ROWS = (int)bmpf->dhdr.width;
    int COLS = (int)bmpf->dhdr.height;

    int gris_before[ROWS][COLS];
    int gris_after[ROWS][COLS];

    int iteration = 0;
    int game_is_changed;
    char bmp_filename[100];
    char path[100];
    char string[100];

    int* f_gris_after[COLS];
    int* f_gris_before[COLS];

    for(int i = 0; i < COLS; i++)
        f_gris_after[i] = gris_after[i];
    for(int i = 0; i < COLS; i++)
        f_gris_before[i] = gris_before[i];

    initialization(bmpf, ROWS, COLS, f_gris_after, f_gris_before);

    draw(ROWS, COLS, f_gris_before);
    while(iteration != 6) {
        game_is_changed = GameIteration(ROWS, COLS, f_gris_after, f_gris_before);
        iteration++;

        if(game_is_changed == 0)
            break;

        strcpy(bmp_filename, output_filename);
        strcat(bmp_filename, itoa(iteration, string, 10));
        strcat(bmp_filename, ".bmp");

        strcpy(path, directory);
        strcat(path, bmp_filename);

        FILE* file = fopen(path, "wb");

        makeBMP(file, bmpf, f_gris_before);
        fclose(file);
    }

    freeBMP(bmpf);
}

int GameIteration(int ROWS, int COLS, int** gris_after, int** gris_before) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++)
            gris_after[i][j] = gris_before[i][j];
    }

    delete_elements(ROWS, COLS, gris_after, gris_before);

    for(int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            add_elements(i, j, ROWS, COLS, gris_after, gris_before);
        }
    }

    int similar = 0;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(gris_before[i][j] != gris_after[i][j])
                similar = 1;
            gris_before[i][j] = gris_after[i][j];
        }
    }

    return similar;
}

void draw(int ROWS, int COLS, int** gris_before) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf(" %d", gris_before[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------------------------------\n");
}

void add_elements(int x, int y, int ROWS, int COLS, int** gris_after, int** gris_before) {
    int comrades = count_comrades(x, y, ROWS, COLS, gris_before);

    if(comrades == 3)
        gris_after[x][y] = 1;
}

void delete_elements(int ROWS, int COLS, int** gris_after, int** gris_before) {
    for(int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int comrades = count_comrades(i, j, ROWS, COLS, gris_before);

            if(comrades <= 1 || comrades >= 4)
                gris_after[i][j] = 0;
        }
    }
}

void initialization(BMPFile* bmpf, int ROWS, int COLS, int** gris_after, int** gris_before) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            gris_before[i][j] = 0;
        }
    }

    int r = (int)bmpf->dhdr.height - 1;
    int c = 0;
    int n = 0;

    while(n < bmpf->dhdr.data_size) {
        if(bmpf->data[n] == 0)
            gris_before[r][c] = 1;

        c++;
        n += 3;

        if(c == bmpf->dhdr.width) {
            c = 0;
            r--;
            n += 2;
        }
    }
}

int count_comrades(int x, int y, int ROWS, int COLS, int** gris_before) {
    int comrades = 0;

    if(y >= 1 && gris_before[x][y-1] == 1)
        comrades++;
    if(y < COLS-1 && gris_before[x][y+1] == 1)
        comrades++;
    if(x >= 1 && gris_before[x-1][y] == 1)
        comrades++;
    if(x < ROWS-1 && gris_before[x+1][y] == 1)
        comrades++;
    if(x < ROWS-1 && y < COLS-1 && gris_before[x+1][y+1] == 1)
        comrades++;
    if(x < ROWS-1 && y >= 1 && gris_before[x+1][y-1] == 1)
        comrades++;
    if(x >= 1 && y < COLS-1 && gris_before[x-1][y+1] == 1)
        comrades++;
    if(x >= 1 && y >= 1 && gris_before[x-1][y-1] == 1)
        comrades++;

    return comrades;
}