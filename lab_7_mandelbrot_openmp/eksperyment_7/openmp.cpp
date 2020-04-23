/* 
 Karol Dzialowski
 39259
 grupa 2B

 c program:
 --------------------------------
  1. draws Mandelbrot set for Fc(z)=z*z +c
  using Mandelbrot algorithm ( boolean escape time )
 -------------------------------         
 2. technique of creating ppm file is  based on the code of Claudio Rocchini
 http://en.wikipedia.org/wiki/Image:Color_complex_plot.jpg
 create 24 bit color graphic file ,  portable pixmap file = PPM 
 see http://en.wikipedia.org/wiki/Portable_pixmap
 to see the file use external application ( graphic viewer)
  */

#include <stdio.h>
#include <math.h>
#include <thread>
#include <vector>
#include <iostream>
#include <omp.h>


const int IterationMax = 200;
/* bail-out value , radius of circle ;  */
const double EscapeRadius = 2;
double ER2 = EscapeRadius * EscapeRadius;

// colors declared for maximum count of 4 threads
// add more if necessary
unsigned char colorTheme[][3] = {{220, 230, 255}, {180, 190, 23}, {42, 129, 84}, {200, 10, 30}, {49, 23, 95}, {120, 90, 32}, {220, 220, 40}, {90, 255, 30}, {30, 30, 225}, {128, 190, 48}};

void mainRunner(int threadsCount, int imageSize) {
  /* screen ( integer) coordinate */
  int iXmax = imageSize;
  int iYmax = imageSize;
  /* world ( double) coordinate = parameter plane*/
  double CxMin = -2.5;
  double CxMax = 1.5;
  double CyMin = -2.0;
  double CyMax = 2.0;
  /* */
  double PixelWidth = (CxMax - CxMin) / iXmax;
  double PixelHeight = (CyMax - CyMin) / iYmax;
  unsigned char*** color = new unsigned char**[iXmax];
  for (int i=0; i<iXmax; i++) {
    color[i] = new unsigned char*[iYmax];
    for (int j=0; j<iYmax; j++) {
      color[i][j] = new unsigned char[3];
    }
  }

   /* color component ( R or G or B) is coded from 0 to 255 */
   /* it is 24 bit color RGB file */
   const int MaxColorComponentValue = 255;
   FILE *fp;
   std::string fileName = "thread" + std::to_string(threadsCount) + "_" + std::to_string(iXmax) + ".ppm";
   const char *filename = fileName.c_str(); 
   char *comment = "# "; /* comment should start with # */
   /*create new file,give it a name and open it in binary mode  */
   fp = fopen(filename, "wb"); /* b -  binary mode */
   /*write ASCII header to the file*/
   fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmax, iYmax, MaxColorComponentValue);

   // Prywatne zmienne używane w wątkach
   int iX, iY;
   double Cx, Cy;
   double Zx, Zy;
   double Zx2, Zy2; 
   int Iteration;
   int threadNumber;
   // Koniec prywatnych zmiennych
   int *iterationCount = new int[threadsCount];
   for (int i=0; i<threadsCount; i++) {
    iterationCount[i] = 0;
   }

   auto start = omp_get_wtime();

   #pragma omp parallel private(threadNumber) shared(color, iterationCount) num_threads(threadsCount)
   {
     threadNumber = omp_get_thread_num();
     #pragma omp for private(iX, iY, Cx, Cy, Zx, Zy, Zx2, Zy2, Iteration) schedule(runtime)
     /* compute and write image data bytes to the file*/
     for (iY = 0; iY < iYmax; iY++) {
        Cy = CyMin + iY * PixelHeight;
        for (iX = 0; iX < iXmax; iX++) {
           Cx = CxMin + iX * PixelWidth;
           /* initial value of orbit = critical point Z= 0 */
           Zx = 0.0;
           Zy = 0.0;
           Zx2 = Zx * Zx;
           Zy2 = Zy * Zy;
           for (Iteration = 0; Iteration < IterationMax && ((Zx2 + Zy2) < ER2); Iteration++) {
              iterationCount[threadNumber]++;
              Zy = 2 * Zx * Zy + Cy;
              Zx = Zx2 - Zy2 + Cx;
              Zx2 = Zx * Zx;
              Zy2 = Zy * Zy;
           };
           /* compute  pixel color (24 bit = 3 bytes) */
           if (Iteration == IterationMax) { /*  interior of Mandelbrot set = black */
              color[iX][iY][0] = 0;
              color[iX][iY][1] = 0;
              color[iX][iY][2] = 0;
           }
           else {
              /* exterior of Mandelbrot set = white */
              color[iX][iY][0] = colorTheme[threadNumber][0]; /* Red*/
              color[iX][iY][1] = colorTheme[threadNumber][1]; /* Green */
              color[iX][iY][2] = colorTheme[threadNumber][2]; /* Blue */
           };
        }
     }
   }
   auto end = omp_get_wtime();
   auto elapsedTime = end - start; 
   std::cout << elapsedTime << std::endl;
   std::cout << "Iterations count:" << std::endl;
   for (int i=0; i<threadsCount; i++) {
    std::cout << iterationCount[i] << ", ";
   }
   std::cout << std::endl;

   /*write color to the file*/
   for (int iY = 0; iY < iYmax; iY++)
   {
      for (int iX = 0; iX < iXmax; iX++)
      {
         fwrite(color[iX][iY], 1, 3, fp);
      }
   }
   fclose(fp);

   /*deallocation*/
   for (int i=0; i<iXmax; i++) {
    for (int j=0; j<iYmax; j++) {
      delete[] color[i][j];
    }
    delete[] color[i];
   }
   delete[] color;

   delete[] iterationCount;

}

int main() {
   const int imageSizes[] = {800, 1600, 3200, 6400};
   for (int j=0; j<4; j++) {
     std::cout << "Image size " << imageSizes[j] << std::endl;
     for (int i=1; i<9; i++) {
        std::cout << "Threads: " << i << std::endl;
        mainRunner(i, imageSizes[j]);
     } 
     std::cout << std::endl;
   }
}
