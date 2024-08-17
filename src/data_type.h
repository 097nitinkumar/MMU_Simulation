#ifndef data_type
#include <stdint.h>

#define KB (1024)
#define MB (1024 * 1024)
#define GB (1024 * 1024 * 1024)
#define PAS_SIZE (2 * MB) // 2 GB of RAM (Physical address space)
#define PAGE_SIZE (4 * KB)
#define FRAME_SIZE (4 * KB)
#define STATUS uint8_t
#define UINT8 uint8_t
#define FLAG uint8_t
#define UINT32 uint32_t
#define OK 0
#define NO_MEMORY 1
#define OUT_OF_RANGE 2
#define False 0
#define True 1
#define ERR stderr
STATUS getGranuality(UINT32 *pgranuality) {
    *pgranuality=PAGE_SIZE;
    return OK;
}

STATUS getPasSize(UINT32 *ppas_size) {
    *ppas_size=PAS_SIZE;
    return OK;
}
#define data_type
#endif //data_type.h
