#include <iostream>
#include <stdio.h>
#include <math.h>
#include <thread>

const int triangleHeight = 1024;
const int levelLimit = 8;

unsigned char image[triangleHeight][triangleHeight][3];

void draw_triangle(int x, int y, int level) {
    for (int i=0; i<triangleHeight; i++) {
        for (int j=0; j<triangleHeight; j++) {
            if (i == j || i==triangleHeight-1 || j==0) {
                image[i][j][0] = 120;
                image[i][j][1] = 120;
                image[i][j][2] = 120;
            }
        }
    }
}

int main()
{
    FILE *fp;
    char *filename = "new1.ppm";
    char *comment = "# ";
    fp = fopen(filename, "wb"); /* b -  binary mode */
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, triangleHeight, triangleHeight, 255);

    draw_triangle(0, 0, 0);

    fwrite(image, 1, 3*triangleHeight*triangleHeight, fp);
}