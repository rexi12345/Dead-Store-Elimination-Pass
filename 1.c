#include <stdio.h>

int main() {
    int x = 12, y = 53; //x is dead y is dead

    x = 204; //this x is not dead bcs its used for z
    int z;
    z = x; //is dead because of next line
    
    z = 12; //is dead because its not used anywhere
    //live 
    x = 42;
    y = 12;

    printf("%d\n", x + y);

    return 0;
}
