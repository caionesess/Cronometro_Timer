#include "io430.h" 

#define LCM_DADO_DIR     P5DIR          // estabelecendo os bits onde os dispositivos est�o conectados e refer�ncias
#define LCM_DADO_OUT     P5OUT          // porta de dado e controle do LCM (Liquid Crystal Module)
#define LCM_CTL_DIR      P3DIR
#define LCM_CTL_OUT      P3OUT

#define RS               BIT7           // defini��o dos bits e da m�scara de envio para o LCD
#define EN               BIT6

#define TEMPO_LCD        25000          // define a temporiza��o 

char mensagem1[]="caio$";

/*---------------------------------------------------------------------------
Fun��o: TempoConfiguravel
Descri��o: Carrega um contador e realiza decrementos sucessivos at� zerar.
Entrada: unsigned int - per�odo da temporiza��o
Sa�da: -
---------------------------------------------------------------------------*/
void TempoConfiguravel(unsigned int periodo)
{
unsigned int cont;              // Variavel auxiliar para contagem
cont = periodo;                 // contagem de tempo
while (cont != 0)
cont--;
}
/*--------------------------------------------------------------
Fun��o: configura_LCD
Descri��o: Configura��o das portas conectadas ao LCM
Entrada: -
Sa�da: -
--------------------------------------------------------------*/
void configura_LCD (void)
{
  LCM_DADO_DIR=0xFF;                            // todos os bits de dados como sa�da
  LCM_DADO_OUT=0x00;                            // limpa sa�da
  LCM_CTL_DIR|=RS+EN;                           // bits da porta de controle como sa�da
  LCM_CTL_OUT&=~(RS+EN);                        // limpa pinos
}
/*--------------------------------------------------------------
Fun��o: envia_comando
Descri��o: recebe um par�metro de entrada e escreve-o no barramento de dados do LCM (porta 5)
Entrada: comando (tipo char)
Sa�da: -
--------------------------------------------------------------*/
void envia_comando (char comando)
{
  LCM_DADO_OUT=comando;
  LCM_CTL_OUT&=~(RS+EN);
  TempoConfiguravel(TEMPO_LCD);
  LCM_CTL_OUT|=EN;
  TempoConfiguravel(TEMPO_LCD);
  LCM_CTL_OUT&=~EN;
}
/*--------------------------------------------------------------
Fun��o: programa_LCM
Descri��o: Programa��o do acionamento do LCM
Entrada: -
Sa�da: -
--------------------------------------------------------------*/
void programa_LCD (void)
{
  envia_comando(0x56);                          // define n� de linhas do display, n� de bits do barramento e formato do caracter
  envia_comando(0x14);                          // ativa display e cursor ativos sem piscar                      
  envia_comando(0x06);                          // define o deslocamento do cursor p/ direita
  envia_comando(0x01);                          // limpa display
}
/*--------------------------------------------------------------
Fun��o: envia_dado
Descri��o: recebe um par�metro de entrada e escreve-o no barramento de dados do LCM (porta 5)
Entrada: dado (tipo char)
Sa�da: -
--------------------------------------------------------------*/
void envia_dado (char dado)
{
  LCM_DADO_OUT=dado;
  LCM_CTL_OUT|=RS;
  LCM_CTL_OUT&=~(EN);
  TempoConfiguravel(TEMPO_LCD);
  LCM_CTL_OUT|=EN;
  TempoConfiguravel(TEMPO_LCD);
  LCM_CTL_OUT&=~(EN);
}
/*--------------------------------------------------------------
Fun��o: envia_string
Descri��o: envia os caracteres de uma mensagem passada como par�metro para a fun��o envia_dado
Entrada: *msg
Sa�da: -
--------------------------------------------------------------*/
void envia_string (const char *msg)
{
  unsigned char cont=0;                 // zera o cont            
      while (msg[cont]!='$')            // enquanto o elemento for diferente desse caracter                    
    {
      envia_dado(msg[cont]);            // envia dado da mensagem
      cont++;                           // incrementa o cont
    }
}
/*--------------------------------------------------------------
Fun��o: conta
Descri��o: conta o tamanho da palavra
Entrada: *msg
Sa�da: tamanho
--------------------------------------------------------------*/
unsigned char conta (char *msg)
{
  unsigned int indice=0;                // zera o indice
  unsigned char tamanho=0;              // zera o tamanho
  
  while(msg[indice]!='$')               // enquanto caracter for diferente de $
  {
    tamanho++;                          // incrementa o tamanho
    indice++;                           // incrementa o indice
  }
  return(tamanho);                      // retorna o tamanho
}
/*--------------------------------------------------------------
Fun��o: desloca_mensagem
Descri��o: desloca a mensagem para a esquerda
Entrada: *msg
Sa�da: -
--------------------------------------------------------------*/
void desloca_mensagem (char *msg)
{
  while(1)
  {
  envia_string((const char *)msg);      // chama o envia_string convertendo msg para const
  unsigned char desloca=conta(msg);     // atribui tamanho � desloca
  unsigned int indice=0;                // zera o indice
  unsigned int primeiro;                // cria vari�vel primeiro
  primeiro=msg[indice];                 // atribui o elemento a vari�vel primeiro
    
    while(indice<desloca-1)             // enquanto desloca for menor que o tamanho da palavra
    {
      indice++;                         // incrementa o indice
      msg[indice-1]=msg[indice];        // move caracter por caracter para a esquerda
    }
  msg[desloca-1]=primeiro;              // move o primeiro caracter para o �ltimo
  envia_comando(0x01);                  // limpa display
  TempoConfiguravel(TEMPO_LCD);
  }
}
