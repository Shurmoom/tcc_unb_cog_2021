/*
 * leds.h
 *
 *  Created on: 14 de abr de 2021
 *      Author: Mateus
 */

#ifndef CONFIGS_H_
#define CONFIGS_H_

inline void led_VM(void) {  P1OUT |=  BIT0;  }  //VM=aceso
inline void led_vm(void) {  P1OUT &= ~BIT0;  }  //VM=apagado
inline void led_Vm(void) {  P1OUT ^=  BIT0;  }  //VM=invertido
inline void led_VD(void) {  P4OUT |=  BIT7;  }  //VD=aceso
inline void led_vd(void) {  P4OUT &= ~BIT7;  }  //VD=apagado
inline void led_Vd(void) {  P4OUT ^=  BIT7;  }  //VD=invertido

void leds_config(void);

void adc_config(void);  // para o MQ-135
void mq135_ler(float *medida);

void timer_medida(void); // leituras a cada 2.5 seg

#endif /* CONFIGS_H_ */
