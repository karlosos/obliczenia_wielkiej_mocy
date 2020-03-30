//============================================================================
// Name        : Lab4.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <thread>

#define N 1024
#define L 8

using namespace std;

static unsigned char color[N][N][3];

void main_fun(int x, int y, int level)
{
    int size = N / (pow(2, level));
    int x2, y2;
    y2 = y + (size / 2);
    x2 = x + (size / 2);
    int i, j;
    unsigned char rand_color1 = rand() % 255, rand_color2 = rand() % 255, rand_color3 = rand() % 255;
    for (j = y; j < y2; ++j)
    {
        for (i = x; i < x2; ++i)
        {
            if (i == j || i == (x2 - 1) || j == y)
            {
                color[i][j][0] = rand_color1;
                color[i][j][1] = rand_color2;
                color[i][j][2] = rand_color3;
            }
            else
            {
                color[i][j][0] = 0;
                color[i][j][1] = 0;
                color[i][j][2] = 0;
            }
        }
    }
    level++;
    if (level < L)
    {
        thread tab[3];
        tab[0] = thread(main_fun, x, y, level);
        tab[1] = thread(main_fun, x2, y, level);
        tab[2] = thread(main_fun, x2, y2, level);
        tab[0].join();
        tab[1].join();
        tab[2].join();
    }
}

int main()
{
    srand(time(NULL));
    FILE *fp;
    char *filename = "new1.ppm";
    char *comment = "# "; /* comment should start with # */

    main_fun(0, 0, 0);
    /*create new file,give it a name and open it in binary mode  */
    fp = fopen(filename, "wb"); /* b -  binary mode */
    /*write ASCII header to the file*/
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, N, N, 255);

    for (int j = 0; j < N; ++j)
    {
        for (int i = 0; i < N; ++i)
        {
            fwrite(color[j][i], 1, 3, fp);
        }
    }
    fclose(fp);
    return 0;
}
