/*
============================================================
  Fichero: prueba.c
  Creado: 05-11-2025
  Ultima Modificacion: dimecres, 5 de novembre de 2025, 05:28:17
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

/* PRUEBA DE DESPLAZAMIENTO PIXEL A PIXEL */

#include "zxou.h"

#define AND 1
#define OR 2
#define XOR 3
#define EXC 4 //no tiene en cuenta el valor previo de la posicion

void pacip(byte chr,byte operation,int x,int y) {
	//coloca un caracter en cualquier posicion de la pantalla
	byte *ptc,*init;
	init=ptc=pointer(OASC+8*chr);
	int px;
	int py=y;
	while(ptc!=init+8) {
		px=x;
		for(byte f=128;f>=1;f=f>>1) {
			byte val=(*ptc & f);
			if(operation!=EXC) {
				byte vip=plotted(x,y);
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
				if(val) plot(x,y);
				else unplot(x,y);
			}
			x++;
		}
		y++;
	}
}
				
		




