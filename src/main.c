#include <stdio.h>
#include "data_type.h"
#include "pas.h"
#include "vas.h"





UINT8 *pram = NULL;
UINT32 granuality = 0L;
UINT32 pas_size = 0;
STATUS main() {
    printf("Hello OS");
    getGranuality(&granuality);
    getPasSize(&pas_size);
    initializeRAM(&pram, &pas_size);
    writeToRAM(pram, pas_size, granuality);
    verifyRAM(pram, pas_size, granuality);
    freeRAM(pram);
    return 0;
}
