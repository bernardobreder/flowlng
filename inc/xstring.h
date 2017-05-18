
#include "platform.h"

#define strcmp1(LEFT, RIGHT) (LEFT[0] == RIGHT[0])
#define strcmp2(LEFT, RIGHT) (strcmp1(LEFT, RIGHT) && LEFT[1] == RIGHT[1])
#define strcmp3(LEFT, RIGHT) (strcmp2(LEFT, RIGHT) && LEFT[2] == RIGHT[2])
#define strcmp4(LEFT, RIGHT) (strcmp3(LEFT, RIGHT) && LEFT[3] == RIGHT[3])
#define strcmp5(LEFT, RIGHT) (strcmp4(LEFT, RIGHT) && LEFT[4] == RIGHT[4])
#define strcmp6(LEFT, RIGHT) (strcmp5(LEFT, RIGHT) && LEFT[5] == RIGHT[5])
#define strcmp7(LEFT, RIGHT) (strcmp6(LEFT, RIGHT) && LEFT[6] == RIGHT[6])
#define strcmp8(LEFT, RIGHT) (strcmp7(LEFT, RIGHT) && LEFT[7] == RIGHT[7])
#define strcmp9(LEFT, RIGHT) (strcmp8(LEFT, RIGHT) && LEFT[8] == RIGHT[8])
#define strcmp10(LEFT, RIGHT) (strcmp9(LEFT, RIGHT) && LEFT[9] == RIGHT[9])
#define strcmp11(LEFT, RIGHT) (strcmp10(LEFT, RIGHT) && LEFT[10] == RIGHT[10])
#define strcmp12(LEFT, RIGHT) (strcmp11(LEFT, RIGHT) && LEFT[11] == RIGHT[11])

js_hash js_hash_perform(char* text, size_t length);
