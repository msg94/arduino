#include <stdio.h>
#include <stdint.h>

void main(){
    for(uint8_t i=0; i<100; i++){
        printf("%02x ",i+1);
    }

}