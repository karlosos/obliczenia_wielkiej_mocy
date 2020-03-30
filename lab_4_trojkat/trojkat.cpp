// Karol Dzialowski
// 39259
// grupa 22B

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <thread>

const int triangleHeight = 1024;
const int levelLimit = 7;

unsigned char image[triangleHeight][triangleHeight][3];

void draw_triangle(int x, int y, int level)
{
    int length = triangleHeight / pow(2, level);
    for (int i = y; i < y + length; i++)
    {
        for (int j = x; j < x + length; j++)
        {
            if (i - y == j - x || i == y + length - 1 || j == x)
            {
                image[i][j][0] = 255;
                image[i][j][1] = 255;
                image[i][j][2] = 255;
            }
        }
    }
    if (level < levelLimit)
    {
        std::thread triangles[3];
        level++;
        triangles[0] = std::thread(draw_triangle, x, y, level);
        triangles[1] = std::thread(draw_triangle, x, y + length / 2, level);
        triangles[2] = std::thread(draw_triangle, x + length / 2, y + length / 2, level);
        triangles[0].join();
        triangles[1].join();
        triangles[2].join();
    }
}

int main()
{
    FILE *fp;
    char *filename = "new1.ppm";
    char *comment = "# ";
    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, triangleHeight, triangleHeight, 255);

    draw_triangle(0, 0, 0);

    fwrite(image, 1, 3 * triangleHeight * triangleHeight, fp);
    fclose(fp);
    return 0;
}