/*
============================================================
  Fichero: prueba.c
  Creado: 05-11-2025
  Ultima Modificacion: dimecres, 5 de novembre de 2025, 20:24:15
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

/* PRUEBA DE DESPLAZAMIENTO PIXEL A PIXEL */

#include <stdio.h>

#include "zxou.h"

#define inscrx (px>=0 && px<SCRWP)
#define inscry (py>=0 && py<SCRHP)

void pacip(byte chr,int x,int y) {
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
					if(val || plotted(px,py)) plot(px,py);
					else unplot(px,py);
				}
				px++;
			}
		}
		py++;
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

begin
	gdu_def();
	while(inkey('q')==0) {
		pacip(128,ghostx,ghosty);
		line(0,0,255,255);
		show;
		pause(0.01);
		cls;
		listen;
		move_gdu();
	}
end
				
		




