#include "rand.h"

static int random = 0;

void seed(int s){
    random = s;
}

int rand(){
    random = 181 * random + 356;
    return random;
}
