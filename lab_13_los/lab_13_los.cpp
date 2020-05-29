#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int size = 500;
char pixels[size][size][3];

void Fill() {
  int i, j, x;

  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++) {
      pixels[i][j][0] = 100;
      pixels[i][j][1] = 100;
      pixels[i][j][2] = 100;
    }

  srand(time(NULL));

  for (j = 0; j < size; j++) {
    x = rand() % (size / 2) + size / 4;

    for (i = size; i > x; i--) {
      pixels[i][j][0] = 0;
      pixels[i][j][1] = 0;
      pixels[i][j][2] = 0;
    }
  }
}

int main() {
  FILE *fp;
  int i, j;
  char *filename = "los.ppm";
  char *comment = "# "; /* comment should start with # */
  const int MaxColorComponentValue = 255;
  int a;

  fp = fopen(filename, "wb"); /* b -  binary mode */
  /*write ASCII header to the file*/

  fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, size, size,
          MaxColorComponentValue);
  Fill();
  fwrite(pixels, 1, 3 * size * size, fp);
  fclose(fp);

  return 0;
}
