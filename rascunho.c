/*
#include <msp430.h>

#include "configs.h" // contém MQ-135
#include "DHT22.h"
#include "LCD.h"
#include "serial.h"

volatile unsigned int modo = 1, meio_seg = 0;

int main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    i2c_config();   // LCD
    lcd_inic();     //Inicializar LCD
    pcf_write(8);   //Acender Back Light
    lcd_cmdo(0xC);  // apagar o cursor

    leds_config();  // DHT22
    gpio_config();
    timer_config();

    adc_config();   // MQ-135

    ser_config();   // serial

    // Titulo LCD
    lcd_str("DHT22 ");
    lcd_cursor(1,7); lcd_str("  "); lcd_char(0xDF); lcd_str("C ");
    lcd_cursor(1,12); lcd_str("  %UR");
    lcd_cursor(2,1); lcd_str("MQ135 CO2 ");
    lcd_cursor(2,14); lcd_str("PPM");

    float temp[3], tempm, umid[3], umidm, gas[3], gasm;
    volatile unsigned int i = 0, esp = 0, seg = 0;
    int parity = 0;

    ser_char(0xD); // corrigir bug inicial no primeiro envio (serial)

    unsigned int tmin = 0, tmax = 0, umidmin = 0, umidmax = 0, gasmax = 0;

    volatile char c;

    while(1){
        c = UCA1RXBUF;
        if(c == 'x' && modo == 0){ // modo Ocioso (aguardando nova leitura ou 'x')
            modo = 1;
            __disable_interrupt();
            while((UCA1IFG & UCRXIFG) == 0); // esperar rx
            UCA1IFG &= ~UCRXIFG;
            ser_str("Nova entrada de dados"); ser_nlin();
            esp = 0;
        }

        if(modo == 1){ // modo Entrada de Dados
            ser_str("Insira os valores mínimo e máximo de temperatura (C): ");
            ser_globalint2(&tmin, &tmax);
            ser_undec16(tmin); ser_char(' '); ser_undec16(tmax); ser_nlin();

            ser_str("Insira os valores mínimo e máximo de umidade (%): ");
            ser_globalint2(&umidmin, &umidmax);
            ser_undec16(umidmin); ser_char(' '); ser_undec16(umidmax); ser_nlin();

            ser_str("Insira o valor máximo de CO2 (PPM): ");
            ser_globalint2(&gasmax, &gasmax);
            ser_undec16(gasmax); ser_nlin();

            //ser_nlin(); ser_str("=== Médias das 3 últimas medidas (T, U, CO2) ==="); ser_nlin(); ser_nlin();

            modo = 0;

            timer_medida(); // ativar disparo de medidas
            __enable_interrupt();
        }

        if(modo == 2){ // modo Leitura e Escrita
            //DHT22
            dht22_ler(&umid[i], &temp[i], &parity);
            if(parity == 0){ temp[i] = 0; umid[i] = 0; led_VD();} // erro de medida --> descartar dados

            //MQ-135
            mq135_ler(&gas[i]);

            //Escrever no LCD --> medidas instantaneas
            lcd_cursor(1,7); lcd_dec16((int)temp[i]);
            lcd_cursor(1,12); lcd_dec16((int)umid[i]);
            lcd_cursor(2,11); lcd_str("   PPM");
            lcd_cursor(2,11); lcd_dec16((int)gas[i]);

            //Escrever na serial --> media movel 3
            if(esp == 2){ // esp serve para esperar 3 medidas
                tempm = (temp[0]+temp[1]+temp[2])/3;
                umidm = (umid[0]+umid[1]+umid[2])/3;
                gasm = (gas[0]+gas[1]+gas[2])/3;
                ser_float(tempm); ser_char(' '); ser_check(tempm, tmin, tmax); ser_char(' ');
                ser_float(umidm); ser_char(' '); ser_check(umidm, umidmin, umidmax); ser_char(' ');
                ser_float(gasm);  ser_char(' '); ser_check(gasm, 0, gasmax); ser_nlin();
            }
            else if(esp == 0) { ser_str("Coletando dados para média móvel"); ser_nlin(); }
            else if(esp == 1) { ser_str("==== Médias de Temperatura, Umidade e CO2 ===="); ser_nlin(); }

            i += 1;
            if(i > 2) i = 0;
            if(esp != 2) esp += 1;

            modo = 0;
        }
    }
    //return 0;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void isr_ccifg0(void)
{
    if(meio_seg == 5){
        meio_seg = 0;
        modo = 2;
    }
    else meio_seg++;
}

 */
