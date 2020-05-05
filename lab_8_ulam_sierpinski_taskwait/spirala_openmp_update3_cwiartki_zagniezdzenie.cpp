// Karol Dzialowski 39259 2B
#include <thread>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <omp.h>
#include <vector>

const int imageWidth = 601;
unsigned char image[imageWidth][imageWidth][3];
unsigned char colorTheme[][3] = {{220, 230, 255}, {180, 190, 23}, {42, 129, 84}, {200, 10, 30}, {49, 23, 95}, {120, 90, 32}, {220, 220, 40}, {90, 255, 30}, {30, 30, 225}, {128, 190, 48}};

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
    omp_set_nested(1);
    FILE *fp;
    char *filename = "new1.ppm";
    char *comment = "# ";
    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, imageWidth, imageWidth, 255);

    // CZWARTY FEEDBACK:
    // ile wątków bierze udział
    // glowna petla powinna byc podzielona na 2 watki ktore uruchamiaja kolejne 2, parallel for ma byc zagniezdzone
    // o to chodzi w tym zadaniu
    //
    // ---
    // Moja odpowiedz:
    // Teraz są:
    // - ćwiartki
    // - określona liczba wątków
    // - zagnieżdżone parallel for bez użycia collapse jak wcześniej
    //
    // Mam nadzieję że o to Panu ostatecznie chodziło. 
    //
    // Wcześniej używałem:
    // #pragma omp parallel for num_threads(4) collapse(2)
    // Korzystanie z collapse(2) wynikało z niezrozumienia zadania. 
    // Myślałem że polecenie - zagnieżdżone parallel for to chodziło o użycie klauzuli collapse.
    // Znalazłem w sieci przykłady z klauzulą collapse() dla zagnieżdżonych pętli prostokątnych. 
    // Stad wcześniejsze użycie takiej konstrukcji.
    //
    // Jeżeli będzie pan innym zwracać program z i będą też używali klauzuli collapse() proszę żeby 
    // wspominał Pan o tym jawnie w feedbacku. Wtedy nie będzie takich nieporozumień jak u mnie.

    int threadNumber = 0;
    #pragma omp parallel for num_threads(2)
    for (int i = 0; i < imageWidth; i++)
    {
        #pragma omp parallel for private(threadNumber) num_threads(2)
        for (int j = 0; j < imageWidth; j++)
        {
            threadNumber = omp_get_thread_num();
            bool isCelPrime = isprime(ulam_get_map(i, j, imageWidth));
            if (isCelPrime)
            {
                image[i][j][0] = 255;
                image[i][j][1] = 255;
                image[i][j][2] = 255;
            }
            else
            {
                image[i][j][0] = colorTheme[threadNumber][0];
                image[i][j][1] = colorTheme[threadNumber][1];
                image[i][j][2] = colorTheme[threadNumber][2];
            }
        }
    }

    fwrite(image, 1, 3 * imageWidth * imageWidth, fp);
    fclose(fp);
    return 0;
}
