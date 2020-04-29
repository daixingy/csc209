/* The functions operate on a linked list of free blocks.  Each node of the
 * list contains the starting location and the length of a free block.
 * 
 */

#include <string.h>
#include <stdlib.h>

#include "free_list.h"
#include "file_ops.h"

/* Give free space back to the free list.  Since the list is ordered by 
 * location, this function is the same for both algorithms.
 */
void add_free_block(FS *fs, int location, int size) {

	Freeblock *fb = malloc(sizeof(Freeblock));
	fb->offset = location;
	fb->length = size;
	fb->next = NULL;
	fb->total = location + size;

	Freeblock *first = fs->freelist;
	Freeblock *curr;

	if(first == NULL){	 //create a new freelist with the first block fb
		fs->freelist = fb;
		return;
	}else if(first->offset >= fb->total){ //insert the new block at the begging of the freelist
		fb->next = first;
        fs->freelist = fb;
        curr = fb;
	}else{
		curr = first;
		while(curr->next!=NULL && curr->next->offset < fb->total){ //keep looping until find the 
			curr = curr->next;                         //first block's offset that's larger than
		}                                              //fb->total

		if(curr->next == NULL){  //curr is the last block of the freelist,add fb at the end of list
			curr->next = fb;
			if(curr->total == fb->offset){
				curr->total =fb->total;
				curr->length += fb->length;
				curr->next = fb->next;
				free(fb);
			}
			return;
		}else if(curr -> total == fb->offset){ //curr is adjancent to fb, update the info to make 
			curr->length += fb->length;        //them in the same block
			curr->total = fb->total;

		}else{
			Freeblock *mid =curr->next;
			curr->next = fb;
			curr = curr->next;
			curr->next = mid;
		}
	}

	if(curr->total == curr->next->offset){
		curr->length += curr->next->length;
		curr->total = curr ->next->total;
		Freeblock *mid = curr->next->next;
		free(curr->next);
		curr->next=mid;
	}

	
}

/* Print the contents of the free list */

void show_freelist(FS *fs) {

	Freeblock *fb = fs->freelist;
	while(fb){
		printf("(offset: %d,length: %d)\n", fb->offset,fb->length);
		fb = fb->next;
	}

}


/* To be used after the metadata has been read from a file, to rebuild the
 * free list.
 */
void rebuild_freelist(FS *fs) {
    int i =0;
    int thei; 
    int min = MAX_FS_SIZE;
    int threshold = 0; //the minimum offset of last last round of loop
    int index = 0;
    int count = 0;
    Inode sorted_os[MAXFILES];

    for(int k =0;k<MAXFILES;k++){          //here we count the number of meatadata that the file it    	 
    	if(fs->metadata[k].offset != -1){  //corresponds to is not empty.
    		count++;
    	}
    }   

    for(i=0;i<MAXFILES;i++){ 
    	for(int j=0;j<MAXFILES;j++){  //keep looping to find the minimum offset in each loop
    	    if(fs->metadata[j].offset != -1 && fs->metadata[j].offset <= min && min >threshold){
    	    	min = fs->metadata[j].offset;
    	    	if(min > threshold){
    	    	    thei = j;
      	    	}else{
      	    		min = MAX_FS_SIZE;
      	    	}
    	    	
    	    } 	    
        }

        threshold = min; //update the threshold 
        min = MAX_FS_SIZE;  //reset minimum 
        if(index == 0){
        	sorted_os[index] = fs->metadata[thei];
        	index++;
        }
        else {
        	sorted_os[index] = fs->metadata[thei]; // add the meatadata of each nonempty file to
        	index++;                               // the array
        	if(index >=count){
        		break;
	        }      	
        }                	
    }


	for(int in =0;in<index ;in++){ 

		if(in == index-1){ 
			int last = sorted_os[in].offset + sorted_os[in].length;
			add_free_block(fs,last,512+MAX_FS_SIZE-last);
			break;
		}else{
			int start = sorted_os[in].offset +sorted_os[in].length;
			add_free_block(fs,start,sorted_os[in+1].offset-start);

		}
		
	}	
    return;
}
