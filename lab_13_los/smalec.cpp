#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include "tbb/blocked_range.h"
#include "tbb/mutex.h"
#include "tbb/parallel_scan.h"
#include "tbb/tbb_stddef.h"

using namespace std;

const int size = 500;
char pixels[size][size][3];
tbb::mutex consoleWrite;

void Fill(){
  int i,j,x;

  for(i=0; i<size; i++)
  {
    for(j=0; j<size; j++)
    {
      pixels[i][j][0] = 60;
      pixels[i][j][1] = 60;
      pixels[i][j][2] = 60;
    }
  }

  srand(time(NULL));

  for(j=0; j<size; j++)
  {
    x = rand()%(size/2)+size/4;

    for(i=size; i>x; i--)
    {
      pixels[i][j][0] = 0;
      pixels[i][j][1] = 0;
      pixels[i][j][2] = 0;
    }
  }

}

void FillAltitudes(vector<double> &altitude, vector<bool> &is_visible)
{
	altitude.push_back(size);
	for(int i = size - 1; i >= 0; i--)
	{
		is_visible.push_back(true);
		for(int j = size - 1; j >= 0; j--)
		{
			if(pixels[j][i][0] != 0)
			{
				altitude.push_back(size - j);
				break;
			}
		}
	}
}

void ColorAltitudes(vector<double> &altitude, vector<bool> &is_visible)
{
	for(int i = 0; i < is_visible.size(); i++)
	{
		if(is_visible[i])
		{
			for(int j = size - 1; j >= size - altitude[i+1]; j--)
			{
				pixels[j][size-1-i][0] = 0;
				pixels[j][size-1-i][1] = 255;
				pixels[j][size-1-i][2] = 0;
			}
		}
		else
		{
			for(int j = size - 1; j >= size-altitude[i+1]; j--)
			{
				pixels[j][size-1-i][0] = 255;
				pixels[j][size-1-i][1] = 0;
				pixels[j][size-1-i][2] = 0;
			}
		}
	}	
}

double LineOfSight(const vector<double> &altitude, vector<bool> &is_visible, double dx)
{
  const int N = altitude.size();

  double max_angle = atan2(dx, altitude[0] - altitude[1]);

  return tbb::parallel_scan(
    tbb::blocked_range<int>(1, N), /* range */
    0.0, /* identity */
    /* scan body */
    [&altitude, &is_visible, dx](const tbb::blocked_range<int>&r, double max_angle, bool is_final_scan)->double
    {
      for(int i = r.begin(); i != r.end(); i++)
      {
        double my_angle = atan2(i * dx, altitude[0] - altitude[i]);
        if(my_angle >= max_angle)
          max_angle = my_angle;
        if(is_final_scan && my_angle < max_angle)
          is_visible[i - 1] = false;
      }

      consoleWrite.lock();
      cout << "Thread(" << tbb::task_arena::current_thread_index() << ") "
        << "\tPackage:\t" << r.begin() << "|" << r.end() << "\tMax: " << max_angle;
      if(is_final_scan)
      {
        cout << "\tfinal-scan" << endl;
      }
      else
      {
        cout << "\tpre-scan" << endl;
      }
      consoleWrite.unlock();

      return max_angle;
    },
    /* combine */
    [](double a, double b)
    {
      return max(a,b);
    });
    
}

int main() {

  FILE * fp;
  char *filename="tbb_los.ppm";
  char *comment="# ";/* comment should start with # */
  const int MaxColorComponentValue=255;

  fp= fopen(filename,"wb"); /* b -  binary mode */
  /*write ASCII header to the file*/

  // altitudes got one more item than is_visible because altitude[0] is a light point
  vector<double> altitudes;
	vector<bool> is_visible;

  Fill();
  FillAltitudes(altitudes, is_visible);
  tbb::task_scheduler_init init(2);
  LineOfSight(altitudes, is_visible, 1);
  ColorAltitudes(altitudes, is_visible);


  fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,size,size,MaxColorComponentValue);
  fwrite(pixels,1,3*size*size,fp);

  fclose(fp);
  cout << TBB_INTERFACE_VERSION << endl;

   return 0;
}


// Promień świata i parallel scan. Przykład omawiany na poprzednim przykładzie. Dla 2 wątków.
// Przykład wygenerowania słupków w PPM w kodzie załączonym.
// Źródło światła w prawym górnym ekranu.

// Wykonujemy pre-scan i final-scan zaczynając od paczek z prawej strony (ich wielkość dowolna).
// Program wypisuje konsoli etapy: wątek nr 1 lub 2, paczka, maksimu, pre-scan lub final, 
// Słupki ich górne piksele malujemy (lub je całe) na czerwono gdy nieoświtlone tg mniejszy, zasłonięte, na zielono w przeciwnym razie. Obejrzyj przykład w podręczniku. Kolorujemy słupki, nie rysujemy prostych do źródła światlła.

// Zapisujemy w ppm i wraz z kodem zdajemy program.