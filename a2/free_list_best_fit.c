#include <string.h>
#include <stdlib.h>

#include "free_list.h"

/* Implement the best fit algorithm to find free space for the
   simulated file data.
 */

int get_free_block(FS *fs, int size) {
	Freeblock *fb = fs->freelist;
	int os = 0;
	int temp_size = 0;

	while(fb){ //keep looking for the smallest freeblock whose size is larger or equal to size
		if(fb->length>=size){
			if(!temp_size){
				os = fb->offset;
				temp_size = fb->length;
			}else{
				if(fb->length < temp_size){
					os = fb->offset;
					temp_size = fb->length;
				}
			}
		}
		fb = fb->next;
	}

	fb = fs->freelist;
	if(fb->offset == os){ //check if the first block has offset os,if so then update its size and offset
		fb->offset+=size;
		fb->length-=size;
		if(fb->length==0){
			free(fb);
			fs->freelist = fb -> next;
		}
		return os;
	}

	while(fb->next){ //find the freeblock with offset os 
		if(fb->next->offset == os){
			break;
		}
		fb = fb->next;
	}

	fb->next->offset+=size; //update thr info of the block found above
	fb->next->length-=size;
	if(fb->next->length == 0){
		Freeblock *block = fb->next->next;
		free(fb->next);
		fb->next =block;
	}
	return os;
}


