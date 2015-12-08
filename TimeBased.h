

#ifndef TIMEBASED_H_
#define TIMEBASED_H_


void convolve(float x[], int N, float h[], int M, float y[], int P);
void wavScale(float signal[], int samples);
void timeBasedConvolution(float x[], int N, float h[], int M, float y[], int P);
void convolve(float x[], int N, float h[], int M, float y[], int P);
void Wave_Write(char *fileName, int numSamples, float *signal);
float* readData(char *fileName, float *signal, int *Thesize);
#endif /* TIMEBASED_H_ */
