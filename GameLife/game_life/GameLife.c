#include "GameLife.h"

void GameLife(char* input_filename, char* output_filename, char* directory, int generations_count) {
    BMPFile* bmp = loadBMP(input_filename);
    if(!bmp) {
        return;
    }

    int ROWS = (int)bmp->dhdr.height;
    int COLS = (int)bmp->dhdr.width;

    int gris_before[ROWS][COLS];
    int gris_after[ROWS][COLS];

    int iteration = 0;
    int game_is_changed;
    char bmp_filename[100];
    char path[100];
    char string[100];

    printf("Variables initialized\n");

    initialization(bmp, ROWS, COLS, gris_before);

    printf("Initialization completed\n");

//    draw(ROWS, COLS, gris_before);
//    print_pixels(bmp);

    while(iteration != generations_count) {
        game_is_changed = GameIteration(ROWS, COLS, gris_after, gris_before);
        iteration++;
//        draw(ROWS, COLS, gris_before);

        if(game_is_changed == 0)
            break;

        strcpy(bmp_filename, output_filename);
        strcat(bmp_filename, itoa(iteration, string, 10));
        strcat(bmp_filename, ".bmp");

        strcpy(path, directory);
        strcat(path, bmp_filename);

        makeBMP(path, bmp, gris_before);
    }

//    printf("SIZE = %d\n", bmp->dhdr.data_size);

    freeBMP(bmp);

    printf("Program finished\n");
}

int GameIteration(int ROWS, int COLS, int gris_after[][COLS], int gris_before[][COLS]) {
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

void add_elements(int x, int y, int ROWS, int COLS, int gris_after[][COLS], int gris_before[][COLS]) {
    int comrades = count_comrades(x, y, ROWS, COLS, gris_before);

    if(comrades == 3)
        gris_after[x][y] = 1;
}

void delete_elements(int ROWS, int COLS, int gris_after[][COLS], int gris_before[][COLS]) {
    for(int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int comrades = count_comrades(i, j, ROWS, COLS, gris_before);

            if(comrades <= 1 || comrades >= 4)
                gris_after[i][j] = 0;
        }
    }
}

void initialization(BMPFile* bmp, int ROWS, int COLS, int gris_before[][COLS]) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            gris_before[i][j] = 0;
        }
    }

    int r = (int)bmp->dhdr.height - 1;
    int c = 0;
    int n = 0;

    while(n < bmp->dhdr.data_size) {
        if(bmp->data[n] == 0)
            gris_before[r][c] = 1;

        c++;
        n += 3;

        if(c == bmp->dhdr.width) {
            c = 0;
            r--;
            n += 2;
        }
    }
}

int count_comrades(int x, int y, int ROWS, int COLS, int gris_before[][COLS]) {
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

void draw(int ROWS, int COLS, int gris[][COLS]) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(gris[i][j] == 1)
                printf("#");
            else
                printf(".");
        }
        printf("\n");
    }

    printf("\n\n-------------------------------------------\n");
}

void print_pixels(BMPFile* bmp) {
    printf("WIDTH = %d | HEIGHT = %d | DATA_SIZE = %d\n\n", bmp->dhdr.width, bmp->dhdr.height, bmp->dhdr.data_size);

    for(int i = 0; i < bmp->dhdr.data_size;) {
        if(i % (3*bmp->dhdr.width + 2) == 0) {
            printf("\n");
            i += 2;
        }

        printf("%02x ", bmp->data[i]);
        i += 3;
    }
    printf("\n");
}