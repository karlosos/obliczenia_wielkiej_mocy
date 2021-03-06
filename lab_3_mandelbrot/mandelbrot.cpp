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


/* screen ( integer) coordinate */
const int iXmax = 800;
const int iYmax = 800;
/* world ( double) coordinate = parameter plane*/
const double CxMin = -2.5;
const double CxMax = 1.5;
const double CyMin = -2.0;
const double CyMax = 2.0;
/* */
double PixelWidth = (CxMax - CxMin) / iXmax;
double PixelHeight = (CyMax - CyMin) / iYmax;

const int IterationMax = 200;
/* bail-out value , radius of circle ;  */
const double EscapeRadius = 2;
double ER2 = EscapeRadius * EscapeRadius;
unsigned char color[iXmax][iYmax][3]; // output bitmap

// colors declared for maximum count of 4 threads
// add more if necessary
unsigned char colorTheme[][3] = {{220, 230, 255}, {180, 190, 23}, {42, 129, 84}, {200, 10, 30}, {49, 23, 95}};

void drawFractal(int startBound, int endBound, int threadNumber) {
   int iX, iY;
   double Cx, Cy;
   /* Z=Zx+Zy*i  ;   Z0 = 0 */
   double Zx, Zy;
   double Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
   int Iteration;

   /* compute and write image data bytes to the file*/
   for (iY = startBound; iY < endBound; iY++) {
      Cy = CyMin + iY * PixelHeight;
      if (fabs(Cy) < PixelHeight / 2)
         Cy = 0.0; /* Main antenna */
      for (iX = 0; iX < iXmax; iX++) {
         Cx = CxMin + iX * PixelWidth;
         /* initial value of orbit = critical point Z= 0 */
         Zx = 0.0;
         Zy = 0.0;
         Zx2 = Zx * Zx;
         Zy2 = Zy * Zy;
         for (Iteration = 0; Iteration < IterationMax && ((Zx2 + Zy2) < ER2); Iteration++) {
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

void mainThread(int threadsCount) {
   /* color component ( R or G or B) is coded from 0 to 255 */
   /* it is 24 bit color RGB file */
   const int MaxColorComponentValue = 255;
   FILE *fp;
   std::string fileName = "thead" + std::to_string(threadsCount) + ".ppm";
   const char *filename = fileName.c_str(); 
   char *comment = "# "; /* comment should start with # */
   /*create new file,give it a name and open it in binary mode  */
   fp = fopen(filename, "wb"); /* b -  binary mode */
   /*write ASCII header to the file*/
   fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmax, iYmax, MaxColorComponentValue);

   auto start = std::chrono::steady_clock::now();  
   /* compute image data bytes */
   std::thread threads[threadsCount];

   for (int i = 0; i < threadsCount; i++)
   {
      threads[i] = std::thread(drawFractal, i * (iYmax / threadsCount), (i + 1) * (iYmax / threadsCount), i);
   }

   for (int i = 0; i < threadsCount; i++)
   {
      threads[i].join();
   }
   
   auto end = std::chrono::steady_clock::now();
   auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
   std::cout << elapsedTime << std::endl;

   /*write color to the file*/
   for (int iY = 0; iY < iYmax; iY++)
   {
      for (int iX = 0; iX < iXmax; iX++)
      {
         fwrite(color[iX][iY], 1, 3, fp);
      }
   }
   fclose(fp);
}

int main() {
   std::cout << "Image size " << iXmax << std::endl;
   for (int i=1; i<5; i++) {
      std::cout << "Threads: " << i << "\t \t ";
      mainThread(i);
   } 
}
