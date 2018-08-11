/**
**===========================================================================
**
**  Pinos Utilizados
**  PB0 = seg a, PB1 = seg b, PB2 = seg c, PB3 = seg d, PB4 = seg e, PB5 = seg f, PB6 = seg g
**  Transistores
**  PA0 = T0, PA1 = T1, PA4 = T2, PA5 = T3
**
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

#define um 0
#define dm 1
#define uh 2
#define dh 3

uint16_t cathode[]={cathode_0,cathode_1,cathode_2,cathode_3,cathode_4,cathode_5,cathode_6,cathode_7,cathode_8,cathode_9};

uint16_t seletor[]={GPIO_ODR_ODR_0,GPIO_ODR_ODR_1,GPIO_ODR_ODR_4,GPIO_ODR_ODR_5};

#define TEMPO_TROCA 600				//quantidade de milisegundos

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
	uint16_t i = 0;
	uint16_t display[4],valor[4]={0,0,0,0};

	//Enabling GPIOS clock
		RCC->AHB1ENR=RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOBEN;

	//Enabling TIM10 clock
		RCC->APB2ENR|=RCC_APB2ENR_TIM10EN;

	//Setting UIF overflow time
		TIM10->PSC = 99;
		TIM10->ARR = 79;
		TIM10->CR1|=TIM_CR1_CEN;//starts

	//habilita os pinos do GPIOA como saídas digitais
		GPIOA->MODER &=~ (GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER4|GPIO_MODER_MODER5);
		GPIOA->MODER|=(GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0|GPIO_MODER_MODER4_0|GPIO_MODER_MODER5_0);//controla os transistores

	//habilita os pinos do GPIOB como saídas digitais
		GPIOB->MODER &=~ (GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER2|GPIO_MODER_MODER3|GPIO_MODER_MODER4|GPIO_MODER_MODER5|GPIO_MODER_MODER6);
	    GPIOB->MODER|=(GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0|GPIO_MODER_MODER2_0|GPIO_MODER_MODER3_0|GPIO_MODER_MODER4_0|GPIO_MODER_MODER5_0|GPIO_MODER_MODER6_0);//controla os segmentos dos catodos

  while (1)
  {
	  display[i]=cathode[valor[i]];

	  GPIOB->ODR &=~ (GPIO_ODR_ODR_0|GPIO_ODR_ODR_1|GPIO_ODR_ODR_2|GPIO_ODR_ODR_3|GPIO_ODR_ODR_4|GPIO_ODR_ODR_5|GPIO_ODR_ODR_6);
	  GPIOA->ODR &=~ (GPIO_ODR_ODR_0|GPIO_ODR_ODR_1|GPIO_ODR_ODR_4|GPIO_ODR_ODR_5);
	  GPIOA->ODR |= seletor[i];
	  GPIOB->ODR |= display[i];

	  if(TIM10->SR&TIM_SR_UIF){						//verifica o overflow

	      TIM10->SR  =~ TIM_SR_UIF;				 	//zera o contador de tempo

	  	  timerMs++;

	  	  if(timerMs == TEMPO_TROCA){		    	//avisa overflow do timerMs
	  		  timerMs = 0;
	  		  FtimerS = 1;
	  	  }
	  	  if (i == 3){
	  		  i = 0;
	  	  }
	  	  else
	  		  i++;
	  }

	  if(FtimerS){
		  FtimerS = 0;

		  if(valor[um] < 9){
			  valor[um]++;
		  }
	  	  else{
	  		valor[um] = 0;
	  		  if(valor[dm] < 5){
	  			valor[dm]++;
	  		  }
	  		  else{
	  			valor[dm] = 0;
	  			  if(valor[uh] < 9 && valor[dh] < 2){
	  				valor[uh]++;
	 	  		  }
	  			  else if(valor[uh] < 3 && valor[dh] == 2){
	  				valor[uh]++;
	  			  }
	  			  else{
	  				valor[uh] = 0;
	  				  if(valor[dh] < 2)
	  					valor[dh]++;
	  				  else
	  					valor[dh] = 0;
	  			  }
	  		  }
	  	  }
	  }//end os FtimerS
  }//end of while
}//end of main
