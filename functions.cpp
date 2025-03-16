#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void read_distribution(const char *filename, double distribution[26]) {
    FILE* file = fopen(filename, "r");
    int i =0;
    if (file != NULL) {
        while (fscanf(file, "%f", &distribution[i]) != EOF)
        {
            i++;
        }
        fclose(file);

        distribution[i] = '\0';
        printf("%f\n", distribution[i]);
        fclose(file);
    }
}
