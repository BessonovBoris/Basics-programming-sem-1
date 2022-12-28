#include "BMP.h"

BMPFile* loadBMP(char* filename) {
    FILE* fp = fopen(filename, "r");
    if(!fp) {
        printf("Can't read %s\n", filename);
        return NULL;
    }

    BMPFile *bmp_file = (BMPFile*) malloc(sizeof(BMPFile));
    fread(&bmp_file->bhdr, sizeof(BMPHeader), 1, fp);
    fread(&bmp_file->dhdr, sizeof(DIBHeader), 1, fp);

    bmp_file->data = (unsigned char*) malloc(bmp_file->dhdr.data_size);
    fseek(fp, (int)bmp_file->bhdr.offset, SEEK_SET);
    fread(bmp_file->data, bmp_file->dhdr.data_size, 1, fp);

    fclose(fp);
    return bmp_file;
}

void makeBMP(FILE* file, BMPFile* bmpf, int** gris) {
    BMPFile* new_bmp = (BMPFile*)malloc(sizeof(BMPFile));

    memcpy(&new_bmp->bhdr, &bmpf->bhdr, sizeof(BMPHeader));
    memcpy(&new_bmp->dhdr, &bmpf->dhdr, sizeof(DIBHeader));

    new_bmp->data = (unsigned char*) malloc(new_bmp->dhdr.data_size);

    for(size_t i = 0; i < new_bmp->dhdr.data_size; i++) {
        new_bmp->data[i] = 0;
    }

    fwrite(&new_bmp->bhdr, sizeof(BMPFile), 1, file);
    fwrite(&new_bmp->dhdr, sizeof(DIBHeader), 1, file);

    fseek(file, (int)new_bmp->bhdr.offset, SEEK_SET);

    int n = 0;
    int row = (int)new_bmp->dhdr.height - 1;
    int col = 0;

    while(n < new_bmp->dhdr.data_size) {
        if(gris[row][col] == 1) {
            new_bmp->data[n] = 0;
            new_bmp->data[n+1] = 0;
            new_bmp->data[n+2] = 0;
        } else if(gris[row][col] == 0) {
            new_bmp->data[n] = 255;
            new_bmp->data[n+1] = 255;
            new_bmp->data[n+2] = 255;
        }

        n+=3;
        col++;

        if(col == new_bmp->dhdr.width) {
            n += 2;
            row--;
            col = 0;
        }
    }

    for(int i = 0; i < new_bmp->dhdr.data_size; i++) {
        fwrite(&new_bmp->data[i], 1, sizeof(unsigned char), file);
    }

    freeBMP(new_bmp);
}

void freeBMP(BMPFile* bmp_file) {
    if(bmp_file) {
        free(bmp_file->data);
        free(bmp_file);
    }
}