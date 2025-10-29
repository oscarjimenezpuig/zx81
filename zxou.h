/*
============================================================
  Fichero: zxou.h
  Creado: 18-10-2025
  Ultima Modificacion: dimecres, 29 d’octubre de 2025, 20:37:55
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "memory.h"

//CONSTANTES

#define NORMAL 0
#define INVERSE 1

#define CURSX (OROM) //direccion de memoria donde se guarda el cursor x
#define CURSY (OROM+1) //direccion de memoria donde se guarda el cursor y

//METODOS (ordenes que se podran utilizar directamente)
//no estan presentes, pero se pueden utilizar todas las funciones matematicas

#define circle(X,Y,R) z_circle((X),(Y),(R))
#define cls z_cls()
#define gdu(I,A,B,C,D,E,F,G,H) z_gdu((I),(A),(B),(C),(D),(E),(F),(G),(H))
#define inkey(C) z_inkey((C))
#define line(A,B,C,D) z_line((A),(B),(C),(D))
#define locate(X,Y) z_locate((X),(Y))
#define normal z_mode(NORMAL)
#define inverse z_mode(INVERSE)
#define pause(S) z_pause((S))
#define peek(D) memory[(D)] //consigue el valor de la direccion de memoria D
#define plot(X,Y) z_plot((X),(Y))
#define poke(D,V) memory[(D)]=(V) //asigna a la direccion D el valor V
#define point(D) memory+(D) //puntero a la direccion de memoria D
#define printc(C) z_printc((C))
#define printn(N) z_printn((N))
#define prints(S) z_prints((S))
#define randomize(S) z_randomize((S))
#define rnd(A,B) z_rnd((A),(B))

void z_circle(byte cx,byte cy,byte radium);
//dibuja un circulo centrado en cx,cy con radio radium

void z_cls();
//limpiamos la memoria, se coloca el mode en normal y el cursor en el origen (el modo se mantiene)

void z_gdu(byte code,byte a,byte b,byte c,byte d,byte e,byte f,byte g,byte h);
//definicion de un gdu en el caracter c

byte z_inkey(char c);
//verifica que el caracter ha sido pulsado

void z_line(byte ox,byte oy,byte fx,byte fy);
//crea una linea de origen ox,oy y final fx,fy

void z_locate(byte x,byte y);
//colocamos el cursor en la posicion marcada

void z_mode(byte m);
//definimos el modo NORMAL o INVERSE, desde la posicion del locate hasta el final de pantalla

void z_pause(double seconds);
//hacer una pausa de un determinado numero de segundos

void z_plot(byte x,byte y);
//dibuja el punto en una posicion de la pantalla

void z_printc(byte chr);
//impresion de un caracter

void z_prints(char* str);
//impresion de una cadena

void z_printn(double num);
//impresion de un numero

void z_randomize(int a);
//establecimiento de la semilla del numero aleatorio, si a negativa, en funcion del tiempo

int z_rnd(int a,int b);
//nuemero aleatorio entre dos valores dados

double z_sin(double angle);
//calculo del seno de un angulo en radianes



