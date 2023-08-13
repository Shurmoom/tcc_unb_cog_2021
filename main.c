#include <msp430.h>

#include "configs.h" // contém MQ-135
#include "DHT22.h"
#include "LCD.h"
#include "serial.h"
#include "nrf_def.h"

volatile unsigned char modo = 2, meio_seg = 0, vent1_estado = 0, vent2_estado = 0, lamp_estado = 0, umid_estado = 0, peltier_estado = 0;

void ventilador_config(void){ // porta P4.0 como OUTPUT
    P4SEL &= ~BIT0;
    P4DIR |= BIT0;
    P4OUT &= ~BIT0;

    P3SEL &= ~BIT7;
    P3DIR |= BIT7;
    P3OUT &= ~BIT7;
}

void lampada_config(void){
    P8SEL &= ~BIT2;
    P8REN |= BIT2;
    P8DIR |= BIT2;
    P8OUT &= ~BIT2;
}

void peltier_config(void){
    P8SEL &= ~BIT1;
    P8DIR |= BIT1;
    P8OUT &= ~BIT1;
}

inline void ventilador1_on(void) {   P4OUT |= BIT0;  }
inline void ventilador1_off(void) {  P4OUT &= ~BIT0; }
inline void ventilador2_on(void) {   P3OUT |= BIT7;  }
inline void ventilador2_off(void) {  P3OUT &= ~BIT7; }

inline void lamp_on(void){
    P8OUT |= BIT2;
    meio_seg = 0;
}
inline void lamp_off(void){
    P8OUT &= ~BIT2;
    meio_seg = 0;
}

inline void peltier_on(void) {   P8OUT |= BIT1;  }
inline void peltier_off(void) {  P8OUT &= ~BIT1; }

void lcd_titulo(unsigned int gasmax,unsigned int tmin,unsigned int tmax,unsigned int umidmin,unsigned int umidmax){
    lcd_str("     ");
    lcd_cursor(1,7); lcd_str("  "); lcd_char(0xDF); lcd_str("C ");
    lcd_cursor(1,12); lcd_str("  %UR");
    lcd_cursor(2,1); lcd_dec16(gasmax); lcd_cursor(2,6);
    lcd_dec16(tmin); lcd_cursor(2,8); lcd_char('-'); lcd_dec16(tmax); lcd_char(' ');
    lcd_dec16(umidmin); lcd_cursor(2,14); lcd_char('-'); lcd_dec16(umidmax);
}

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

    ventilador_config();    // P4.0 e P3.7 como output
    lampada_config();       // P8.2 como output
    peltier_config();

    unsigned int tmin = 22, tmax = 26, umidmin = 50, umidmax = 90, gasmax = 800;

    lcd_titulo(gasmax,tmin,tmax,umidmin,umidmax);

    float temp[3], tempm, umid[3], umidm, gas[3], gasm;
    volatile unsigned int i = 0, esp = 0, seg = 0;
    int parity = 0;

    ser_char(0xD); // corrigir bug inicial no primeiro envio (serial)


    timer_medida();         // para começar medindo --> testes
    __enable_interrupt();

    volatile char c;

    //====== TESTE NRF ======
    unsigned char payload[27]="25,600 > 60,799 # 545,666 #";
    spi_config();
    unsigned char nome[5]="COGML";
    nrf_config();
    nrf_wr_addr(NRF_TX_ADDR, nome);
    nrf_wr_addr(NRF_RX_ADDR_P0, nome);
    nrf_flush_tx();
    nrf_flush_rx();
    nrf_modo_tx();
    nrf_wr_reg(NRF_STATUS,RX_DR|TX_DS|MAX_RT);  //RX_DR=0, TX_DS=0; MAX_RT=0
    nrf_power_up();

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

        if(c == 'y' && modo == 0){ // teste VENT 1
            while((UCA1IFG & UCRXIFG) == 0); // esperar rx
            UCA1IFG &= ~UCRXIFG;
            if(vent1_estado == 0){ // liga/desliga os ventiladores
                ser_str("Vent 1 ON"); ser_nlin();
                vent1_estado = 1;
                ventilador1_on();
            }
            else{
                ser_str("Vent 1 OFF"); ser_nlin();
                vent1_estado = 0;
                ventilador1_off();
            }
        }

        if(c == 'w' && modo == 0){ // teste VENT 2
            while((UCA1IFG & UCRXIFG) == 0); // esperar rx
            UCA1IFG &= ~UCRXIFG;
            if(vent2_estado == 0){ // liga/desliga os ventiladores
                ser_str("Vent 2 ON"); ser_nlin();
                vent2_estado = 1;
                ventilador2_on();
            }
            else{
                ser_str("Vent 2 OFF"); ser_nlin();
                vent2_estado = 0;
                ventilador2_off();
            }
        }

        if(c == 'z' && modo == 0){ // teste LAMP
            while((UCA1IFG & UCRXIFG) == 0); // esperar rx
            UCA1IFG &= ~UCRXIFG;
            if(lamp_estado == 0){ // liga/desliga a lampada
                ser_str("Lampada ON"); ser_nlin();
                lamp_estado = 1;
                lamp_on();
            }
            else{
                ser_str("Lampada OFF"); ser_nlin();
                lamp_estado = 0;
                lamp_off();
            }
            lcd_inic(); // reiniciar LCD (evita bugs)
            lcd_cmdo(0xC);
            lcd_titulo(gasmax,tmin,tmax,umidmin,umidmax);
        }

        if(c == 'k' && modo == 0){ // teste PELTIER
            while((UCA1IFG & UCRXIFG) == 0); // esperar rx
            UCA1IFG &= ~UCRXIFG;
            if(peltier_estado == 0){ // liga/desliga umid
                ser_str("Peltier ON"); ser_nlin();
                peltier_estado = 1;
                peltier_on();
            }
            else{
                ser_str("Peltier OFF"); ser_nlin();
                peltier_estado = 0;
                peltier_off();
            }
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

            modo = 0;
            timer_medida(); // ativar disparo de medidas
            __enable_interrupt();
        }

        if(modo == 2){ // modo Leitura e Escrita
            led_VM();
            //DHT22
            dht22_ler(&umid[i], &temp[i], &parity);
            if(parity == 0){ temp[i] = 0; umid[i] = 0; led_VD();} // erro de medida --> descartar dados

            //MQ-135
            mq135_ler(&gas[i]);

            lcd_cursor(1,1); lcd_dec16((int)gas[i]); lcd_char('P');
            lcd_cursor(1,7); lcd_dec16((int)temp[i]); lcd_cursor(1,9); lcd_char(0xDF); lcd_str("C ");
            lcd_cursor(1,12); lcd_dec16((int)umid[i]);lcd_cursor(1,14); lcd_str("%UR");
            lcd_cursor(2,1); lcd_dec16(gasmax); lcd_cursor(2,6);
            lcd_dec16(tmin); lcd_cursor(2,8); lcd_char('-'); lcd_dec16(tmax); lcd_char(' ');
            lcd_dec16(umidmin); lcd_cursor(2,14); lcd_char('-'); lcd_dec16(umidmax);

            //Escrever na serial --> media movel 3
            if(esp == 2){ // esp serve para esperar 3 medidas
                tempm = (temp[0]+temp[1]+temp[2])/3;
                umidm = (umid[0]+umid[1]+umid[2])/3;
                gasm = (gas[0]+gas[1]+gas[2])/3;
                ser_float(tempm); ser_char(' '); ser_check(tempm, tmin, tmax); ser_char(' ');
                ser_float(umidm); ser_char(' '); ser_check(umidm, umidmin, umidmax); ser_char(' ');
                ser_float(gasm);  ser_char(' '); ser_check(gasm, 0, gasmax); ser_nlin();

                //====== TESTE NRF ======
                nrf_wr_reg(NRF_STATUS,RX_DR|TX_DS|MAX_RT); // limpa regs
                nrf_wr_payload(payload,27);
            }
            else if(esp == 0) { ser_str("Coletando dados para média móvel"); ser_nlin(); }
            else if(esp == 1) { ser_str("==== Médias de Temperatura, Umidade e CO2 ===="); ser_nlin(); }

            i += 1;
            if(i > 2) i = 0;
            if(esp != 2) esp += 1;

            modo = 0;
            led_vm();
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
    else{
        meio_seg++;
        led_Vd();
    }
}
