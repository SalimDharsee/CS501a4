/*
 * Salim Dharsee
 * ID: 10062458
 * Time Based Convolution
 */

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "TimeBased.h"

using namespace std;

int size;
char *chunkID;
int chunkSize;
char *formatArr;
char *SC1ID;
int SC1Size;
int16_t aFormat;
int16_t channelNum;
int SRate;
int BRate;
int16_t BAlign;
int16_t bPS;
char *SC2ID;
int wavSize;
short* wavData;




/*
 * Please not that much of this code was taken from the online files that were given to us by our TAS and Professor Manzara
 * Specifically the files Main.c and Testtone.c where aided in reading and writing the wave files
 * Any similiarties in code should be assumed to be from this
 */
void timebased(char *File1, char *File2, char *File3)
{
	// While i was testing, it would seem to ignore my prints until the very end, so if the program is run just wait until the convolution is complete

	printf("Time Based\n");

	// getting the command line arguments
	char *DryWave = File1; // dry wave file
	char *IRWave = File2; // impulse response
	char *createdWav = File3; // the output wave file
	// initializing the variables for the file size
	int inputFileSignalSize;
	int IRFileSignalSize;

	// Reading the data of the dry wave file
	float* inputFileSignal = readData(DryWave, inputFileSignal, &size);
	inputFileSignalSize = size;

	// Reading the data of the Impulse response wave file
	float* IRFileSignal = readData(IRWave, IRFileSignal, &size);
	IRFileSignalSize = size;

	// Setting the size for the output signal
	int outputFileSignalSize = inputFileSignalSize + IRFileSignalSize - 1;
	// Initialiting the array for the new signal
	float *outputFileSignal = new float[outputFileSignalSize];
	// The convolution method given to use in class

	timeBasedConvolution(inputFileSignal, inputFileSignalSize, IRFileSignal, IRFileSignalSize, outputFileSignal, outputFileSignalSize);


	// Scaling the wave back
	wavScale(outputFileSignal, outputFileSignalSize);

	// Writing the wave file
	Wave_Write(createdWav, outputFileSignalSize, outputFileSignal);

	printf("Output Wave Created: %s\n", createdWav);

}

// This is the method that is responsible for scaling the signal file,
// The code was aided with the lectures notes in class along with tutorial notes
void wavScale(float signal[], int samples)
{
	printf("Scaling signal file\n");
	float min = 0, max = 0;
	int i = 0;

	for(i = 0; i < samples; i++)
	{
		if(signal[i] > max)
			max = signal[i];
		if(signal[i] < min)
			min = signal[i];
	}

	min *= -1;
	if(min > max)
		max = min;
	for(i = 0; i < samples; i++)
	{
		signal[i] = signal[i] / max;
	}
}

// This code was given to use in class
void timeBasedConvolution(float x[], int N, float h[], int M, float y[], int P) {
	int n, m;

	for (n = 0; n < P; n++)
		y[n] = 0.0;

	float percent = 0.0;
	for (n = 0; n < N; n++) {
		for (m = 0; m < M; m++)
		{
			y[n+m] += x[n] * h[m];
		}
	}

}

// This method was heavily influenced by main.c and testtone.c
// This method is responsible for write the wav file into the specified wav file given via command line
void Wave_Write(char *fileName, int numSamples, float *signal)
{
	// opening the stream, setting parameters for the read
	ofstream outFile( fileName, ios::out | ios::binary);

	printf("Writing file: %s\n", fileName);
	// getting the bytes for hte data chunk
	int chunkSize = channelNum * numSamples * (bPS / 8);
	// Checking the Riff header and setting its componenets
	chunkID = "RIFF";
	outFile.write( chunkID, 4);
	outFile.write( (char*) &chunkSize, 4);
	// CHhecking the wave header and settings its components
	formatArr = "WAVE";
	outFile.write( formatArr, 4);
	outFile.write( SC1ID, 4);
	SC1Size = 16;
	outFile.write( (char*) &SC1Size, 4);
	outFile.write( (char*) &aFormat, 2);
	outFile.write( (char*) &channelNum, 2);
	outFile.write( (char*) &SRate, 4);
	outFile.write( (char*) &BRate, 4);
	outFile.write( (char*) &BAlign, 2);
	outFile.write( (char*) &bPS, 2);

	// sub chunk id 2
	outFile.write( SC2ID, 4);

	wavSize = numSamples * 2;
	outFile.write( (char*)&wavSize, 4);

	// Seting the wave data, and scaling back up to its orignal size
	int16_t val;
	for(int i = 0; i < numSamples; i++)
	{
		val = (int16_t)(signal[i] * (32767));
		outFile.write((char*)&val, 2);
	}
	// closing the file
	outFile.close();
}


float* readData(char *fileName, float *signal, int *Thesize)
{
	// opening the input stream
	ifstream inputFile( fileName, ios::in | ios::binary);

	printf("Reading file: %s\n", fileName);
	// setting the stream to start at the beginning
	inputFile.seekg(ios::beg);
	// getting all of hte header data from the wave file
	chunkID = new char[4];
	inputFile.read( chunkID, 4);
	inputFile.read( (char*) &chunkSize, 4);
	formatArr = new char[4];
	inputFile.read( formatArr, 4);
	SC1ID = new char[4];
	inputFile.read( SC1ID, 4);
	inputFile.read( (char*) &SC1Size, 4);
	inputFile.read( (char*) &aFormat, 2);
	inputFile.read( (char*) &channelNum, 2);
	inputFile.read( (char*) &SRate, 4);
	inputFile.read( (char*) &BRate, 4);
	inputFile.read( (char*) &BAlign, 2);
	inputFile.read( (char*) &bPS, 2);

	// garbage collection, if needed
	if(SC1Size == 18)
	{
		char *garbage;
		garbage = new char[2];
		inputFile.read( garbage, 2);
	}
	// Sub chunk id 2, for the wave data chunk
	SC2ID = new char[4];
	inputFile.read( SC2ID, 4);

	//Data size
	inputFile.read( (char*)&wavSize, 4);

	*Thesize = wavSize / 2;
	int size = wavSize / 2;
	wavData = new short[size];
	// Reading the wav data
	for(int j = 0 ; j < size; j++)
	{
		inputFile.read((char*) &wavData[j], 2);
	}
	printf("\nDone reading...now producing signal\n");

	// Creating the signal that we will use for the convolution
	// This method was influenced by our lecture notes

	short val;
	signal = new float[size];
	for(int i = 0; i < size; i++)
	{
		val = wavData[i];
		signal[i] = (val * 1.0) / (32767);
		if(signal[i] < -1.0)
			signal[i] = -1.0;
	}
	inputFile.close();

	return signal;
}


