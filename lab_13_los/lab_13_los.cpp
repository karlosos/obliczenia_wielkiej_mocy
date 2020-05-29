// Karol Działowski 2B
// Zadanie Line of Sight parallel scan i blocked range
// Na podstawie przykładu z książki IntelTBB strona 57

#include "tbb/blocked_range.h"
#include "tbb/parallel_scan.h"
#include "tbb/task_scheduler_init.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

const int size = 500;
char pixels[size][size][3];

void Fill() {
  int i, j, x;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      pixels[i][j][0] = 20;
      pixels[i][j][1] = 20;
      pixels[i][j][2] = 20;
    }
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

void FillAltitudes(vector<double> &altitude, vector<bool> &is_visible) {
  altitude.push_back(size);

  for (int i = size - 1; i >= 0; i--) {
    is_visible.push_back(true);

    for (int j = size - 1; j >= 0; j--) {
      if (pixels[j][i][0] != 0) {
        altitude.push_back(size - j);
        break;
      }
    }
  }
}

/*
Implementacja analogicznie jak w książce 2019_Book_ProTBB.pdf ze strony 57
*/
void LineOfSight(const vector<double> &altitude,
                    vector<bool> &is_visible, double dx) {
  const int N = altitude.size();

  double max_angle = atan2(dx, altitude[0] - altitude[1]);

  double final_max_angle = tbb::parallel_scan(
      tbb::blocked_range<int>(1, N), 0.0,
      [&altitude, &is_visible, dx](const tbb::blocked_range<int> &r,
                                   double max_angle,
                                   bool is_final_scan) -> double {
        for (int i = r.begin(); i != r.end(); ++i) {
          double my_angle = atan2(i * dx, altitude[0] - altitude[i + 1]);
          if (my_angle >= max_angle) {
            max_angle = my_angle;
          }
          if (is_final_scan && my_angle < max_angle) {
            is_visible[i] = false;
          }
        }
        return max_angle;
      },
      [](double a, double b) { return max(a, b); });
}

void ColorAltitudes(vector<double> &altitude, vector<bool> &is_visible) {
  for (int i = 0; i < is_visible.size(); i++) {
    if (is_visible[i]) {
      for (int j = size - 1; j >= size - altitude[i + 1]; j--) {
        pixels[j][size - 1 - i][0] = 20;
        pixels[j][size - 1 - i][1] = 200;
        pixels[j][size - 1 - i][2] = 45;
      }
    } else {
      for (int j = size - 1; j >= size - altitude[i + 1]; j--) {
        pixels[j][size - 1 - i][0] = 200;
        pixels[j][size - 1 - i][1] = 10;
        pixels[j][size - 1 - i][2] = 30;
      }
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
  vector<double> altitude;
  vector<bool> is_visible;
  fp = fopen(filename, "wb");
  fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, size, size,
          MaxColorComponentValue);
  Fill();
  FillAltitudes(altitude, is_visible);

  tbb::task_scheduler_init init(2);
  LineOfSight(altitude, is_visible, 1);
  ColorAltitudes(altitude, is_visible);

  fwrite(pixels, 1, 3 * size * size, fp);
  fclose(fp);

  return 0;
}
