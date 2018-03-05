#include <stdio.h>
#include "rngs.h"
int main() {
    PlantSeeds(-1); // -1 -> Use system time
    FILE *fp;
    fp = fopen("randomoutput.txt", "w");
    if (fp != NULL)
        for (int i = 0; i < 100000; ++i) {
            printf("%f\n", Random());
            fprintf(fp, "%f\n", Random());
        }
    fclose(fp);
    return 0;
}
