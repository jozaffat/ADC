#include "lib/include.h"

int main(void)
{

    volatile uint16_t digital[6]; //cantidad de canales

    int k=0;
    Configurar_SysTick();
    Configurar_PLL();  //Confiuracion de velocidad de reloj
    Configura_Reg_ADC0();
    Configurar_UART1();
    
  
    char num[4];
    
    while(1)
    {

         ADC0->PSSI |= (1<<0)| (1<<3);  //INICIA EL MUESTREO 
             
         while((ADC0->RIS & (1<<0)) == 0){} ;   /* CICLOS ESPEFICICOS PARA  SS0*/
         //direccion de memoria del SS0
         //EL SS0 puede leer hasta 8 datos de los cuales usaremos 5
         
         digital[0]= ADC0->SSFIFO0&0xFFF;   
         digital[1]= ADC0->SSFIFO0&0xFFF;
         digital[2]= ADC0->SSFIFO0&0xFFF;
         digital[3]= ADC0->SSFIFO0&0xFFF;
         digital[4]= ADC0->SSFIFO0&0xFFF;
         
         utoa(digital[0],num,10);//Pasa de una variable usingned a string
         enviar(num);//envia a traves del UAR1 
        
         utoa(digital[1],num,10);
         enviar(num);

         utoa(digital[2],num,10);
         enviar(num);

         utoa(digital[3],num,10);
         enviar(num);

         utoa(digital[4],num,10);
         enviar(num);

         while((ADC0->RIS & (1<<3)) == 0){} ; /* CICLOS ESPEFICICOS PARA CADA SENCUENSIDOR SS3*/

         //EL SS3 puede leer  1 dato el cual usaremos para completar los 6 entradas
         digital[5]= ADC0->SSFIFO3& 0xFFF;
           
         
         
        utoa(digital[5],num,10);
         enviar(num);
         
        ADC0->ISC = (1<<2)|(1<<1); //termina la el muestreo de PSSI

          
        ADC0->ISC = 8;          /* clear coversion clear flag bit*/
			/*control Green PF3->LED */
		
    }

}
