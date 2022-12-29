#include "BMP.h"

BMPFile* loadBMP(char* filename) {
    FILE* file = fopen(filename, "rb");

    if(!file) {
        printf("Can't read %s\n", filename);
        return NULL;
    }

    BMPFile *bmp_file = (BMPFile*) malloc(sizeof(BMPFile));
    rewind(file);

    fread(&bmp_file->bhdr, sizeof(BMPHeader), 1, file);
    fread(&bmp_file->dhdr, sizeof(DIBHeader), 1, file);

    bmp_file->data = (unsigned char*) malloc(bmp_file->dhdr.data_size);

    fseek(file, (int)bmp_file->bhdr.offset, SEEK_SET);
    fread(bmp_file->data, bmp_file->dhdr.data_size, 1, file);

    fclose(file);
    printf("File loaded\n");

    return bmp_file;
}

void makeBMP(char* path, BMPFile* bmp_file, int generation[][bmp_file->dhdr.width]) {
    FILE* file = fopen(path, "wb");
    int padding = (int)(bmp_file->dhdr.data_size/bmp_file->dhdr.height);
    padding = padding % (int)(3*bmp_file->dhdr.width);

    if(!file) {
        printf("Can't read %s\n", path);
        return;
    }
    rewind(file);

    BMPFile* new_bmp = (BMPFile*)malloc(sizeof(BMPFile));

    memcpy(&new_bmp->bhdr, &bmp_file->bhdr, sizeof(BMPHeader));
    memcpy(&new_bmp->dhdr, &bmp_file->dhdr, sizeof(DIBHeader));

    new_bmp->data = (unsigned char*)malloc(new_bmp->dhdr.data_size);

    for(size_t i = 0; i < new_bmp->dhdr.data_size; i++) {
        new_bmp->data[i] = 0;
    }

    rewind(file);

    fwrite(&new_bmp->bhdr, sizeof(BMPFile), 1, file);

    fwrite(&new_bmp->dhdr, sizeof(DIBHeader), 1, file);

    fseek(file, (int)bmp_file->bhdr.offset, SEEK_SET);

    int n = 0;
    int row = (int)new_bmp->dhdr.height - 1;
    int col = 0;

    while(n < new_bmp->dhdr.data_size) {
        for(int k = 0; k < 3; k++) {
            if(generation[row][col] == 1) {
                new_bmp->data[n] = 0;
            } else if(generation[row][col] == 0) {
                new_bmp->data[n] = 255;
            }
            n++;
        }

        col++;

        if(col == new_bmp->dhdr.width) {
            n += padding;
            row--;
            col = 0;
        }
    }

    for(int i = 0; i < new_bmp->dhdr.data_size; i++) {
        fwrite(&new_bmp->data[i], 1, sizeof(unsigned char), file);
    }

    fclose(file);
    freeBMP(new_bmp);
}

void freeBMP(BMPFile* bmp_file) {
    if(bmp_file) {
        free(bmp_file->data);
        free(bmp_file);
    }
}