#include "GameLife.h"

int main(int argc, char* argv[]) {
    char input_filename[100] = "ex.bmp";
    char output_filename[100] = "generation_";
    char directory[100] = "generations/";

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

    GameLife(input_filename, output_filename, directory);

    return 0;
}


