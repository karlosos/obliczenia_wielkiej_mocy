// Karol Dzialowski 39259 2B
#include <iostream>
#include <math.h>
#include <algorithm>
#include <omp.h>

const int imageWidth = 601;
unsigned char image[imageWidth][imageWidth][3];

int ulam_get_map(int x, int y, int n)
{
    x -= (n - 1) / 2;
    y -= n / 2;
    int mx = abs(x), my = abs(y);
    int l = 2 * std::max(mx, my);
    int d = y >= x ? l * 3 + x + y : l - x - y;
    return pow(l - 1, 2) + d;
}

int isprime(int n)
{
    int p;
    for (p = 2; p * p <= n; p++)
        if (n % p == 0)
            return 0;
    return n > 2;
}

int main()
{
    omp_set_num_threads(8);
    FILE *fp;
    char *filename = "new1.ppm";
    char *comment = "# ";
    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, imageWidth, imageWidth, 255);

    // https://www.openmp.org/spec-html/5.0/openmpsu41.html#x64-1290002.9.2
    // Za dokumentacją openmp:
    // The collapse clause may be used to specify how many loops are associated with the worksharing-loop construct. The parameter of the collapse clause must be a constant positive integer expression. If a collapse clause is specified with a parameter value greater than 1, then the iterations of the associated loops to which the clause applies are collapsed into one larger iteration space that is then divided according to the schedule clause. The sequential execution of the iterations in these associated loops determines the order of the iterations in the collapsed iteration space. If no collapse clause is present or its parameter is 1, the only loop that is associated with the worksharing-loop construct for the purposes of determining how the iteration space is divided according to the schedule clause is the one that immediately follows the worksharing-loop directive.  
    #pragma omp parallel for collapse(2) schedule(dynamic) 
    for (int i = 0; i < imageWidth; i++)
    {
        for (int j = 0; j < imageWidth; j++)
        {
            bool isCelPrime = isprime(ulam_get_map(i, j, imageWidth));
            if (isCelPrime)
            {
                image[i][j][0] = 255;
                image[i][j][1] = 255;
                image[i][j][2] = 255;
            }
            else
            {
                image[i][j][0] = 0;
                image[i][j][1] = 0;
                image[i][j][2] = 0;
            }
        }
    }

    fwrite(image, 1, 3 * imageWidth * imageWidth, fp);
    fclose(fp); return 0; }
