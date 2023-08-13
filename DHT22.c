/*
 * DHT22.c
 *
 *  Created on: 18 de abr de 2021
 *      Author: mateu
 */

#include <msp430.h>

#include "configs.h"
#include "dht22.h"

void gpio_config(void){
    P2DIR &= ~BIT5;     // P2.5 = Hi-Z
    P2OUT &= ~BIT5;     // P2.5 = 0
    P2REN &= ~BIT5;     // Desabilita Pull/up/down
}

void timer_config(void){
    TA0CTL = TASSEL_2 | ID_0 | MC_2;
}

void dht22_ler(float *umid_loc, float *temp_loc, int *parity_loc){

    volatile unsigned int pot_dois[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    volatile int i = 0;
    static int vet[43];
    for(i = 0; i<43; i++){ vet[i] = 0;}
    i = 0;

    // Start Signal
    ow_pin_low();
    __delay_cycles(Tbe_tip);
    ow_pin_hiz();

    // All data
    char aux = 0;
    TA0CTL |= TACLR;
    while (i < 43)
    {
        while (aux == 1){ aux = ow_pin_rd(); aux ^= BIT0;};
        vet[i++] = TA0R - 50;
        TA0CTL |= TACLR;
        if(i == 43) break;
        while (aux == 0){ aux = ow_pin_rd(); aux ^= BIT0;};
    }
    i = 0;
    // Tirar o lixo
    for(i = 0; i<40; i++){ vet[i] = vet[i+3];}

    // Converter em binario
    for (i = 0; i < 40; i++)
    {
        if (vet[i] < 55)
            vet[i] = 0;
        else
            vet[i] = 1;
    }
    // Umidade
    volatile unsigned int umid_hi = 0;
    volatile unsigned int umid_hi_s = 0;
    volatile unsigned int umid_low = 0;
    for (i = 0; i < 8; i++)
    {
        umid_hi += (vet[i] * pot_dois[i]);
        umid_hi_s += (vet[i] * pot_dois[i]);
    }
    umid_hi_s = umid_hi;
    umid_hi = umid_hi * 256;
    for (i = 8; i < 16; i++)
    {
        umid_low += (vet[i] * pot_dois[i - 8]);
    }
    *umid_loc = (float) (umid_hi + umid_low)/10;

    // Temperatura
    volatile unsigned int temp_hi = 0;
    volatile unsigned int temp_hi_s = 0;
    volatile unsigned int temp_low = 0;
    vet[16] = 0;
    for (i = 17; i < 24; i++)
    {
        temp_hi += (vet[i] * pot_dois[i - 16]);
    }
    temp_hi_s = temp_hi;
    temp_hi = temp_hi * 256;
    for (i = 24; i < 32; i++)
    {
        temp_low += (vet[i] * pot_dois[i - 24]);
    }
    *temp_loc = (float) (temp_hi + temp_low)/10;

    // Parity
    volatile unsigned int parity = 0;
    volatile unsigned int sum = umid_hi_s + umid_low + temp_hi_s + temp_low;
    while(sum >= 256) sum = sum - 256;
    for (i = 32; i < 40; i++)
    {
        parity = parity + (vet[i] * pot_dois[i - 32]);
    }
    if(parity == sum){ *parity_loc = 1; }
    else { *parity_loc = 0; }

}
