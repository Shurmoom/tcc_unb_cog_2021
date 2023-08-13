/*
 * LCD.h
 *
 *  Created on: 22 de abr de 2021
 *      Author: mateu
 */

#ifndef LCD_H_
#define LCD_H_

#define TRUE    1
#define FALSE   0

// Definição do endereço do PCF_8574
#define PCF_ADR  0x27

#define BR_100K    11  //SMCLK/100K = 11
#define BR_50K     21  //SMCLK/50K  = 21
#define BR_10K    105  //SMCLK/10K  = 105

void lcd_inic(void);
void lcd_aux(char dado);
void lcd_char(char letra);
void lcd_str(char palavra[]);
void lcd_dec16(unsigned int dec);
void lcd_cmdo(char x);
void lcd_cursor(char linha, char coluna);
void lcd_clear(void);
void lcd_teste_ascii(unsigned int n); // roda todos os cacarcteres n vezes

int pcf_read(void);
void pcf_write(char dado);
int pcf_teste(char adr);

void i2c_config(void);
void delay(long limite);

#endif /* LCD_H_ */
