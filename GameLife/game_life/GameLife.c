#include "GameLife.h"

void GameLife(char* input_filename, char* output_filename, char* directory, int generations_count) {
    BMPFile* bmp = loadBMP(input_filename);
    if(!bmp) {
        return;
    }

    int ROWS = (int)bmp->dhdr.height;
    int COLS = (int)bmp->dhdr.width;

    printf("ROWS = %d | COLS = %d | DATA_SIZE = %d | FILE_SIZE = %d\n", ROWS, COLS, bmp->dhdr.data_size, bmp->dhdr.size);
    int gen_before[ROWS][COLS];
    int gen_after[ROWS][COLS];

    int iteration = 0;
    int game_is_changed;
    char bmp_filename[100];
    char path[100];
    char string[100];

    printf("Variables initialized\n");

    initialization(bmp, ROWS, COLS, gen_before);

    printf("Initialization completed\n");

//    draw(ROWS, COLS, gen_before);
//    print_pixels(bmp);

    while(iteration != generations_count) {
        iteration++;
        printf("Calculating generation: %d\r", iteration);

        game_is_changed = GameIteration(ROWS, COLS, gen_after, gen_before);
//        draw(ROWS, COLS, gen_before);

        if(game_is_changed == 0)
            break;

        strcpy(bmp_filename, output_filename);
        strcat(bmp_filename, itoa(iteration, string, 10));
        strcat(bmp_filename, ".bmp");

        strcpy(path, directory);
        strcat(path, bmp_filename);

        makeBMP(path, bmp, gen_before);
    }

    freeBMP(bmp);

    printf("\nProgram finished\n");
}

int GameIteration(int ROWS, int COLS, int gen_after[][COLS], int gen_before[][COLS]) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++)
            gen_after[i][j] = gen_before[i][j];
    }

    delete_elements(ROWS, COLS, gen_after, gen_before);

    for(int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            add_elements(i, j, ROWS, COLS, gen_after, gen_before);
        }
    }

    int similar = 0;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(gen_before[i][j] != gen_after[i][j])
                similar = 1;
            gen_before[i][j] = gen_after[i][j];
        }
    }

    return similar;
}

void add_elements(int x, int y, int ROWS, int COLS, int gen_after[][COLS], int gen_before[][COLS]) {
    int comrades = count_comrades(x, y, ROWS, COLS, gen_before);

    if(comrades == 3)
        gen_after[x][y] = 1;
}

void delete_elements(int ROWS, int COLS, int gen_after[][COLS], int gen_before[][COLS]) {
    for(int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int comrades = count_comrades(i, j, ROWS, COLS, gen_before);

            if(comrades <= 1 || comrades >= 4)
                gen_after[i][j] = 0;
        }
    }
}

void initialization(BMPFile* bmp, int ROWS, int COLS, int gen_before[][COLS]) {
    int padding = (int)(bmp->dhdr.data_size/bmp->dhdr.height);
    padding = padding % (int)(3*bmp->dhdr.width);

    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            gen_before[i][j] = 0;
        }
    }

    int r = (int)bmp->dhdr.height - 1;
    int c = 0;
    int n = 0;

    while(n < bmp->dhdr.data_size) {
        if(bmp->data[n] == 0)
            gen_before[r][c] = 1;

        c++;
        n += 3;

        if(c == bmp->dhdr.width) {
            c = 0;
            r--;
            n += padding;
        }
    }
}

int count_comrades(int x, int y, int ROWS, int COLS, int gen_before[][COLS]) {
    int comrades = 0;

    if(y >= 1 && gen_before[x][y - 1] == 1)
        comrades++;
    if(y < COLS-1 && gen_before[x][y + 1] == 1)
        comrades++;
    if(x >= 1 && gen_before[x - 1][y] == 1)
        comrades++;
    if(x < ROWS-1 && gen_before[x + 1][y] == 1)
        comrades++;
    if(x < ROWS-1 && y < COLS-1 && gen_before[x + 1][y + 1] == 1)
        comrades++;
    if(x < ROWS-1 && y >= 1 && gen_before[x + 1][y - 1] == 1)
        comrades++;
    if(x >= 1 && y < COLS-1 && gen_before[x - 1][y + 1] == 1)
        comrades++;
    if(x >= 1 && y >= 1 && gen_before[x - 1][y - 1] == 1)
        comrades++;

    return comrades;
}

void draw(int ROWS, int COLS, int generation[][COLS]) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(generation[i][j] == 1)
                printf("#");
            else
                printf(".");
        }
        printf("\n");
    }

    printf("\n\n-------------------------------------------\n");
}

void print_pixels(BMPFile* bmp) {
    int padding = (int)(bmp->dhdr.data_size/bmp->dhdr.height);
    padding = padding % (int)(3*bmp->dhdr.width);

    for(int i = 0; i < bmp->dhdr.data_size;) {
        if(i % (3*bmp->dhdr.width + padding) == 0) {
            printf("\n");
            i += padding;
        }

        printf("%02x ", bmp->data[i]);
        i += 3;
    }
    printf("\n");
}