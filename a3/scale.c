#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

int scale_factor;
void scale_filter(Bitmap *bmp) {
	int w = bmp->width;
    int h = bmp->height;
    Pixel row0[w];
    Pixel row1[w * scale_factor];
    for(int i=0;i<h;i++){
    	int count = 0;
    	if(fread(row0,sizeof(Pixel),w,stdin)==-1){
            printf("fread error");
            exit(1);
        }
    	int index = 0;
    	for(int j=0;j<w;j++){
    		int c = 0;
    		while(c <scale_factor){
    			row1[index] = row0[j];
    			c++;
    			index++;
    		}
    	}

    	while(count < scale_factor){
    		if(fwrite(row1,sizeof(Pixel),w*scale_factor,stdout)==-1){
                printf("fwrite error");
                exit(1);
            }
    		count++;
    	}
  	
    }
    
}


int main(int argc, char **argv) {
	scale_factor = strtol(argv[1], NULL, 10);
    run_filter(scale_filter, scale_factor);
    return 0;

}
