/*
 * AlgorithmBased.h
 *
 *  Created on: Dec 7, 2015
 *      Author: Salim Dharsee
 */

#ifndef ALGORITHMBASED_H_
#define ALGORITHMBASED_H_

void fastfouriertransform(float data[], int nn, int isign);
void calculation(float wavInput[], float irInput[], float result[], int size);
void frequencybased(char *File1, char *File2, char *File3);

#endif /* ALGORITHMBASED_H_ */
