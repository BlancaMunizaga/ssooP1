#include <stdio.h>
int main (int argc, char** argv)
{

    int pos = 8388736;
              
    unsigned char bytes[3];
    
    bytes[2] = pos & 0xff; //contains Lsb
    bytes[1] = (pos >> 8) & 0xff;
    bytes[0] = (pos >> 16) & 0xff;

    printf("Bytes: %u, %u, %u", bytes[0], bytes[1], bytes[2]);

  return 0;
}


