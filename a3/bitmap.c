#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"


/*
 * Read in bitmap header data from stdin, and return a pointer to
 * a new Bitmap struct containing the important metadata for the image file.
 *
 * TODO: complete this function.
 *
 * Notes:
 *   1. Store header data in an array of unsigned char (essentially
 *      an array of bytes). Examples:
 *      on the stack,
 *          unsigned char data[10];
 *      on the heap,
 *          unsigned char *data = malloc(10);
 *
 *   2. Don't make any assumptions about the header size. You should read in
 *      BMP_HEADER_SIZE_OFFSET bytes first, and then the header size,
 *      and then use this to allocate enough space for the actual header.
 *
 *   3. You can use memcpy to transfer bytes to and from the Bitmap "header" field.
 *      You can even write these bytes to memory allocated for variables of other types!
 *      For example:
 *          unsigned char bytes[4];
 *          int x = 10;
 *          int y;
 *          memcpy(bytes, &x, 4);  // Copy the int x into bytes.
 *          memcpy(&y, bytes, 4);  // Copy the contents of bytes into y.
 *
 *   4. You can use either fread/fwrite or read/write to perform I/O operations here.
 *
 *   5. Make good use of the provided macros in bitmap.h to index into the "header" array.
 */
Bitmap *read_header() {
    unsigned char data[10];
    if(fread(data,sizeof(char),10,stdin)== -1){
        printf("fread error");
        exit(1);

    }
    unsigned char hs[4]; 
    if(fread(hs,sizeof(char),4,stdin)==-1){
        printf("fread error");
        exit(1);
    }
    int header_size;
    memcpy(&header_size,hs,4);
    Bitmap *bmp = malloc(sizeof(Bitmap));
    bmp->headerSize = header_size;
    unsigned char left_contents[header_size - 14];
    if(fread(left_contents,sizeof(char), header_size-14 ,stdin)==-1){
        printf("fread error");
        exit(1);
    }
    bmp->header = malloc(bmp->headerSize);
    memcpy(bmp->header,data,10);
    memcpy(bmp->header+10,hs,4);
    memcpy(bmp->header+14,left_contents,header_size-14);
    memcpy(&bmp->width,left_contents+4,4);
    memcpy(&bmp->height,left_contents+8,4);

    return bmp;
}

/*
 * Write out bitmap metadata to stdout.
 * You may add extra fprintf calls to *stderr* here for debugging purposes.
 */
void write_header(const Bitmap *bmp) {
    if(fwrite(bmp->header, bmp->headerSize, 1, stdout)==-1){
        printf("fwrite error");
        exit(1);
    }
}

/*
 * Free the given Bitmap struct.
 */
void free_bitmap(Bitmap *bmp) {
    free(bmp->header);
    free(bmp);
}

/*
 * Update the bitmap header to record a resizing of the image.
 *
 * TODO: complete this function when working on the "scale" filter.
 *
 * Notes:
 *   1. As with read_header, use memcpy and the provided macros in bitmap.h.
 *
 *   2. bmp->header *must* be updated, as this is what's written out
 *      in write_header.
 *
 *   3. You may choose whether or not to also update bmp->width and bmp->height.
 *      This choice may depend on how you implement the scale filter.
 */
void scale(Bitmap *bmp, int scale_factor) {
    int file_size;
    memcpy(&file_size,bmp->header+2,4);
    int new_width = bmp->width * scale_factor;
    int new_height = bmp->height * scale_factor;
    int new_size = file_size + sizeof(Pixel)*(bmp->width * bmp->height * (square(scale_factor)-1));
    memcpy(bmp->header+2,&new_size,4);
    memcpy(bmp->header+18,&new_width,4);
    memcpy(bmp->header+22,&new_height,4);
}


/*
 * The "main" function.
 *
 * Run a given filter function, and apply a scale factor if necessary.
 * You don't need to modify this function to make it work with any of
 * the filters for this assignment.
 */
void run_filter(void (*filter)(Bitmap *), int scale_factor) {
    Bitmap *bmp = read_header();

    if (scale_factor > 1) {
        scale(bmp, scale_factor);
    }

    write_header(bmp);

    // Note: here is where we call the filter function.
    filter(bmp);

    free_bitmap(bmp);
}


/******************************************************************************
 * The gaussian blur and edge detection filters.
 * You should NOT modify any of the code below.
 *****************************************************************************/
const int gaussian_kernel[3][3] = {
    {1, 2, 1},
    {2, 4, 2},
    {1, 2, 1}
};

const int kernel_dx[3][3] = {
    {1, 0, -1},
    {2, 0, -2},
    {1, 0, -1}
};

const int kernel_dy[3][3] = {
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2, -1}
};

const int gaussian_normalizing_factor = 16;


Pixel apply_gaussian_kernel(Pixel *row0, Pixel *row1, Pixel *row2) {
    int b = 0, g = 0, r = 0;
    Pixel *rows[3] = {row0, row1, row2};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            b += rows[i][j].blue * gaussian_kernel[i][j];
            g += rows[i][j].green * gaussian_kernel[i][j];
            r += rows[i][j].red * gaussian_kernel[i][j];
        }
    }

    b /= gaussian_normalizing_factor;
    g /= gaussian_normalizing_factor;
    r /= gaussian_normalizing_factor;

    Pixel new = {
        .blue = b,
        .green = g,
        .red = r
    };

    return new;
}


Pixel apply_edge_detection_kernel(Pixel *row0, Pixel *row1, Pixel *row2) {
    int b_dx = 0, b_dy = 0;
    int g_dx = 0, g_dy = 0;
    int r_dx = 0, r_dy = 0;
    Pixel *rows[3] = {row0, row1, row2};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            b_dx += rows[i][j].blue * kernel_dx[i][j];
            b_dy += rows[i][j].blue * kernel_dy[i][j];
            g_dx += rows[i][j].green * kernel_dx[i][j];
            g_dy += rows[i][j].green * kernel_dy[i][j];
            r_dx += rows[i][j].red * kernel_dx[i][j];
            r_dy += rows[i][j].red * kernel_dy[i][j];
        }
    }
    int b = floor(sqrt(square(b_dx) + square(b_dy)));
    int g = floor(sqrt(square(g_dx) + square(g_dy)));
    int r = floor(sqrt(square(r_dx) + square(r_dy)));

    int edge_val = max(r, max(g, b));
    Pixel new = {
        .blue = edge_val,
        .green = edge_val,
        .red = edge_val
    };

    return new;
}
