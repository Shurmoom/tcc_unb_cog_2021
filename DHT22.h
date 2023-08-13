/*
 * DHT22.h
 *
 *  Created on: 14 de abr de 2021
 *      Author: Mateus
 */

#ifndef DHT22_H_
#define DHT22_H_

#define TRUE    1
#define FALSE   0

#define Tbe_tip 1000    // Host the start signal down

// One Wire ou Single Bus?
inline char ow_pin_rd(void) {return (P2IN&BIT5)>>5;}    //Ler P2.5
inline void ow_pin_low(void) {P2DIR |=  BIT5; }         //P2.5 = Zero
inline void ow_pin_hiz(void) {P2DIR &= ~BIT5; }         //P2.5 = Hi-Z

void gpio_config(void);
void timer_config(void);
void dht22_ler(float *umid_loc, float *temp_loc, int *parity_loc);

#endif /* DHT22_H_ */
