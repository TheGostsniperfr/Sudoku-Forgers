#include <stdio.h>

void print_progress(double count, double max) {
    const int bar_width = 50;

    float progress = (float) count / max;
    int bar_length = progress * bar_width;

    printf("\rProgress: [");
    for (int i = 0; i < bar_length-1; ++i) {
        printf("=");
    }
    printf(">");
    for (int i = bar_length; i < bar_width-1; ++i) {
        printf(" ");
    }
    printf("] %.2f%%", progress * 100);

    fflush(stdout);
}

int main() {
    double n = 0;
    double maxN = 276440;

    while (n != maxN) {
        print_progress(n, maxN);
        n++;
    }
    
    printf("\n");

    return 0;
}
