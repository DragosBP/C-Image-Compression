#include <stdio.h>

int main() {
    unsigned char c;
    unsigned int n;
    FILE* temp = fopen("test.bin", "rb");
    fread(&n, sizeof(unsigned int), 1, temp);
    printf("%u\n", n);
    while (c != '\n') {
        fread(&c, sizeof(unsigned char), 1, temp);
        printf("%d ", c);
        // c='\n';
    }
    printf("\n");
}