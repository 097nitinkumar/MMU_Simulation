#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "data_type.h"

UINT32     RAM_SIZE = 0;
UINT32     NUM_FRAMES = 0;
UINT32    *pframe_bitmap = NULL;

/*
Problem Statement
 1) Allocate some memory and assume it to be RAM memory (Physical Address Space)
 2) The size of PAS will be in multiple of Frame Size.
 3) The Bit map will be used to track which Frames are occupied.
*/

STATUS initBitMap();
// Function to allocate memory that acts as RAM
STATUS initializeRAM(UINT8 **ppram, UINT32 *ppas_size) {
    NUM_FRAMES = (*ppas_size/FRAME_SIZE);
    RAM_SIZE = (NUM_FRAMES)*FRAME_SIZE;
    *ppram = (UINT8 *)malloc(RAM_SIZE*sizeof(UINT8));
    // type case to 1 byte to read one byte at once
    // 4096 such reads accounts for 1 page, since it is type casted
    // to 1 byte the type of ppram is also made UINT8

    if (*ppram == NULL) {
        fprintf(ERR, "Failed to allocate %d of memory!\n", RAM_SIZE);
        return NO_MEMORY;
    }
    printf("%d of RAM requested.\n", *ppas_size);
    *ppas_size = RAM_SIZE;
    printf("%d of RAM successfully allocated.\n", *ppas_size);
    initBitMap();
    return OK;
}

// Creating a bitmap which holds the information which page is occupied
// also note 4096 bytes read accounts for 1 Bytes.
// The data structure is 1D-Array where each integer keeps information of 32 pages
STATUS initBitMap() {
    pframe_bitmap=(UINT32 *)malloc(sizeof(UINT32)*NUM_FRAMES/32);
    if (pframe_bitmap == NULL) { 
        fprintf(ERR, "Failed to allocate memory for bitmap !\n");
        return NO_MEMORY;
    }
    memset(pframe_bitmap, 0, sizeof(*pframe_bitmap));
    return OK;
}

//Marks a Frame to be occupied
STATUS setFrameOccupied(UINT32 frame_num) {
    pframe_bitmap[frame_num / 32] |= (1 << (frame_num % 32));
    return OK;
}

STATUS setFrameFree(UINT32 frame_num) {
    pframe_bitmap[frame_num / 32] &= ~(1 << (frame_num % 32));
    return OK;
}

STATUS isFrameOccupied(UINT32 frame_num, FLAG *pfound) {
    *pfound = (FLAG) pframe_bitmap[frame_num / 32] & (1 << (frame_num % 32));
    return OK;
}

STATUS findFreeFrame(UINT32 *ppageNumber) {
    FLAG occupied=True;
    for (UINT32 i = 0; i < NUM_FRAMES; i++) {
        isFrameOccupied(i, &occupied);
        if (occupied == False) {
            *ppageNumber=i;
            return OK;
        }
    }
    return NO_MEMORY; // No free page found
}

// Function to simulate writing to memory
STATUS writeToRAM(UINT8 *pram, UINT32 pas_size, UINT32 frame_size) {
    for (size_t i = 0, j = 0; i < pas_size; i += (frame_size), j++) {
        if (i >= pas_size) {
            fprintf(ERR, "Index out of bounds: %zu\n", i);
            return OUT_OF_RANGE;
        }
        pram[i] = (i / frame_size); // Write a value to first byte of every frame
        //since we are writting one byte only, the value in pram[i] can be only [0-255]
        printf("Written to RAM at frame %zu ,at byte %zu: %u\n", j, i, pram[i]);
    }
    return OK;
}

// Function to verify data written to memory
STATUS verifyRAM(UINT8 *pram, UINT32 pas_size, UINT32 frame_size) {
    for (size_t i = 0, j = 0; i < pas_size; i += frame_size, j++) {
        printf("Read from RAM at frame %zu ,at byte %zu: %u\n", j, i, pram[i]);
    }
    return OK;
}

// Function to free the allocated RAM
STATUS freeRAM(UINT8 *pram) {
    free(pram);
    printf("Memory successfully freed.\n");
    return OK;
}
