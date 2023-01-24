#include "lib/include.h"

extern void Configurar_UART5(void)
{
    SYSCTL->RCGCUART  = (1<<1);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<2);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port B
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOB->AFSEL = (1<<0) | (1<<1);
    //GPIO Port Control (GPIOPCTL) PB0-> U1Rx PB1-> U1Tx pag.688
    GPIOB->PCTL = (GPIOB->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<1);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOB->DEN = (1<<0) | (1<<1);//PB0 PB1
    //UART1 UART Control (UARTCTL) pag.918 DISABLE!!
    UART1->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 40,000,000 / (16*28800) = 86.8055
    UARTFBRD[DIVFRAC] = integer(.8055 * 64 + 0.5)
    */
    UART1->IBRD = 86;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART1->FBRD = 52;
    //  UART Line Control (UARTLCRH) pag.916
    UART1->LCRH = (0x3<<5)|(1<<4);//DETERMINA EL TAMAÑO DEL ENVIO 8 BITS
    //  UART Clock Configuration(UARTCC) pag.939
    UART1->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART1->CTL = (1<<0) | (1<<8) | (1<<9);



}

extern void trans(char c)
{ 
     while((UART1->FR & (1<<5))!=0);
    {
        UART1->DR=c;
    }
}

extern void enviar(char *string)
{
while(*string)
{
trans(*(string++)); 
}
trans('\n');
}








