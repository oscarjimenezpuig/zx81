/*
============================================================
  Fichero: prueba.c
  Creado: 05-11-2025
  Ultima Modificacion: mié 05 nov 2025 12:24:12
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

/* PRUEBA DE DESPLAZAMIENTO PIXEL A PIXEL */

#include <stdio.h>

#include "zxou.h"

#define AND 1
#define OR 2
#define XOR 3
#define EXC 4 //no tiene en cuenta el valor previo de la posicion

#define inscrx (px>=0 && px<SCRWP)
#define inscry (py>=0 && py<SCRHP)

void pacip(byte chr,byte operation,int x,int y) {
	//coloca un caracter en cualquier posicion de la pantalla
	byte *ptc,*init;
	init=ptc=pointer(OASC+8*chr);
	int px;
	int py=y;
	while(ptc!=init+8) {
		if(inscry) {
			px=x;
			for(byte f=128;f>=1;f=f>>1) {
				if(inscrx) {
					byte val=(*ptc & f);
					if(operation!=EXC) {
						byte vip=plotted(px,py);
						switch(operation) {
							case AND:
								val=val & vip;
								break;
							case OR:
								val=val | vip;
								break;
							default:
								val=val ^ vip;
						}
						if(val) plot(px,py);
						else unplot(px,py);
					}
				}
				px++;
			}
			py++;
		}
		ptc++;
	}
}

void gdu_def() {
	gdu(128,60,126,219,255,195,231,255,153);
}

int ghostx=100;
int ghosty=100;

void move_gdu() {
	const int ADV=2;
	if(inkey('i')) ghosty-=ADV;
	else if(inkey('k')) ghosty+=ADV;
	if(inkey('j')) ghostx-=ADV;
	else if(inkey('l')) ghostx+=ADV;
}

begin_program
	gdu_def();
	while(inkey('q')==0) {
		pacip(128,OR,ghostx,ghosty);
		line(0,0,255,255);
		show;
		pause(0.01);
		cls;
		listen;
		move_gdu();
	}
end_program
				
		




