// LCD_Base
// Rotinas básicas para usar o LCD

// P3.0 ==> SDA
// P3.1 ==> SCL

#include <msp430.h>

#include "LCD.h"

// Incializar LCD modo 4 bits
void lcd_inic(void){

    // Preparar I2C para operar
    UCB0I2CSA = PCF_ADR;    //Endereço Escravo
    UCB0CTL1 |= UCTR    |   //Mestre TX
                UCTXSTT;    //Gerar START
    while ( (UCB0IFG & UCTXIFG) == 0);          //Esperar TXIFG=1
    UCB0TXBUF = 0;                              //Saída PCF = 0;
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT);   //Esperar STT=0
    if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG)    //NACK?
                while(1);

    // Começar inicialização
    lcd_aux(0);     //RS=RW=0, BL=1
    delay(20000);
    lcd_aux(3);     //3
    delay(10000);
    lcd_aux(3);     //3
    delay(10000);
    lcd_aux(3);     //3
    delay(10000);
    lcd_aux(2);     //2

    // Entrou em modo 4 bits
    lcd_aux(2);     lcd_aux(8);     //0x28
    lcd_aux(0);     lcd_aux(8);     //0x08
    lcd_aux(0);     lcd_aux(1);     //0x01
    lcd_aux(0);     lcd_aux(6);     //0x06
    lcd_aux(0);     lcd_aux(0xF);   //0x0F

    while ( (UCB0IFG & UCTXIFG) == 0)   ;          //Esperar TXIFG=1
    UCB0CTL1 |= UCTXSTP;                           //Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    delay(50);
}

// Auxiliar inicialização do LCD (RS=RW=0)
// *** Só serve para a inicialização ***
void lcd_aux(char dado){
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0XF0) | BIT3;            //PCF7:4 = dado;
    delay(50);
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0XF0) | BIT3 | BIT2;     //E=1
    delay(50);
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0XF0) | BIT3;            //E=0;
}

void lcd_char(char letra){
    unsigned int parte_esq = (letra/0x10)*0x10; // 0x40
    unsigned int parte_dir = (letra - parte_esq)*0x10; // (0x41 - 0x40)*0x10 = 0x1*0x10 = 0x10
    pcf_write(parte_esq + 0x9);
    pcf_write(parte_esq + 0xD);
    pcf_write(parte_esq + 0x9);
    pcf_write(parte_dir + 0x9);
    pcf_write(parte_dir + 0xD);
    pcf_write(parte_dir + 0x9);
}

void lcd_str(char palavra[]){
    unsigned int i = 0;
    while(palavra[i] != '\0'){
        lcd_char(palavra[i++]);
    }
}

void lcd_dec16(unsigned int dec){
    unsigned int novo_dec = dec, i, num, vet10[5] = { 10000, 1000, 100, 10, 1 }, achou_alg = 0;
    for (i = 0; i < 5; i++){
        num = novo_dec / vet10[i];
        if (num != 0){
            novo_dec = novo_dec - num * vet10[i];
            achou_alg = 1;
        }
        if (achou_alg == 1) lcd_char(num + 0x30);
    }
    if (achou_alg == 0) lcd_char(0x30);
}

void lcd_cmdo(char x){
    char msn,lsn;
    msn = x & 0xF0;
    lsn = x << 4;

    pcf_write(msn | 0x8);
    pcf_write(msn | 0xC);
    pcf_write(msn | 0x8);
    pcf_write(lsn | 0x8);
    pcf_write(lsn | 0xC);
    pcf_write(lsn | 0x8);
}

void lcd_cursor(char linha, char coluna){
    char x = 0;
    if(linha == 2) x = 0x40;
    x += coluna - 1;
    lcd_cmdo(0x80 | (x&0x7F));
}

void lcd_clear(void){
    lcd_cmdo(0x1);
    lcd_cursor(1,1);
}

void lcd_teste_ascii(unsigned int n){
    unsigned int pos[2] = { 1, 1 }; // linha, coluna --> max 1-2,1-16
    char x = 0x21;
    int t = 0;
    // Titulo
    lcd_clear();
    lcd_str("  TESTE  ASCII");
    lcd_cursor(2,1);
    lcd_str("roda "); lcd_dec16(n);
    if(n == 1) lcd_str(" vez");
    else lcd_str(" vezes");
    delay(100000);
    lcd_clear();

    while (t != n){
        lcd_char(x);
        delay(10000);
        pos[1] += 1; // avança col
        if (pos[1] > 16){
            if (pos[0] == 1){
                pos[0] = 2; // muda para linha 2
                pos[1] = 1; // muda para coluna 1
                lcd_cursor(2, 1);
            }
            else{
                pos[0] = 1; // muda para linha 1
                pos[1] = 1; // muda para coluna 1
                delay(20000);
                lcd_clear();
            }
        }
        x += 1;
        if (x == 127) x = 0xA1;
        else if (x == 0xFE){
            x = 0x21;
            t += 1;
            delay(20000);
            lcd_clear();
        }
    }
}

// Ler a porta do PCF
int pcf_read(void){
    int dado;
    UCB0I2CSA = PCF_ADR;                //Endereço Escravo
    UCB0CTL1 &= ~UCTR;                  //Mestre RX
    UCB0CTL1 |= UCTXSTT;                //Gerar START
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT);
    UCB0CTL1 |= UCTXSTP;                //Gerar STOP + NACK
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    while ( (UCB0IFG & UCRXIFG) == 0);  //Esperar RX
    dado=UCB0RXBUF;
    return dado;
}

// Escrever dado na porta
void pcf_write(char dado){
    UCB0I2CSA = PCF_ADR;        //Endereço Escravo
    UCB0CTL1 |= UCTR    |       //Mestre TX
                UCTXSTT;        //Gerar START
    while ( (UCB0IFG & UCTXIFG) == 0)   ;          //Esperar TXIFG=1
    UCB0TXBUF = dado;                              //Escrever dado
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT)   ;   //Esperar STT=0
    if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG)       //NACK?
                while(1);                          //Escravo gerou NACK
    UCB0CTL1 |= UCTXSTP;                        //Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
}

// Testar endereço I2C
// TRUE se recebeu ACK
int pcf_teste(char adr){
    UCB0I2CSA = adr;                            //Endereço do PCF
    UCB0CTL1 |= UCTR | UCTXSTT;                 //Gerar START, Mestre transmissor
    while ( (UCB0IFG & UCTXIFG) == 0);          //Esperar pelo START
    UCB0CTL1 |= UCTXSTP;                        //Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP);   //Esperar pelo STOP
    if ((UCB0IFG & UCNACKIFG) == 0)     return TRUE;
    else                                return FALSE;
}

// Configurar UCSB0 e Pinos I2C
// P3.0 = SDA e P3.1=SCL
void i2c_config(void){
    UCB0CTL1 |= UCSWRST;    // UCSI B0 em ressete
    UCB0CTL0 = UCSYNC |     //Síncrono
               UCMODE_3 |   //Modo I2C
               UCMST;       //Mestre
    UCB0BRW = BR_100K;      //100 kbps
    P3SEL |=  BIT1 | BIT0;  // Use dedicated module
    UCB0CTL1 = UCSSEL_2;    //SMCLK e remove ressete
}

void delay(long limite){
    volatile long cont=0;
    while (cont++ < limite) ;
}
