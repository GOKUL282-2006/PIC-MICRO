#include <xc.h>
#define _XTAL_FREQ 20000000

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


void UART_Init(void){
    TRISC6=0;
    TRISC7=1;
    SPBRG=129;
    BRGH=1;
    SYNC=0;
    SPEN=1;
    TXEN=1;
    CREN=1;
}
void UART_Write(char data){
    while(!TXIF);
    TXREG=data;
}
void UART_Write_Text(const char *text){
    while(*text){
        UART_Write(*text);
        text++;
    }
}
void UART_Write_Number(unsigned int number){
    if(number>=1000){
    UART_Write((number/1000)+'0');
    number=number%1000;
    }
     if(number>=100){
    UART_Write((number/100)+'0');
    number=number%100;
    }
     if(number>=10){
    UART_Write((number/10)+'0');
    number=number%10;
    }
    UART_Write(number+'0');
}
void ADC_Init(void){
    ADCON0=0x41;
    ADCON1=0x8E;
    }
unsigned int ADC_Read(void){
    __delay_us(20);
    GO_nDONE=1;
    while(GO_nDONE);
    return((unsigned int)ADRESH<<8)|ADRESL;
}
void main(void) {
    unsigned int adc_value;
    unsigned int temperature;
    
    ADC_Init();
    UART_Init();
    
    UART_Write_Text("\r\n");
    UART_Write_Text("PIC16F877A Temperature Monitor\r\n");
    UART_Write_Text("--------------------\r\n");
    
    while(1){
        adc_value=ADC_Read();
        
        temperature=(adc_value*500UL)/1023UL;
        UART_Write_Text("Temperature");
         UART_Write_Number(temperature);
          UART_Write_Text("C\r\n");
          __delay_ms(1000);
        
    }
    return;
}
