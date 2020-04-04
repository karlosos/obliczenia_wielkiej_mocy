// Karol Dzialowski 39259 2B
#include <thread>
#include <iostream>
#include <math.h>
#include <algorithm>

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

void drawSpiral(std::pair<int, int> x_bounds, std::pair<int, int> y_bounds, int color)
{
    for (int i = x_bounds.first; i < x_bounds.second; i++)
    {
        for (int j = y_bounds.first; j < y_bounds.second; j++)
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
                image[i][j][0] = color;
                image[i][j][1] = color;
                image[i][j][2] = color;
            }
        }
    }
}

int main()
{
    FILE *fp;
    char *filename = "new1.ppm";
    char *comment = "# ";
    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, imageWidth, imageWidth, 255);

    std::thread spiral_quarter[4];
    auto upper_bound = std::make_pair(0, imageWidth / 2);
    auto lower_bound = std::make_pair(imageWidth / 2, imageWidth);
    auto left_bound = std::make_pair(0, imageWidth / 2);
    auto right_bound = std::make_pair(imageWidth / 2, imageWidth);

    spiral_quarter[0] = std::thread([&] { drawSpiral(upper_bound, left_bound, 0); });
    spiral_quarter[1] = std::thread([&] { drawSpiral(upper_bound, right_bound, 0); });
    spiral_quarter[2] = std::thread([&] { drawSpiral(lower_bound, left_bound, 0); });
    spiral_quarter[3] = std::thread([&] { drawSpiral(lower_bound, right_bound, 0); });

    spiral_quarter[0].join();
    spiral_quarter[1].join();
    spiral_quarter[2].join();
    spiral_quarter[3].join();

    fwrite(image, 1, 3 * imageWidth * imageWidth, fp);
    fclose(fp);
    return 0;
}
