#include "io430.h"

#define COLUNAS_DIR     P2DIR
#define COLUNAS         P2IN
#define LINHAS_DIR      P6DIR
#define LINHAS          P6OUT
#define BITS_LINHA      BIT4+BIT5+BIT6+BIT7
#define BITS_COLUNA     BIT0+BIT1+BIT2

unsigned char TAB_TEC[13]= {"\00""321654987E0A"}; 
unsigned char TAB_LIN[5]={BIT4,BIT5,BIT6,BIT7,0X00};

void configura_teclado (void)
{
  COLUNAS_DIR&=~BITS_COLUNA; // Define as colunas como entrada
  LINHAS_DIR|=BITS_LINHA;    // Define as linhas como saída
}

/*------------------------------------------------------------------------------
Nome:       teclado
Descrição:  Função para ler a entrada do teclado matricial.
Entrada:         -
Saída:           -
------------------------------------------------------------------------------*/
unsigned char teclado (void)
{
  unsigned char *APT_TEC=TAB_TEC;
  unsigned char *APT_LIN=TAB_LIN;
  LINHAS|=BITS_LINHA;               
   if(COLUNAS & BITS_COLUNA)     
    {
      __delay_cycles(30000); 
      LINHAS=*APT_LIN;           
      while(((COLUNAS & (BITS_COLUNA))== 0X00) && (*APT_LIN != '\00'))
      {

        APT_TEC=APT_TEC+3;
        APT_LIN=APT_LIN+1;
        LINHAS=*APT_LIN;
      }
     if(*APT_LIN=='\00')
      APT_TEC=TAB_TEC;
     else if(COLUNAS&BIT0)
      APT_TEC=APT_TEC+1;
     else if(COLUNAS&BIT1)
      APT_TEC=APT_TEC+2;
     else if(COLUNAS&BIT2)
      APT_TEC=APT_TEC+3;
     else
      APT_TEC=TAB_TEC;
    }
   return (*APT_TEC);
}       