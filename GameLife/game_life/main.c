#include "GameLife.h"

void print_pixels(BMPFile* bmpf) {
    for(int i = 0; i < bmpf->dhdr.data_size; i+=1) {
        if(i % (bmpf->dhdr.width*3 + 2) == bmpf->dhdr.width*3) {
            printf("\n");
            i+=2;
        }
        if(i % 3 == 0)
            printf("    |    ");

        printf("%02d ", bmpf->data[i]);
    }

    printf("\nNEW_BMP: WIDTH = %d | HEIGHT = %d | OFFSET = %d | DATA SIZE = %d ", bmpf->dhdr.width, bmpf->dhdr.height, bmpf->bhdr.offset, bmpf->dhdr.data_size);
}

int main(int argc, char* argv[]) {
    char input_filename[100] = "ex.bmp";
    char output_filename[100] = "generation_";
    char directory[100] = "generations/";
    char bmp_filename[100];

    for(int i = 0; i < argc; i++) {
        if(!strcmp(argv[i], "-input")) {
            if(i+1 >= argc) {
                printf("ERROR IN INPUT!!!");
                return 0;
            }

            strcpy(input_filename, argv[i+1]);
        } else if(!strcmp(argv[i], "-output")) {
            if(i+1 >= argc) {
                printf("ERROR IN INPUT!!!");
                return 0;
            }

            strcpy(output_filename, argv[i+1]);
        } else if(!strcmp(argv[i], "-dir_to_save")) {
            if(i+1 >= argc) {
                printf("ERROR IN INPUT!!!");
                return 0;
            }

            strcpy(directory, argv[i+1]);
        }
    }

    BMPFile* bmpf = loadBMP(input_filename);

    int iteration = 0;
    char string[100];
    char path[100];

    initialization(bmpf);
    int game_is_changed;
    draw();
    while (iteration != 1) {
        game_is_changed = GameLife();
        iteration++;

        strcpy(bmp_filename, output_filename);
        strcat(bmp_filename, itoa(iteration, string, 10));
        strcat(bmp_filename, ".bmp");

        strcpy(path, directory);
        strcat(path, bmp_filename);

        FILE* file = fopen(bmp_filename, "wb");
        makeBMP(file, bmpf, get_gris());

//        print_pixels(bmpf);

        fclose(file);

        if(game_is_changed == 0)
            break;
    }

    freeBMP(bmpf);

    return 0;
}


