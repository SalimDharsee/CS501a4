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
#include <string.h>
#include "TimeBased.h"
#include <math.h>
#include <time.h>
int fsize;
clock_t t1, t2;
#define PI	3.141592653589793
#define TWO_PI	(2.0 * PI)
#define SWAP(a,b)	tempr=(a);(a)=(b);(b)=tempr


int main(int argc, char *argv[]){
	t1 = clock();

	// running frequency based code
	//frequencybased(argv[1], argv[2], argv[3]);
	t2 = clock();
	float diff = (t2 - t1);
	printf("%f",diff);
	return -1;

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

	int padSize = inputFileSignalSize + IRFileSignalSize - 1;
	int totalsize = 1;
	int a = 0;
	while(totalsize < padSize){
		totalsize <<= a;
		a++;
	}
	// pad for the dry wave file
	float *complex = new float [2*totalsize];
	float *input = new float[2*totalsize];
	float *ir = new float [2*totalsize];

	int i, k;
	for(i = 0, k = 0; i < inputFileSignalSize; i++, k+=2)
	{
	    input[k] = inputFileSignal[i];
	    input[k + 1] = 0;
	}
	// pad for the impulse response file
	i = k;
	memset(input + k, 0, (2*totalsize) -1);


	for(i = 0, k = 0; i < IRFileSignalSize; i++, k+=2)
	{
	    ir[k] = IRFileSignal[i];
	    ir[k + 1] = 0;
	}
	i = k;
	memset(ir + k, 0, (2*totalsize) -1);
	memset(complex, 0, (2*totalsize));
	//calling the FFT algorithm with the padded results of the inputfile and the irfile
	fastfouriertransform(input-1, totalsize, 1);
	fastfouriertransform(ir-1, totalsize, 1);
	// the fft algorithm produces a complex number
	calculation(input, ir, complex, totalsize);
	fastfouriertransform(complex-1, totalsize,-1);

	for(i=0; i < totalsize; i++){
		complex[i*2] /= (float)totalsize;
		complex[(i*2)+1] /= (float)totalsize;
	}

	for(i=0, k=0; i < outputFileSignalSize; i++, k+=2){
		outputFileSignal[i]=complex[k];
	}
	// Using the scale function used in timebased.cpp
	wavScale(outputFileSignal,outputFileSignalSize);
	// Using the write function used in timebased.cpp
	Wave_Write(createdWav, outputFileSignalSize, outputFileSignal);

}
// This calculation was created with help of online references along with course documents
void calculation(float wavInput[], float irInput[], float result[], int size){
	int i, temporay = 0;
	for(i = 0; i < size; i++){
		temporay = i * 2;
		result[temporay] = wavInput[temporay] * irInput[temporay] - wavInput[temporay+1] * irInput[temporay+1];
		result[temporay +1] = wavInput[temporay+1] * irInput[temporay] - wavInput[temporay] * irInput[temporay+1];

	}
}
// This was given to us on the handout labeled Fast Fourier Theorem
// The array is filled withe complex numbers and the signal is real
void fastfouriertransform(float data[], int nn, int isign){
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
	    	theta = isign * ((2.0 * PI) / mmax);
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


