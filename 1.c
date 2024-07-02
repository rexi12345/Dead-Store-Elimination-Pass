#include <stdio.h>

int main() {
    int x = 12, y = 53; //x is dead y is dead

    x = 204; //this x is not dead
    int z;
    z = x;
    z = 12;
    x = 42; //this one is not dead bcs its used in pf
    y = 12;

    printf("%d\n", x + y);

    return 0;
}