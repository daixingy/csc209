#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

void update_row(Pixel *pre,Pixel *post,int w);

void gaussian_blur(Bitmap *bmp) {
    int w = bmp->width;
    int h = bmp->height;
    Pixel row0[w];
    Pixel row1[w];
    Pixel row2[w];
    Pixel temp[w];

    fread(row0,sizeof(Pixel),w,stdin);
    fread(row1,sizeof(Pixel),w,stdin);

    for(int j =0;j<=h-3;j++){

        if(j>0){
            update_row(row0,row1,w);
            update_row(row1,row2,w);
        }
        if(fread(row2,sizeof(Pixel),w,stdin)==-1){
            printf("fread error");
            exit(1);
        }
        for(int i =0;i<w-1;i++){
            temp[i+1]= apply_gaussian_kernel(row0+i,row1+i,row2+i);
        }
        temp[0] = temp[1];
        temp[w-1] = temp[w-2];
        if(fwrite(temp,sizeof(Pixel),w,stdout)==-1){
            printf("fwrite error");
            exit(1);
        }
        if(j==0 || j==h-3){
            if(fwrite(temp,sizeof(Pixel),w,stdout)==-1){
                printf("fwrite error");
                exit(1);
            }
        }

    }   
}

void update_row(Pixel *pre,Pixel *post,int w){
    for(int i =0;i<w;i++){
        pre[i] = post[i];
    }
}

int main() {
    run_filter(gaussian_blur, 1);
    return 0;

}
