#include "io430.h"
#include "teclado.h"
#include "LCD.h"
#define SUB_SYSTEM	0

# define INI_TAR	3036		//Contagem de 100000 pulsos de clock(F=10HZ)	| 1000k/100k=10 |65536-15536= 50000 Prático 50001

  unsigned char indica;                 // cria variáveis indica e tst

  char seg1=0x30;                       // zera as variáveis
  char seg2=0x30;
  char min1=0x30;
  char min2=0x30;
  char hor1=0x30;
  char hor2=0x30;

  const char nome[]="TIMER$";           // declara os strings que aparecerão no LCD
  const char tempo[]="00:00:00$";
/*------------------------------------------------------------------------------
Nome:       zera
Descrição:  Zera o cronômetro
Entrada:         -
Saída:           -
------------------------------------------------------------------------------*/
void zera(void)
{
  hor1=0x30;                            // zera as variáveis
  hor2=0x30;
  min1=0x30;
  min2=0x30;
  seg1=0x30;
  seg2=0x30;
  
  envia_comando(0xC4);                  // envia o cronômetro zerado para o LCD
  envia_string(tempo);
}

/*------------------------------------------------------------------------------
Nome:       dispara
Descrição:  Dispara o cronômetro
Entrada:         -
Saída:           -
------------------------------------------------------------------------------*/
void dispara(void)
{
  seg1++;                               // incrementa segundo 1
  __delay_cycles(10000);
  envia_comando(0xCB);                  // envia segundo 1 para o LCD
  envia_dado(seg1);
  
  if(hor2==0x32&&hor1==0x33&&min2==0x35&&min1==0x39&&seg2==0x35&&seg1==0x39)    // se 23:59:59
  {
    zera();                             // zera o cronômetro
  }
  
  else if(hor1==0x39&&min2==0x35&&min1==0x39&&seg2==0x35&&seg1==0x39)           // se 09:59:59
  {
  seg1=0x30;                            // zera as variáveis
  seg2=0x30;
  min1=0x30;
  min2=0x30;
  hor1=0x30;
  hor2++;                               // incrementa hora 2
  __delay_cycles(10000);
  
  envia_comando(0xCB);                  // envia as variáveis para o LCD
  envia_dado(seg1);
  envia_comando(0xCA);
  envia_dado(seg2);
  envia_comando(0xC8);
  envia_dado(min1);
  envia_comando(0xC7);
  envia_dado(min2);
  envia_comando(0xC5);
  envia_dado(hor1);
  envia_comando(0xC4);
  envia_dado(hor2);
  __delay_cycles(10000);
  }
  
  else if(min2==0x35&&min1==0x39&&seg2==0x35&&seg1==0x39)                       // se 00:59:59
    {
  seg1=0x30;                            // zera as variáveis
  seg2=0x30;
  min1=0x30;
  min2=0x30;
  hor1++;                               // incrementa hora 1
  __delay_cycles(10000);
  
  envia_comando(0xCB);                  // envia as variáveis para o LCD
  envia_dado(seg1);
  envia_comando(0xCA);
  envia_dado(seg2);
  envia_comando(0xC8);
  envia_dado(min1);
  envia_comando(0xC7);
  envia_dado(min2);
  envia_comando(0xC5);
  envia_dado(hor1);
  __delay_cycles(10000);
  }
  
  else if(min1==0x39&&seg2==0x35&&seg1==0x39)                                   // se 00:09:59
    {
  seg1=0x30;                            // zera as variáveis
  seg2=0x30;
  min1=0x30;
  min2++;                               // incrementa minuto 2
   __delay_cycles(10000);
  
  envia_comando(0xCB);                  // envia as variáveis para o LCD
  envia_dado(seg1);
  envia_comando(0xCA);
  envia_dado(seg2);
  envia_comando(0xC8);
  envia_dado(min1);
  envia_comando(0xC7);
  envia_dado(min2);
   __delay_cycles(10000);
  }
  
  else if(seg2==0x35&&seg1==0x39)                                               // se 00:00:59
  {
  seg1=0x30;                            // zera as variáveis
  seg2=0x30;
  min1++;                               // incrementa minuto 1
   __delay_cycles(10000);
  
  envia_comando(0xCB);                  // envia as variáveis para o LCD
  envia_dado(seg1);
  envia_comando(0xCA);
  envia_dado(seg2);
  envia_comando(0xC8);
  envia_dado(min1);
  __delay_cycles(10000);
  }
  
  else if(seg1==0x39)                                                           // se 00:00:09
  {
   seg1=0x30;                           // zera segundo 1
   seg2++;                              // incrementa segundo 2
    __delay_cycles(10000);
  
  envia_comando(0xCB);                  // envia as variáveis para o LCD
  envia_dado(seg1);
  envia_comando(0xCA);
  envia_dado(seg2);
  __delay_cycles(10000);
  }
  
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
 #ifdef SUB_SYSTEM
  BCSCTL2 |= SELS;				//Define XT2 como fonte de clock do SMCLK
 BCSCTL2 |= DIVS1+DIVS0;		//Divide fonte por 8 SMCLK=500kHz

#endif
// -----------------------------------------------------------------------------
  
// ----------------------- Configuração do Timer_A -----------------------------
  // Define valor inicial do TAR
  TAR=INI_TAR;
  // Núcleo do Timer
#ifdef SUB_SYSTEM
 TACTL = TASSEL1 +          	// Fonte do clock: SMCLK ( 4MHz )

#endif	

	      ID1 +	ID0+				// Divide FONTE CLOCK/4

          MC_2 +               	// Modo de contagem: crescente contínua
		  TAIE; 	    		// Habilita a interrupção do núcleo

// -----------------------------------------------------------------------------

// ----------------------- Configuração da porta -----------------------------
  P1DIR |= BIT0; 				// Configura P1.0 como saída
  P1OUT |= BIT0; 				// ON

#ifdef SUB_SYSTEM
  // Configuração da porta para inspeção do SMCLK
  P1DIR |=BIT4; 				// Configura P1.4 como saida
  P1SEL |=BIT4; 				// Configura P1.4 como função especial SMCLK
#else	
  // Configuração da porta para inspeção do ACLK
  P2DIR |=BIT0; 				// Configura P2.0 como saida
  P2SEL |=BIT0; 				// Configura P2.0 como função especial ACLK
#endif	
// -----------------------------------------------------------------------------

 __bis_SR_register(GIE);   	// Habilita interrupção geral
 
  configura_LCD();
  programa_LCD();
  configura_teclado();
  
  envia_comando(0x85);                  // envia as strings para o LCD
  envia_string(nome);
  envia_comando(0xC4);
  envia_string(tempo);
  char tst=0;                           // cria variável tst
  
  while (1)
  {
    unsigned char tecla=teclado();
    if(tecla)
     tst=tecla;
      switch(tst)
      {
        case 0x30:                      // caso tecla 0 pressionada
          zera();                       // zera o cronômetro
          indica=0;                     // zera indica
          break;
        case 0x31:                      // caso tecla 1 pressionada
          indica=1;                     // coloca 1 no indica
          break;
        case 0x32:                      // caso tecla 2 pressionada
          indica=0;                     // zera o indica e volta, parando a contagem
          break;
        case 0x33:                      // caso tecla 3 pressionada
          indica=1;                     // coloca 1 no indica
          break;
        default:
          if (indica==0)                // se indica for igual a 1
            {
              zera();                // continua a contagem 
            }                           // se não, volta a testar tecla
          break;
      }
  }
}
// TA0_A1 Interrupt vector
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void)
{
TACTL &= ~MC_3;								//Parar o Timer
TAR=INI_TAR;
		TACTL |= MC_2;						//Disparar o Timer em modo continuo

    

switch(__even_in_range(TA0IV,0x0A))			//Só verifica os valores entre 0 e 0x0A
  	{
      case TA0IV_NONE: break;              	// Vector  0:  No interrupt
      case TA0IV_TACCR1:                   	// Vector  2:  TACCR1 CCIFG
       break;
      case TA0IV_TACCR2:              		// Vector  4:  TACCR2 CCIFG
        break;
      case TA0IV_6: break;                  // Vector  6:  Reserved CCIFG
      case TA0IV_8: break;                  // Vector  8:  Reserved CCIFG
      case TA0IV_TAIFG: 					// Vector 10:  TAIFG
  		// Define valor inicial do TAR
  				  if (indica==1)
    {
      dispara();
      P1OUT ^= BIT1;
    }
		break;              				
      default: 	break;
  	}
}
  

  

