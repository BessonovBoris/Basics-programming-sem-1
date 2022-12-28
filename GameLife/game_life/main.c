#include "GameLife.h"

int string_to_int(char* string);

int main(int argc, char* argv[]) {
    char input_filename[100] = "ex39.bmp";
    char output_filename[100] = "generation_";
    char directory[100] = "generations/";
    int generations_count = 20;

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
        } else if(!strcmp(argv[i], "-generations_count")) {
            if(i+1 >= argc) {
                printf("ERROR IN INPUT!!!");
                return 0;
            }

            generations_count = string_to_int(argv[i + 1]);
        } else if(!strcmp(argv[i], "-help")) {
            printf("-input - name of input file\n-output - name of outputs files\n-dir_to_save - directory where save files\n-generations_count - count of generations\n");

            return 0;
        }
    }

    GameLife(input_filename, output_filename, directory, generations_count);

    return 0;
}

int string_to_int(char* string) {
    int number = 0;

    int i = 0;
    int radix = 1;
    while(*(string+i) != '\0') {
        i++;
    }
    i--;

    while(i >= 0) {
        number += radix*((int)(*(string+i)) - 48);

        i--;
        radix*=10;
    }

    return number;
}


