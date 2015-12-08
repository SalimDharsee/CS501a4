

#ifndef TIMEBASED_H_
#define TIMEBASED_H_
void timebased(char *File1, char *File2, char *File3);
void Wave_Write(char *fileName, int numSamples, float *signal);
float* readData(char *fileName, float *signal, int *Thesize);
void wavScale(float signal[], int samples);
void timeBasedConvolution(float x[], int N, float h[], int M, float y[], int P);

#endif /* TIMEBASED_H_ */
