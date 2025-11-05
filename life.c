/*
============================================================
  Fichero: life.c
  Creado: 03-11-2025
  Ultima Modificacion: dilluns, 3 de novembre de 2025, 20:54:08
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>

#include "zxou.h"

#define WORLD 32
#define AWORLD (WORLD*WORLD)

#define POS(X,Y) ((X)+WORLD*(Y))
#define FRM(X) ((X)%WORLD)

#define CELL 128

void gdu_init() {
	gdu(CELL,60,126,255,255,255,255,126,60);
}

void cell_init(byte* w) {
	//inicia la celda
	byte* ptw=w;
	while(ptw!=w+AWORLD) *ptw++=0;
}

void cell_move(byte* d,byte* o) {
	//copia un mundo en otro
	byte *pto,*ptd;
	pto=o;
	ptd=d;
	while(pto!=o+AWORLD) *ptd++=*pto++;
}

void cell_insert(byte* w,byte x,byte y) {
	//se coloca una celda en una posicion
	x=FRM(x);
	y=FRM(y);
	*(w+POS(x,y))=1;
}

void cell_erase(byte* w,byte x,byte y) {
	//se coloca una celda en una posicion
	x=FRM(x);
	y=FRM(y);
	*(w+POS(x,y))=0;
}

byte cell_get(byte* w ,byte x,byte y) {
	x=FRM(x);
	y=FRM(y);
	return *(w+POS(x,y));
}

byte cell_neighbours(byte* w,byte x,byte y) {
	//cuenta los vecinos que hay alrededor
	int xx=x;
	int yy=y;
	byte cells=0;
	for(int i=-1;i<=1;i++) {
		for(int j=-1;j<=1;j++) {
			if(i!=0 || j!=0) {
				int px=FRM(xx)+i;
				int py=FRM(yy)+j;
				if(cell_get(w,px,py)) cells++;
			}
		}
	}
	return cells;
}

void cell_show(byte* w,double p) {
	//muestra el mundo
	byte* ptr=w;
	locate(0,0);
	while(ptr!=w+AWORLD) {
		if(*ptr==1) printc(CELL);
		else printc(' ');
		ptr++;
	}
	show;
	pause(p);
}

void cell_random(byte* w,int cells) {
	//coloca de manera aleatoria las celdas ocupadas
	cells=(cells<0)?-cells:cells;
	cells=(cells>AWORLD)?AWORLD:cells;
	for(int k=0;k<cells;k++) {
		int x,y;
		do {
			x=rnd(0,AWORLD-1);
			y=rnd(0,AWORLD-1);
		}while(cell_get(w,x,y)==1);
		cell_insert(w,x,y);
	}
}

//condiciones de juego
#define NACIMIENTO(N) ((N==3)) //condicion de nacimiento de la celula
#define MUERTE(N) (((N)<=1) || ((N)>3)) //condicion de muerte de la celula
#define INICIALES 100 //numero de celdas iniciales repartidas aleatoriamente
#define PAUSA .5 //pausa entre cada generacion
					
begin_program
	randomize(-1);
	gdu_init();
	byte world[AWORLD];
	byte cworld[AWORLD];
	cell_init(world);
	cell_random(world,INICIALES);
	while(inkey('q')==0) {
		cell_show(world,PAUSA);
		cell_init(cworld);
		for(int i=0;i<WORLD;i++) {
			for(int j=0;j<WORLD;j++) {
				byte cell=cell_get(world,i,j);
				byte neig=cell_neighbours(world,i,j);
				if(cell==0 && NACIMIENTO(neig)) cell_insert(cworld,i,j);
				else if(cell==1 && !MUERTE(neig)) cell_insert(cworld,i,j);
			}
		}
		cell_move(world,cworld);
		listen;
	}
end_program

	
	

	

	


	



