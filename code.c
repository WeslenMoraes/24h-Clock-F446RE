/**
**===========================================================================
**
**  Pinos Utilizados
**      PA0 = clock do 4017, PA1 = reset do 4017
**	PB0 = seg a, PB1 = seg b, PB2 = seg c, PB3 = seg d, PB4 = seg e, PB5 = seg f, PB6 = seg g
**	PC0 = b1, PC1 = b2, PC2 = b3
**===========================================================================
*/

/* Includes */

#include "stm32f4xx.h"

#define cathode_0                    ((uint32_t)0x03f)
#define cathode_1                    ((uint32_t)0X006)
#define cathode_2                    ((uint32_t)0X05b)
#define cathode_3                    ((uint32_t)0X04f)
#define cathode_4                    ((uint32_t)0X066)
#define cathode_5                    ((uint32_t)0X06d)
#define cathode_6                    ((uint32_t)0X07d)
#define cathode_7                    ((uint32_t)0X007)
#define cathode_8                    ((uint32_t)0X07f)
#define cathode_9                    ((uint32_t)0X06f)

#define TEMPO_TROCA 60000				//quantidade de milisegundos

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
	uint16_t timerMs = 0;
	uint16_t FtimerS = 0;
	uint16_t dh = 0x03f;				//dh = dezena hora, uh = unidade hora, dm = dezena minuto, um = unidade minuto
	uint16_t uh = 0x03f;				//são usados para a contagem numérica do tempo
	uint16_t dm = 0x03f;
	uint16_t um = 0x03f;
	uint16_t cdh = 0x03f;				//cdh = convertido dezena hora, cuh, cdm, cum
	uint16_t cuh = 0x03f;				//são usados para converter os valores numéricos para valores 7-seg
	uint16_t cdm = 0x03f;
	uint16_t cum = 0x03f;
	uint16_t i = 0;
	uint16_t j[] = {cdh,cuh,cdm,cum};

	//Enabling GPIOS clock
		RCC->AHB1ENR=0X00000087;

	//Enabling TIM10 clock
		RCC->APB2ENR|=RCC_APB2ENR_TIM10EN;

	//Setting UIF overflow time
		TIM10->PSC = 199;
		TIM10->ARR = 79;
		TIM10->CR1|=TIM_CR1_CEN;//starts

	//habilita os pinos do GPIOA como saídas digitais
		GPIOA->MODER|=0x5;				//controla o contador habilitador dos displays

	//habilita os pinos do GPIOB como saídas digitais
		GPIOB->MODER=0x1555;			        //controla os segmentos dos catodos

	//habilita os pinos do GPIOC como entradas digitais
		GPIOC->MODER=0x0;                                //lê os botões

  while (1)
  {
	  //Conversão número -> número no catodo
	  if(dh == 0) cdh = cathode_0;
	  else if(dh == 1) cdh = cathode_1;
	  else if(dh == 2) cdh = cathode_2;
	  if(uh == 0) cuh = cathode_0;
	  else if(uh == 1) cuh = cathode_1;
	  else if(uh == 2) cuh = cathode_2;
	  else if(uh == 3) cuh = cathode_3;
	  else if(uh == 4) cuh = cathode_4;
	  else if(uh == 5) cuh = cathode_5;
	  else if(uh == 6) cuh = cathode_6;
	  else if(uh == 7) cuh = cathode_7;
	  else if(uh == 8) cuh = cathode_8;
	  else if(uh == 9) cuh = cathode_9;
	  if(dm == 0) cdm = cathode_0;
	  else if(dm == 1) cdm = cathode_1;
	  else if(dm == 2) cdm = cathode_2;
	  else if(dm == 3) cdm = cathode_3;
	  else if(dm == 4) cdm = cathode_4;
	  else if(dm == 5) cdm = cathode_5;
	  if(um == 0) cum = cathode_0;
	  else if(um == 1) cum = cathode_1;
	  else if(um == 2) cum = cathode_2;
	  else if(um == 3) cum = cathode_3;
	  else if(um == 4) cum = cathode_4;
	  else if(um == 5) cum = cathode_5;
	  else if(um == 6) cum = cathode_6;
	  else if(um == 7) cum = cathode_7;
	  else if(um == 8) cum = cathode_8;
	  else if(um == 9) cum = cathode_9;

	  j[3] = cdh;									//update j[] values
	  j[2] = cuh;
	  j[1] = cdm;
	  j[0] = cum;

	  if(i == 0){
		  GPIOA->ODR |= 0x2;						//reseta o 4017
	  	  GPIOB->ODR = j[i];						//joga info nos displays
	  }
	  else{
		  GPIOB->ODR = j[i];						//joga info nos displays
	  }

	  if(TIM10->SR&TIM_SR_UIF){						//verifica o overflow

	      TIM10->SR  =~ TIM_SR_UIF;				 	//zera o contador de tempo
	  	  GPIOA->ODR &=~ 0x3;			 			//joga o reset para zero e garante o clock em zero

	  	  timerMs++;

	  	  if(timerMs == TEMPO_TROCA){		    	//avisa overflow do timerMs
	  		  timerMs = 0;
	  		  FtimerS = 1;
	  	  }

	  	  if (i == 3){
	  		  i = 0;
	  	  }
	  	  else{
	  		  GPIOA->ODR |= 0x1;
	  		  GPIOA->ODR &=~ 0x1;
	  		  i++;
	  	  }
	  }

	  if(FtimerS){
		  FtimerS = 0;

		  if(um < 9){
			  um++;
		  }
	  	  else{
	  		  um = 0;
	  		  if(dm < 5){
	  			  dm++;
	  		  }
	  		  else{
	  			  dm = 0;
	  			  if(uh < 9 && dh < 2){
	  				  uh++;
	 	  		  }
	  			  else if(uh < 3 && dh == 2){
	  				  uh++;
	  			  }
	  			  else{
	  				  uh = 0;
	  				  if(dh < 2)
	  					  dh++;
	  				  else
	  					  dh = 0;
	  			  }
	  		  }
	  	  }
	  }//end os FtimerS
  }//end of while
}//end of main
