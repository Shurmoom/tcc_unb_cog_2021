/*
 * configs.c
 *
 *  Created on: 22 de abr de 2021
 *      Author: mateu
 */

#include <msp430.h>

#include "configs.h"

void leds_config(void){
    P1DIR |= BIT0;  P1OUT &= ~BIT0; //Led VM
    P4DIR |= BIT7;  P4OUT &= ~BIT7; //Led VD
}

void adc_config(void){
    ADC12CTL0 = ADC12ON | ADC12ENC;
    ADC12CTL1 = ADC12CSTARTADD_0 |        //Mem0
                ADC12SHS_0       |        //Disp por softw
                ADC12CONSEQ_0    |        //um canal, sem repetição
                ADC12SSEL_3;              //SMCLK
    ADC12CTL2 = ADC12RES_2;               //12 bits
    ADC12MCTL0 = ADC12INCH_0    |         //A0=P6.0
                 ADC12SREF_0;             //Ref AVcc e GND

    P6SEL |= BIT0;
}

void mq135_ler(float *medida){
    ADC12CTL0 |= ADC12SC;       //ADC12SC=1
    ADC12CTL0 &= ~ADC12SC;      //ADC12SC=0, disp conversão

    while ((ADC12IFG & ADC12IFG0) == 0); //Converteu?
    *medida = ADC12MEM0;
}

void timer_medida(void){
    TA1CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
    TA1CCTL0 = CCIE;
    TA1CCR0 = 10920; // meio seg
}
