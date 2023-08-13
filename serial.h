/*
 * serial.h
 *
 *  Created on: 7 de mai de 2021
 *      Author: mateu
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <msp430.h>

#define TRUE    1
#define FALSE   0
#define SMCLK   1048576L  //SMCLK

void ser_config(void);
void ser_char(char c);
void ser_str(char palavra[]);
void ser_undec16(unsigned int dec);
void ser_float(float num); // 3 casas decimais TRUNCADO
void ser_dechex(long int decimalNumber);

char ser_lerchar(void);
int ser_charint(char c);

void ser_leresc(void);
void ser_limpaglobal(void);
void ser_globalint2(unsigned int *min, unsigned int *max);
void ser_check(float media, int min, int max);

inline void ser_nlin(void){  ser_char(0xA); ser_char(0xD); }

#endif /* SERIAL_H_ */
