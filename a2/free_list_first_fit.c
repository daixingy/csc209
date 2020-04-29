#include <string.h>
#include <stdlib.h>

#include "free_list.h"

/* Implement the first fit algorithm to find free space for the
   simulated file data.
 */

int get_free_block(FS *fs, int size) {

	Freeblock *curr;
	curr = fs->freelist;
	int os;
	if(curr->length >= size){ //check if the first block has length bigger than size
		os = curr->offset;    //if so then update its info
		curr->offset += size;
		curr->length -= size;
		return os;

		if(curr->length == 0){
			fs->freelist = fs->freelist->next;
		}
	}
	while(curr->next!= NULL){ //find the first freeblock then update its info
		if(size <= curr->next->length){

			os = curr->next->offset;
		    curr->next->offset += size;
		    curr->next->length -= size;

		    if(curr->next->length == 0){
			curr->next = curr->next->next;
		    }

		    return os;

		}else{
			curr->next= curr->next->next;
		}

		
	}

    return -1;
}

