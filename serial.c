#include <msp430.h>

#include "serial.h"

#define TRUE    1
#define FALSE   0
#define SMCLK   1048576L  //SMCLK

char palavra_global[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile unsigned int k = 0;

void ser_config(void){
    UCA1CTL1 =  UCSSEL_2 | UCSWRST;     //UCSI em Reset
    UCA1CTL0 = UCSPB;                   //2 Stops

    // Baudrate 9.600
    //UCA1BRW = 6;                //Divisor por 3
    //UCA1MCTL=UCBRF_13 | UCBRS_0 | UCOS16;

    // 115200
    UCA1BRW = 9;                //Divisor por 3
    UCA1MCTL=UCBRF_0 | UCBRS_1;

    P4DIR |=  BIT4;             //P4.4 saída
    P4DIR &= ~BIT5;             //P4.5 entrada
    P4SEL |= BIT5 | BIT4;       //Tx serial

    PMAPKEYID = 0X02D52;    //Liberar mapeamentp
    P4MAP4 = PM_UCA1TXD;    //TX=Sair por P4.4
    P4MAP5 = PM_UCA1RXD;    //Sair por P4.5

    UCA1CTL1 &= ~ UCSWRST;      //UCSI sai de Reset

}

void ser_char(char c){
    UCA1TXBUF = c;
    while ((UCA1IFG & UCTXIFG) == 0);
    UCA1IFG &= ~UCTXIFG;
}

void ser_str(char palavra[]){
    int i = 0;
    while(palavra[i] != '\0'){
        ser_char(palavra[i]);
        i += 1;
    }
}

void ser_undec16(unsigned int dec){
    unsigned int novo_dec = dec, i, num, vet10[5] = { 10000, 1000, 100, 10, 1 }, achou_alg = 0;
    for (i = 0; i < 5; i++){
        num = novo_dec / vet10[i];
        if (num != 0){
            novo_dec = novo_dec - num * vet10[i];
            achou_alg = 1;
        }
        if (achou_alg == 1) ser_char(num + 0x30);
    }
    if (achou_alg == 0) ser_char(0x30);
}

void ser_float(float num){ // xx,abc
    long int esquerda = (long int) num;
    ser_undec16(esquerda);

    ser_char(',');

    float direita = num - esquerda; // direita = 0,abc
    direita = direita * 1000; // direita = abc,00
    direita = (int) direita; // direita = abc

    float dig1 = direita/100.0; // abc/100.0 = a.bc
    dig1 = (int) dig1; // (int) a.bc = a
    ser_char(dig1 + 0x30); // manda 'a'

    float dig2 = (direita - (dig1*100))/10.0; // (abc - a00)/10.0 = bc/10.0 = b.c
    dig2 = (int) dig2; // (int) b.c = b
    ser_char(dig2 + 0x30); // manda 'b'

    float dig3 = (direita - (dig1*100) - (dig2*10)); // abc - a00 - b0 = c
    dig3 = (int) dig3;
    ser_char(dig3 + 0x30); // manda 'c'
}

void ser_dechex(long int decimalNumber){ // max 4095, 4096 = 0xFFF
    long int quotient;
    int i = 1, j, temp;
    char hexadecimalNumber[100];
    quotient = decimalNumber;
    while (quotient != 0)
    {
        temp = quotient % 16;
        //To convert integer into character
        if (temp < 10)
            temp = temp + 48;
        else
            temp = temp + 55;
        hexadecimalNumber[i++] = temp;
        quotient = quotient / 16;
    }
    for (j = i - 1; j > 0; j--)
        ser_char(hexadecimalNumber[j]);
    if(decimalNumber == 0) ser_char('0');
}

char ser_lerchar(void){
    while((UCA1IFG & UCRXIFG) == 0);
    UCA1IFG &= ~UCRXIFG;
    return UCA1RXBUF;
}

int ser_charint(char c){
    return c - 0x30;
}

void ser_leresc(void){  // salva a entrada na global
    char c = 0;
    while(c != 0xA && c !=0xD){
        c = ser_lerchar();
        if(c != ' ') palavra_global[k] = ser_charint(c);
        else palavra_global[k] = c;
        k += 1;
    }
    palavra_global[k-1] = 'f';
    k = 0;
}

void ser_limpaglobal(void){
    int i = 0;
    for(i=0; i<10; i++){
        palavra_global[i] = 0;
    }
}

void ser_globalint2(unsigned int *min, unsigned int *max){ // max 999
    ser_leresc();
    unsigned int tam = 0, cont = 0;
    while (cont < 2){
        tam += 1;
        if (palavra_global[k] == ' '){
            if (tam == 4) *min = (palavra_global[k-3]*100) + (palavra_global[k-2]*10) + palavra_global[k - 1];
            else if (tam == 3) *min = palavra_global[k - 2] * 10 + palavra_global[k - 1];
            else if (tam == 2) *min = palavra_global[k - 1];
            tam = 0;
            cont += 1;
        }
        else if (palavra_global[k] == 'f'){
            if (tam == 4) *max = (palavra_global[k-3]*100) + (palavra_global[k-2]*10) + palavra_global[k - 1];
            else if (tam == 3) *max = palavra_global[k - 2] * 10 + palavra_global[k - 1];
            else if (tam == 2) *max = palavra_global[k - 1];
            cont += 1;
        }
        k += 1;
    }
    k = 0; cont = 0; ser_limpaglobal();
}

void ser_check(float media, int min, int max){
    if(media > max){
        ser_char('>');
    }
    else if(media < min){
        ser_char('<');
    }
    else ser_char('#');
}
