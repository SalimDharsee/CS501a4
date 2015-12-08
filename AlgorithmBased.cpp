/*
 * AlgorithmBased.cpp
 *
 *  Created on: Dec 7, 2015
 *      Author: Salim Dharsee
 */

#include "AlgorithmBased.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "TimeBased.h"
#include <math.h>
int fsize;
#define PI	3.141592653589793
#define TWO_PI	(2.0 * PI)
#define SWAP(a,b)	tempr=(a);(a)=(b);(b)=tempr
int main(int argc, char *argv[]){
	//remove comments to run TimeBased test
	//timebased(argv[1], argv[2], argv[3]);
	// running frequency based code
	frequencybased(argv[1], argv[2], argv[3]);

}
// The reading and writing portion was taken from my TimeBased.cpp file
void frequencybased(char *File1, char *File2, char *File3){
	printf("Frequency Based\n");
	// getting the command line arguments
	char *DryWave = File1; // dry wave file
	char *IRWave = File2; // impulse response
	char *createdWav = File3; // the output wave file
	// initializing the variables for the file size
	int inputFileSignalSize;
	int IRFileSignalSize;

	// Reading the data of the dry wave file
	float* inputFileSignal = readData(DryWave, inputFileSignal, &fsize);
	inputFileSignalSize = fsize;

	// Reading the data of the Impulse response wave file
	float* IRFileSignal = readData(IRWave, IRFileSignal, &fsize);
	IRFileSignalSize = fsize;

	// Setting the size for the output signal
	int outputFileSignalSize = inputFileSignalSize + IRFileSignalSize - 1;
	// Initialiting the array for the new signal
	float *outputFileSignal = new float[outputFileSignalSize];
	// The convolution method given to use in class
}

// This was given to us on the handout labeled Fast Fourier Theorem
// The array is filled withe complex numbers and the signal is real
void fastfouriertransform(double data[], int nn, int isign){
	 unsigned long n, mmax, m, j, istep, i;
	    float wtemp, wr, wpr, wpi, wi, theta;
	    float tempr, tempi;

	    n = nn << 1;
	    j = 1;

	    for (i = 1; i < n; i += 2)
	    {
	    	if (j > i)
	    	{
	    		SWAP(data[j], data[i]);
	    		SWAP(data[j+1], data[i+1]);
	    	}
	    	m = nn;
	    	while (m >= 2 && j > m)
	    	{
	    		j -= m;
	    		m >>= 1;
	    	}
	    	j += m;
	    }

	    mmax = 2;
	    while (n > mmax)
	    {
	    	istep = mmax << 1;
	    	theta = isign * (6.28318530717959 / mmax);
	    	wtemp = sin(0.5 * theta);
	    	wpr = -2.0 * wtemp * wtemp;
	    	wpi = sin(theta);
	    	wr = 1.0;
	    	wi = 0.0;
	    	for (m = 1; m < mmax; m += 2)
	    	{
	    		for (i = m; i <= n; i += istep)
	    		{
	    			j = i + mmax;
	    			tempr = wr * data[j] - wi * data[j+1];
	    			tempi = wr * data[j+1] + wi * data[j];
	    			data[j] = data[i] - tempr;
	    			data[j+1] = data[i+1] - tempi;
	    			data[i] += tempr;
	    			data[i+1] += tempi;
	    		}
	    		wr = (wtemp = wr) * wpr - wi * wpi + wr;
	    		wi = wi * wpr + wtemp * wpi + wi;
	    	}
	    	mmax = istep;
	    }
}
void createComplexSine(double data[], int size, int harmonicNumber){
	int i, ii;
	for (i = 0, ii = 0; i < size; i++, ii+=2){
		data[ii] = sin((double)harmonicNumber * (double)i *TWO_PI / (double)size);
		data[ii+1] = 0.0;

	}
}
// This was given to us on the handout labeled Fast Fourier Theorem
// The array is filled with complex numbers and the signal is real
void createComplexCosine(double data[], int size, int harmonicNumber ){
	int i, ii;
		for (i = 0, ii = 0; i < size; i++, ii+=2){
			data[ii] = cos((double)harmonicNumber * (double)i *TWO_PI / (double)size);
			data[ii+1] = 0.0;
		}
}

