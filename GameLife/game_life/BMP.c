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
    fseek(fp, bmp_file->bhdr.offset, SEEK_SET);
    fread(bmp_file->data, bmp_file->dhdr.data_size, 1, fp);

    fclose(fp);
    return bmp_file;
}

void makeBMP(FILE* file, BMPFile* bmpf, int** gris) {
    BMPFile* new_bmp = (BMPFile*)malloc(sizeof(BMPFile));
    new_bmp->bhdr = bmpf->bhdr;
    new_bmp->dhdr = bmpf->dhdr;
//    memcpy(&new_bmp->bhdr, &bmpf->bhdr, sizeof(BMPHeader));
//    memcpy(&new_bmp->dhdr, &bmpf->dhdr, sizeof(DIBHeader));

    new_bmp->data = (unsigned char*) malloc(bmpf->dhdr.data_size);

    for(int i = 0; i < bmpf->dhdr.data_size; i++) {
        new_bmp->data[i] = bmpf->data[i];
        new_bmp->data[i] = 0;
    }

//    int r = bmpf->dhdr.height-1, c = 0;
//    for(int i = 0; i < bmpf->dhdr.data_size;) {
//        if(gris[r][c] == 0) {
//            bmpf->data[i] = 255;
//            bmpf->data[i+1] = 255;
//            bmpf->data[i+2] = 255;
//        }
//        if(gris[r][c] == 1) {
//            bmpf->data[i] = 0;
//            bmpf->data[i+1] = 0;
//            bmpf->data[i+2] = 0;
//        }
//
//        c++;
//        i += 3;
//
//        if(i % (bmpf->dhdr.width*3+2) == bmpf->dhdr.width*3) {
//            c = 0;
//            r--;
//            i += 2;
//        }
//    }

    fwrite(&new_bmp->bhdr, sizeof(BMPFile), 1, file);
    fwrite(&new_bmp->dhdr, sizeof(DIBHeader), 1, file);

    for(int i = 0; i < bmpf->dhdr.data_size; i++) {
        fwrite(&bmpf->data[i], 1, 1, file);
    }

    freeBMP(new_bmp);
}

void freeBMP(BMPFile* bmp_file) {
    if(bmp_file) {
        free(bmp_file->data);
        free(bmp_file);
    }
}