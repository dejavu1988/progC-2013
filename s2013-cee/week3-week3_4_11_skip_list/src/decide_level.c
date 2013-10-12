#include <stdlib.h>

int decide_level(int max_level) {
    int k=1;    
    while (rand()%2 != 0){
        k++;
        if(k == max_level){
            return k;
        }
    }      
    return k;
}
