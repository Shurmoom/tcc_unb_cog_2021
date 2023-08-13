/*
 * nrf_def.h
 *
 *  Created on: 21 de mai de 2021
 *      Author: mateu
 */

#ifndef NRF_DEF_H_
#define NRF_DEF_H_

///////////////////////////////////////////
//////// NRF24L01 - Constantes ////////////
///////////////////////////////////////////

#define NRF_BUF_TAM 32  //Tamanho do payload

// Constantes para facilitar nomear os pipes
// Somente os pipes de recepÃ§Ã£o
#define RX_PIPE_0 0
#define RX_PIPE_1 1
#define RX_PIPE_2 2
#define RX_PIPE_3 3
#define RX_PIPE_4 4
#define RX_PIPE_5 5

// NRF - Registradores
#define NRF_CONFIG      0x00
#define NRF_EN_AA       0x01
#define NRF_EN_RXADDR   0x02
#define NRF_SETUP_AW    0x03
#define NRF_SETUP_RETR  0x04
#define NRF_RF_CH       0x05
#define NRF_RF_SETUP    0x06
#define NRF_STATUS      0x07
#define NRF_OBSERVE_TX  0x08
#define NRF_RPD         0x09  //CD no 24L01
#define NRF_RX_ADDR_P0  0x0A
#define NRF_RX_ADDR_P1  0x0B
#define NRF_RX_ADDR_P2  0x0C
#define NRF_RX_ADDR_P3  0x0D
#define NRF_RX_ADDR_P4  0x0E
#define NRF_RX_ADDR_P5  0x0F
#define NRF_TX_ADDR     0x10
#define NRF_RX_PW_P0    0x11
#define NRF_RX_PW_P1    0x12
#define NRF_RX_PW_P2    0x13
#define NRF_RX_PW_P3    0x14
#define NRF_RX_PW_P4    0x15
#define NRF_RX_PW_P5    0x16
#define NRF_FIFO_STATUS 0x17
#define NRF_DYNPD       0x1C
#define NRF_FEATURE     0x1D

// NRF - Comandos
#define R_REGISTER         0x00
#define W_REGISTER         0x20
#define R_RX_PAYLOAD       0x61
#define W_TX_PAYLOAD       0xA0
#define FLUSH_TX           0xE1
#define FLUSH_RX           0xE2
#define REUSE_TX_PL        0xE3
#define R_RX_PL_WID        0x60
#define W_ACK_PAYLOAD      0xA8
#define W_TX_PAYLOAD_NOACK 0xB0
#define NOP                0xFF

// NRF - MnemÃ´nico para os bits
#define REG_MASK    0x1F    //Mascarar 5 bits Ã  direita
#define MASK_RX_DR  BIT6    //Reg 00 CONFIG
#define MASK_TX_DS  BIT5    //Reg 00 CONFIG
#define MASK_MAX_RT BIT4    //Reg 00 CONFIG
#define EN_CRC      BIT3    //Reg 00 CONFIG
#define CRCO        BIT2    //Reg 00 CONFIG
#define PWR_UP      BIT1    //Reg 00 CONFIG
#define PRIM_RX     BIT0    //Reg 00 CONFIG
#define ENAA_P5     BIT5    //Reg 01 EN_AA
#define ENAA_P4     BIT4    //Reg 01 EN_AA
#define ENAA_P3     BIT3    //Reg 01 EN_AA
#define ENAA_P2     BIT2    //Reg 01 EN_AA
#define ENAA_P1     BIT1    //Reg 01 EN_AA
#define ENAA_P0     BIT0    //Reg 01 EN_AA
#define ERX_P5      BIT5    //Reg 02 EN_RXADDR
#define ERX_P4      BIT4    //Reg 02 EN_RXADDR
#define ERX_P3      BIT3    //Reg 02 EN_RXADDR
#define ERX_P2      BIT2    //Reg 02 EN_RXADDR
#define ERX_P1      BIT1    //Reg 02 EN_RXADDR
#define ERX_P0      BIT0    //Reg 02 EN_RXADDR
#define AW          BIT0    //Reg 03 SETUP_AW (2 bits)
#define ARD         4       //Reg 04 SETUP_RETR (bits 7,6,5,4)
#define ARC         0       //Reg 04 SETUP_RETR (bits 3,2,1,0)
#define CONT_WAVE   BIT7    //Reg 06 RF_SETUP
#define RF_DR_LOW   BIT5    //Reg 06 RF_SETUP
#define PLL_LOCK    BIT4    //Reg 06 RF_SETUP
#define RF_DR_HIGH  BIT3    //Reg 06 RF_SETUP
#define RF_PWR      BIT1    //Reg 06 RF_SETUP (2 bits)
#define RX_DR       BIT6    //Reg 07 STATUS
#define TX_DS       BIT5    //Reg 07 STATUS
#define MAX_RT      BIT4    //Reg 07 STATUS
#define RX_P_NO     1       //Reg 07 STATUS (bits 3,2,1)
//#define TX_FULL     BIT0    //Reg 07 STATUS
#define PLOS_CNT    BIT4    //Reg 08 OBSERVE_TX (4 bits)
#define ARC_CNT     BIT0    //Reg 08 OBSERVE_TX (4 bits)
#define TX_REUSE    BIT6    //Reg 17 FIFO_STATUS
#define TX_FULL     BIT5    //Reg 17 FIFO_STATUS
#define TX_EMPTY    BIT4    //Reg 17 FIFO_STATUS
#define RX_FULL     BIT1    //Reg 17 FIFO_STATUS
#define RX_EMPTY    BIT0    //Reg 17 FIFO_STATUS
#define DPL_P5      BIT5    //Reg 1C DYNPD
#define DPL_P4      BIT4    //Reg 1C DYNPD
#define DPL_P3      BIT3    //Reg 1C DYNPD
#define DPL_P2      BIT2    //Reg 1C DYNPD
#define DPL_P1      BIT1    //Reg 1C DYNPD
#define DPL_P0      BIT0    //Reg 1C DYNPD
#define EN_DPL      BIT2    //Reg 1D FEATURE
#define EN_ACK_PAY  BIT1    //Reg 1D FEATURE
#define EN_DYN_ACK  BIT0    //Reg 1D FEATURE

void ser_charbin(unsigned char c){
    int i;
    for (i = 7; i >= 0; --i){
        ser_char( (c & (1 << i)) ? '1' : '0' );
    }
    //ser_char('\n');
}

// ============= SPI =============
void spi_config(void){
    UCA0CTL1 = UCSSEL_2 | UCSWRST;  //SMCLK e RST
    UCA0CTL0 = UCCKPH | UCMSB | UCMST | UCSYNC;
    UCA0BRW = 10;

    P6DIR |= BIT5; // P6.5 = CSN
    P6OUT |= BIT5; // CSN em alto --> sem comu

    //P6DIR |= BIT6; // P6.6 = CE
    //P1DIR |= BIT6; // P1.6 = IRQ

    P3SEL |= BIT3 | BIT4;
    P2SEL |= BIT7;

    UCA0CTL1 = UCSSEL_2;
}

unsigned char spi_char(unsigned char dado){
    UCA0TXBUF = dado;
    while ((UCA0IFG & UCRXIFG) == 0);
    //UCA0IFG &= ~UCRXIFG;
    return UCA0RXBUF;
}

// ============= NRF =============
inline void nrf_ce(void) { P6OUT &= ~BIT6; }
inline void nrf_CE(void) { P6OUT |= BIT6; }
inline void nrf_csn(void) { P6OUT &= ~BIT5; }
inline void nrf_CSN(void) { P6OUT |= BIT5; }

unsigned char nrf_wr_reg(unsigned char reg, unsigned char dado){
    unsigned char rg,stat;
    rg=W_REGISTER | (REG_MASK&reg); //só 5 bits de reg
    nrf_csn();
    stat=spi_char(rg);
    spi_char(dado);
    nrf_CSN();
    return stat;
}

unsigned char nrf_wrser_reg(unsigned char reg, unsigned char dado){ // para debug
    unsigned char rg,stat;
    rg=W_REGISTER | (REG_MASK&reg); //só 5 bits de reg
    nrf_csn();
    stat=spi_char(rg);
    spi_char(dado);
    nrf_CSN();
    ser_str("reg  WR: "); ser_charbin(reg); ser_char(' '); ser_undec16(reg); ser_nlin();
    //ser_str("mask WR: "); ser_charbin(rg); ser_char(' '); ser_undec16(rg); ser_nlin();
    ser_str("dado WR: "); ser_charbin(dado); ser_char(' '); ser_undec16(dado); ser_nlin();
    //ser_str("stat WR: "); ser_charbin(stat); ser_char(' '); ser_undec16(stat); ser_nlin();
    return stat;
}

unsigned char nrf_rd_reg(unsigned char reg){
    unsigned char dado;
    nrf_csn();
    dado=spi_char(reg); // status
    dado=spi_char(0);
    nrf_CSN();
    return dado;
}

unsigned char nrf_rdser_reg(unsigned char reg){ // para debug
    unsigned char dado;
    //unsigned char stat;
    nrf_csn();
    //stat=spi_char(reg);
    dado=spi_char(reg); // status
    dado=spi_char(0);
    nrf_CSN();
    ser_str("reg  rd: "); ser_charbin(reg); ser_char(' '); ser_undec16(reg); ser_nlin();
    //ser_str("stat rd: "); ser_charbin(stat); ser_char(' '); ser_undec16(stat); ser_nlin();
    ser_str("dado rd: "); ser_charbin(dado); ser_char(' '); ser_undec16(dado); ser_nlin();
    return dado;
}

void nrf_ser_status(void){ // para debug
    unsigned char stat;
    nrf_csn();
    stat = spi_char(NOP);
    nrf_CSN();
    ser_str("RX_DR: ");   ser_undec16((0b01000000 & stat)>>6); ser_nlin();
    ser_str("TX_DS: ");   ser_undec16((0b00100000 & stat)>>5); ser_nlin();
    ser_str("MAX_RT: ");  ser_undec16((0b00010000 & stat)>>4); ser_nlin();
    ser_str("RX_P_NO: "); ser_undec16((0b00001110 & stat)>>1); ser_nlin();
    ser_str("TX_FULL: "); ser_undec16(0b00000001 & stat); ser_nlin();
    ser_nlin();
}

void nrf_ser_regs(void){ // imprime todos os regs
    volatile unsigned char vet[9]={0,1,2,3,4,5,6,7,NOP},k=0;
    for(k=0;k<9;k++){
        nrf_rdser_reg(vet[k]);
        ser_nlin();
    }
}

void nrf_power_up(void){
    unsigned char x;
    x=nrf_rd_reg(NRF_CONFIG);
    x |= PWR_UP;
    x=nrf_wr_reg(NRF_CONFIG,x);
}

void nrf_power_down(void){
    unsigned char x;
    x=nrf_rd_reg(NRF_CONFIG);
    x &= ~PWR_UP;
    x=nrf_wr_reg(NRF_CONFIG,x);
}

void nrf_modo_rx(void){
    unsigned char x;
    x=nrf_rd_reg(NRF_CONFIG);
    x |= PRIM_RX;              //PRIM_RX=1 => Modo recepção
    x=nrf_wr_reg(NRF_CONFIG,x);
}

void nrf_modo_tx(void){
    unsigned char x;
    x=nrf_rd_reg(NRF_CONFIG);
    x &= ~PRIM_RX;              //PRIM_RX=0 => Modo transmissão
    x=nrf_wr_reg(NRF_CONFIG,x);
}

unsigned char nrf_flush_tx(void){
    unsigned char x;
    nrf_csn();
    x=spi_char(FLUSH_TX);
    nrf_CSN();
    return x;
}

unsigned char nrf_flush_rx(void){
    unsigned char x;
    nrf_csn();
    x=spi_char(FLUSH_RX);
    nrf_CSN();
    return x;
}

void nrf_config(void){
    nrf_flush_tx();
    nrf_wr_reg(NRF_SETUP_RETR,(5<<ARD) | (15<<ARC));//Espera 1500us e 15 retransmissões
    nrf_wr_reg(NRF_RF_SETUP,0x01);                  //??`Precisa disso??
    nrf_wr_reg(NRF_RF_CH,0x4C);                     //Freq do canal (76)
    nrf_wr_reg(NRF_EN_AA,0x3F);                     //Hab Auto Ack em todos os pipes (0-5)
    nrf_wr_reg(NRF_SETUP_AW,0x03);                  //Endereço com 5 bytes
    nrf_wr_reg(NRF_RX_PW_P0,0x20);                  //Pipe 0  = payload de 32 bytes
    nrf_wr_reg(NRF_RX_PW_P1,0x20);                  //Pipe 1  = payload de 32 bytes
    nrf_wr_reg(NRF_RX_PW_P2,0x20);                  //Pipe 2  = payload de 32 bytes
    nrf_wr_reg(NRF_RX_PW_P3,0x20);                  //Pipe 3  = payload de 32 bytes
    nrf_wr_reg(NRF_RX_PW_P4,0x20);                  //Pipe 4  = payload de 32 bytes
    nrf_wr_reg(NRF_RX_PW_P5,0x20);                  //Pipe 5  = payload de 32 bytes
    nrf_wr_reg(NRF_CONFIG,EN_CRC|CRCO);             //Hab CRC e CRC com 2 bytes; CRC = cyclic redundancy check
    nrf_wr_reg(NRF_STATUS, RX_DR|TX_DS|MAX_RT);     //RX_DR=TX_DS=MAX_RT=0
}

unsigned char nrf_wr_payload(unsigned char *vet, unsigned char qtd){
    unsigned char i,stat;
    nrf_csn();
    stat=spi_char(W_TX_PAYLOAD);
    for (i=0; i<qtd; i++) spi_char(vet[i]);
    for (;i<32;i++)       spi_char(0);        //Completar com zeros
    nrf_CSN();
    nrf_CE();   //CE=1 ==> Transmitir
    delay(20); // 14 useg?
    nrf_ce();   //CE=0 ==> Finalizar
    //while ( (nrf_rd_status()&TX_DS) == 0);   //Esperar terminar de transmitir
    //delay(10);
    //nrf_ser_status();
    return stat;
}

unsigned char nrf_wr_reg_mb(unsigned char reg, unsigned char *vt, unsigned char qtd){
    unsigned char rg,i;
    rg=W_REGISTER | (reg&REG_MASK);  //Montar comando de escrita em Reg
    nrf_csn();
    spi_char(rg);
    for (i=0; i<qtd; i++) spi_char(vt[i]);
    nrf_CSN();
    return spi_char(NOP);
}

unsigned char nrf_wr_addr(unsigned char pipe, unsigned char *vet){
    unsigned char qtd,stat;
    qtd=2+nrf_rd_reg(NRF_SETUP_AW);   //Tamanho do endereço
    switch(pipe){
        case NRF_RX_ADDR_P0:
        case NRF_RX_ADDR_P1:
        case NRF_TX_ADDR:       stat=nrf_wr_reg_mb(pipe, vet, qtd);  break;
        case NRF_RX_ADDR_P2:
        case NRF_RX_ADDR_P3:
        case NRF_RX_ADDR_P4:
        case NRF_RX_ADDR_P5:    stat=nrf_wr_reg(pipe, vet[0]);      break;
    }
    return stat;
}

unsigned char nrf_rd_payload(unsigned char *vet, unsigned char qtd){
    unsigned char i;
    nrf_csn();
    spi_char(R_RX_PAYLOAD);
    for (i=0; i<qtd; i++) vet[i]=spi_char(NOP);
    nrf_CSN();
    return spi_char(NOP);
}

void nrf_clear_max_rt(void){
    unsigned char stat;
    stat = nrf_rd_reg(7);
    stat = stat | 0b00010000;
    nrf_wr_reg(7,stat);
}

void ser_stru(unsigned char palavra[]){
    int i = 0;
    while(palavra[i] != '\0'){
        ser_char(palavra[i]);
        i += 1;
    }
}

#endif /* NRF_DEF_H_ */
