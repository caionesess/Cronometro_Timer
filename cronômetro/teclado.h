void configura_teclado(void);
unsigned char teclado(void);
void tempo_repique(unsigned int);
#define COLUNAS_DIR     P2DIR
#define COLUNAS         P2IN
#define LINHAS_DIR      P6DIR
#define LINHAS          P6OUT
#define BITS_LINHA      BIT4+BIT5+BIT6+BIT7
#define BITS_COLUNA     BIT0+BIT1+BIT2
