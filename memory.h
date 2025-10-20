/*
============================================================
  Fichero: memory.h
  Creado: 18-10-2025
  Ultima Modificacion: diumenge, 19 d’octubre de 2025, 19:38:15
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

//CONSTANTES

//dimensiones de la pantalla
#define SCRWC 32 //ancho en caracteres
#define SCRHC 32 //alto en caracteres
#define SCRWP (SCRWC*8) //ancho en pixeles
#define SCRHP (SCRHC*8) //alto en pixeles

//dimenion del pixel
#define DPIX 3 //definicion del tamaño del pixel

//numero de teclas guardadas
#define DKPL 5

//direcciones de la memoria
#define OSCR 0 //inicio de la pantalla
#define DSCR (SCRWP*SCRHP/8) //dimension de la pantalla
#define OINV (OSCR+DSCR) //origen de los datos de inversion
#define DINV (SCRWC*SCRHC/8) //dimension de los datos de inversion
#define OKEY (OINV+DINV) //inicio del guardado de las teclas
#define DKEY DKPL //dimension de memoria dedicada a guardar las teclas pulsadas
#define OASC (OKEY+DKEY) //inicio de la memoria donde se guardan los caracteres
#define DASC (256*8) //dimension de la memoria dedicada a los ASCII
#define OROM (OASC+DASC) //rom, dedicada a guardar diferentes valores que no conviene cambiar
#define DROM 2 //dimension de la rom
#define ORAM (OROM+DROM) //ram, a disposicion del usuario
#define DRAM 1024 //dimension de la ram

//dimension total de la memoria
#define DMEM (DSCR+DINV+DKEY+DASC+DROM+DRAM)

//banderas
#define MODEFLG 1 //cuando esta conectado el modeflag se considera inverse

//TIPOS

typedef unsigned char byte;
typedef unsigned short address;

//VARIABLES

extern byte memory[DMEM];

//FUNCIONES

void output();
//funcion que da la representacion de la pantalla

void input();
//funcion que registra la entrada por teclado

void program();
//funcion que contine el programa que se ha de ejecutar


