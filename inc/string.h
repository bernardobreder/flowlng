
#define strcmp1(LEFT, RIGHT) LEFT[0] == RIGHT[0]
#define strcmp2(LEFT, RIGHT) strcmp1(LEFT, RIGHT) && LEFT[1] == RIGHT[1]
#define strcmp3(LEFT, RIGHT) strcmp2(LEFT, RIGHT) && LEFT[2] == RIGHT[2]
#define strcmp4(LEFT, RIGHT) strcmp3(LEFT, RIGHT) && LEFT[3] == RIGHT[3]
#define strcmp5(LEFT, RIGHT) strcmp4(LEFT, RIGHT) && LEFT[4] == RIGHT[4]
#define strcmp6(LEFT, RIGHT) strcmp5(LEFT, RIGHT) && LEFT[5] == RIGHT[5]
#define strcmp7(LEFT, RIGHT) strcmp6(LEFT, RIGHT) && LEFT[6] == RIGHT[6]
#define strcmp8(LEFT, RIGHT) strcmp7(LEFT, RIGHT) && LEFT[7] == RIGHT[7]
#define strcmp9(LEFT, RIGHT) strcmp8(LEFT, RIGHT) && LEFT[8] == RIGHT[8]