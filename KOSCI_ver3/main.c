/*-------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - PROJEKT GRY KOSCI
					autor: JAN GADEK
					wersja: 19.01.2023r.
----------------------------------------------------------------------------*/
/*
TO PLAY GAME!!! PRESS S1-S4
1)Wybieramy gracza nr1-S1,nr2-S2,nr3-S3,nr4-S4
2)Zostaly wylosowane kosci oraz mamy mozliwosc wyboru kosci do wymiany poprzez przyciski s3-lewo s4-prawo s2-wybierz s1-losuj
3)Po wylosowaniu wyswietla wylosowane kosci
4)gracz ponownie otrzymuje opcje wyboru(w sumie 3 razy wybiera)
5)Wybieramy gracza nr2 poprzez przycisk s4
6)analogiczne postepowanie tak jak w przypadku gracza nr1
7)wyswietlamy kosci graczy
8)naciskamy S1 aby zagrac ponownie
*/	
	
#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "klaw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>                                                             
#include <stdio.h>      
volatile uint8_t S1_press=0; 	//stan przycisku S1
volatile uint8_t S2_press=0;	//stan przycisku S2
volatile uint8_t S3_press=0;	//stan przycisku S3
volatile uint8_t S4_press=0;	//stan przycisku S4
char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};



void roll(uint8_t *tab,uint8_t n)												//funkcja losujaca kosci(1-6)
{
		for (uint8_t i=0 ;i<n;i++) tab[i] = rand()%6+1;
}
void printrandom(uint8_t *tab,uint8_t n)								//funkcja wypisujaca wylosowane liczby
{
	for(uint8_t i =0;i<n;i++)
			{
			sprintf(display," %d",tab[i]);
			LCD1602_Print(display);
			}
}

void toroll(uint8_t *tab, uint8_t a)
{
	//uint8_t tab2[5]={0};
	if(a==7)tab[a-7]=1;
	else if (a == 9) tab[1]=1;
	else if (a == 11) tab[2]=1;
	else if (a == 13) tab[3]=1;
	else if (a == 15) tab[4]=1;
}
void rollsome( uint8_t *tab,uint8_t n,uint8_t nr)     	//funkcja umozliwiajaca wybranie poszczegolnych kosci do losowania oraz wyswietlenie ich
{
	for(uint8_t p = 1;p<4;p++)
	{
		S1_press=0;
		S2_press=0;
		uint8_t a=7;
	//	uint8_t tab1[5];
		uint8_t tab2[5]={0};
			LCD1602_ClearAll();
			LCD1602_SetCursor(0,0);
		sprintf(display,"Gracz%d",nr);
			LCD1602_Print(display);
		printrandom(tab,5);
			LCD1602_Print(display);
			LCD1602_SetCursor(0,1);
		sprintf(display,"Roll %d: ",p);
			LCD1602_Print(display);
		
		LCD1602_SetCursor(6,1);
				printrandom(tab2,5);
		
			LCD1602_SetCursor(a,1);
		
		sprintf(display,"^");//wys los liczby
			LCD1602_Print(display);
		uint8_t t=0;
		while(1)
		{
			
			if(S3_press) 																			//przesuwanie wskaznika w lewo w zaleznosci od stanu przycisku 
			{
				S3_press=0;	
					LCD1602_ClearAll();
				sprintf(display,"Gracz%d:",nr);
					LCD1602_Print(display);
					LCD1602_SetCursor(6,0);
				printrandom(tab,5);
					LCD1602_SetCursor(0,1);
				sprintf(display,"Roll %d: ",p);
					LCD1602_Print(display);
				
				LCD1602_SetCursor(6,1);
				printrandom(tab2,5);
				
					LCD1602_SetCursor(a,1);
				a=a-2;		
			}			
			if(S4_press) 																			//przesuwanie wskaznika w prawo w zaleznosci od stanu przycisku 
			{
				S4_press=0;
					LCD1602_ClearAll();
				sprintf(display,"Gracz%d:",nr);
					LCD1602_Print(display);
					LCD1602_SetCursor(6,0);
				printrandom(tab,5);
					LCD1602_SetCursor(0,1);
				sprintf(display,"Roll %d: ",p);
					LCD1602_Print(display);
				
				LCD1602_SetCursor(6,1);
				printrandom(tab2,5);
				
					LCD1602_SetCursor(a,1);
				a=a+2;	
			}	
				LCD1602_SetCursor(a,1);													//
			sprintf(display,"^");															//wyswitlanie wskaznika
				LCD1602_Print(display);													//
			
			if(S1_press) 																			//zapisanie opcji i wyjscie z funkcji
			{
				S1_press=0;
				break;
			}
			
			if(S2_press)																			//potwierdzenie wybrania kosci do losowania
			{
				S2_press = 0;
				toroll(tab2,a);
				//tab1[t]=(a-6)/2;
				
				t++;	
				if(t==5) break;
			}	
		}
		S1_press=0;
		
			for(uint8_t i=0; i<6; i++)
		{
			if(tab2[i]==1) tab[i]=rand()%6+1;
		}
				LCD1602_ClearAll();
			sprintf(display,"Gracz%d:",nr);
				LCD1602_Print(display);
				LCD1602_SetCursor(6,0);
			printrandom(tab,5);
			
	}
	DELAY(1000);
		LCD1602_ClearAll();
	sprintf(display,"THE NEXT PLAYER");
		LCD1602_Print(display);
		LCD1602_SetCursor(6,1);
	sprintf(display,"TURN");
		LCD1602_Print(display);
}		
		
void PORTA_IRQHandler(void){	// Podprogram obslugi przerwania od klawiszy S1,S2,S3,S4
	uint32_t buf;
	buf=PORTA->ISFR & (S1_MASK | S2_MASK | S3_MASK | S4_MASK);

	switch(buf)
	{
		case S1_MASK:	DELAY(10)
									if(!(PTA->PDIR&S1_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S1_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S1_press)
											{
												S1_press=1;
											}
										}
									}
									break;
		
		case S2_MASK:	DELAY(10)
									if(!(PTA->PDIR&S2_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S2_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S2_press)
											{
												S2_press=1;
											}
										}
									}
									break;
		case S3_MASK:	DELAY(10)
									if(!(PTA->PDIR&S3_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S3_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S3_press)
											{
												S3_press=1;
											}
										}
									}
									break;
		case S4_MASK:	DELAY(10)
									if(!(PTA->PDIR&S4_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S4_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S4_press)
											{
												S4_press=1;
											}
										}
									}
									break;
		default:			break;
	}	
	PORTA->ISFR |=  S1_MASK | S2_MASK | S3_MASK | S4_MASK;	// Kasowanie wszystkich bitów ISF
	NVIC_ClearPendingIRQ(PORTA_IRQn);
}


void results(uint8_t *tab1,uint8_t nr1,uint8_t *tab2,uint8_t nr2,uint8_t *tab3,uint8_t nr3,uint8_t *tab4,uint8_t nr4)
{
		LCD1602_ClearAll();
		LCD1602_SetCursor(0,0);
	for(uint8_t i=3;i>0;i--)
	{
			LCD1602_ClearAll();
		sprintf(display,"Results in %d s",i);
			LCD1602_Print(display);
		DELAY(1000);
	}
	while(!S1_press)
	{
			LCD1602_ClearAll();
		LCD1602_SetCursor(0,0);
		sprintf(display,"Gracz%d",nr1);
			LCD1602_Print(display);
		printrandom(tab1,5);
			LCD1602_SetCursor(0,1);
		sprintf(display,"Gracz%d",nr2);
			LCD1602_Print(display);
		printrandom(tab2,5);
		DELAY(1100);
		LCD1602_ClearAll();
		LCD1602_SetCursor(0,0);
		sprintf(display,"Gracz%d",nr3);
			LCD1602_Print(display);
		printrandom(tab3,5);
		LCD1602_SetCursor(0,1);
		sprintf(display,"Gracz%d",nr4);
			LCD1602_Print(display);
		printrandom(tab4,5);
		DELAY(1100);
			LCD1602_ClearAll();
			LCD1602_SetCursor(1,0);
			sprintf(display,"TO PLAY AGAIN");
			LCD1602_Print(display);
			LCD1602_SetCursor(3,1);
			sprintf(display,"PUSH S1 :)");
			LCD1602_Print(display);
		DELAY(1100);
	}
}

int main (void) 
{
	time_t t;
  srand(t);
	uint8_t gracz1[5]={0};
	uint8_t gracz2[5]={0};
	uint8_t gracz3[5]={0};
	uint8_t gracz4[5]={0};
	
	Klaw_Init();								// Inicjalizacja klawiatury
	Klaw_S2_4_Int();						// Klawisze S2, S3 i S4 zglaszaja przerwanie
	LCD1602_Init();		 					// Inicjalizacja LCD
	LCD1602_Backlight(TRUE);  	// Wlaczenie podswietlenia
	
	LCD1602_ClearAll();					// Wyczysc ekran
	LCD1602_Print("TO PLAY GAME!!");		// Wyswietl powitanie (ciag tekstowy)
	LCD1602_SetCursor(0,1);			// Ustaw kursor na poczatku drugiej linii
	LCD1602_Print("Press S1-S3");	// Wyswietl powitanie c.d.
		
  while(1)		// Poczatek petli glównej
	{
		time_t t;
		srand(t++);
		if(S1_press)
		{
			
			roll(gracz1,5);
			printrandom(gracz1,5);
			rollsome(gracz1,5,1);
			S1_press=0;
		}
		if(S2_press)	// Czy klawisz S2 wcisniety?
		{
			roll(gracz2,5);
			printrandom(gracz2,5);
			rollsome(gracz2,5,2);
			S2_press=0;	
		}
		if(S3_press)							// Czy klawisz S3 wcisniety?
		{
			roll(gracz3,5);
			printrandom(gracz3,5);
			rollsome(gracz3,5,3);
			S3_press=0;
		}
		if(S4_press)							// Czy klawisz S4 wcisniety?
		{
			roll(gracz4,5);
			printrandom(gracz4,5);
			rollsome(gracz4,5,4);
			results(gracz1,1,gracz2,2,gracz3,3,gracz4,4);
			S4_press=0;	
		}
		
	}
}
