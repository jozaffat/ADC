#include "lib/include.h"

extern void Configurar_UART5(void)
{
    SYSCTL->RCGCUART  = (1<<5);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<4);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port E
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOE->AFSEL = (1<<4) | (1<<5);
    //GPIO Port Control (GPIOPCTL) PE4-> U5Rx PE5-> U5Tx pag.688
    GPIOE->PCTL = (GPIOE->PCTL&0xFF00FFFF) | 0x00110000;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOE->DEN = (1<<4) | (1<<5);//PE4 PE5
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART5->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 40,000,000 / (16*28800) = 86.8055
    UARTFBRD[DIVFRAC] = integer(.8055 * 64 + 0.5)
    */
    UART5->IBRD = 86;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART5->FBRD = 52;
    //  UART Line Control (UARTLCRH) pag.916
    UART5->LCRH = (0x3<<5)|(1<<4);//DETERMINA EL TAMAÑO DEL ENVIO 8 BITS
    //  UART Clock Configuration(UARTCC) pag.939
    UART5->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART5->CTL = (1<<0) | (1<<8) | (1<<9);



}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART5->FR & (1<<4)) != 0 );
    v = UART5->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART5->FR & (1<<5)) != 0 );
    UART5->DR = c;
}
extern void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}

extern char * readString(char delimitador)
{

   int i=0;
   char *string = (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       *(string+i) = c;
       i++;
       if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       c = readChar();
   }

   return string;

}
//Experimento 2

//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 

