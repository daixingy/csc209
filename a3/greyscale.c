#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"


void greyscale_filter(Bitmap *bmp) {
	Pixel p;
    Pixel q;
    int av;
    for(int i =0;i<bmp->width;i++){
    	for(int j =0;j<bmp->height;j++){
    		if(fread(&p,sizeof(Pixel),1,stdin)==-1){
                printf("fread error");
                exit(1);
            }
            av = (p.blue + p.green + p.red) / 3;
            q.blue = av;
            q.green = av;
            q.red = av;
    		if(fwrite(&q,sizeof(Pixel),1,stdout)==-1){
                printf("fwrite error");
                exit(1);
            }
    	}
    }
	
}

int main() {
    // Run the filter program with copy_filter to process the pixels.
    // You shouldn't need to change this implementation.
    run_filter(greyscale_filter, 1);
    return 0;
}