/*
============================================================
  Fichero: sprite.c
  Creado: 06-11-2025
  Ultima Modificacion: divendres, 7 de novembre de 2025, 05:26:00
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>

#include "zxou.h"

address sprite_new(address dir,byte h,char* data[]) {
	//se crea un sprite en la memoria, en la direccion dir, la altura del sprite es h, data, si es '0' no se dibuja. Se exige que cada componente de data tenga la misma longitud
	//se retorna 0 si no se ha podido construir el sprite o la siguiente direccion si se ha podido
	byte w=0;
	byte* ptr=memory+dir;
	for(int j=0;j<h;j++) {
		char* ptd=data[j];
		byte flag=128;
		*ptr=0;
		byte nw=0;
		while(*ptd!='\0' && ptr!=memory+DMEM) {
			nw++;
			if(*ptd!='0') *ptr|=flag;
			flag=flag>>1;
			if(flag==0) {
				flag=128;
				ptr++;
				*ptr=0;
			}
			ptd++;
		}
		ptr++;
		if(w!=0 && w!=nw) return 0;
		else if(w==0) w=nw;
	}
	address newdir=dir+((w/8 +((w%8)?1:0))*h);
	return newdir;
}

#define ASC(A) (OASC+ (8*(A))) //da la direccion de memoria de ASCII

#define inscrx (px>=0 && px<SCRWP)
#define inscry (py>=0 && py<SCRHP)

void sprite_screen(address dir,byte w,byte h,int x,int y) {
	//se traslada un sprite que empieza en la direccion de memoria dir a la posicion de pantalla x,y
	int q=w/8;
	int r=w%8;
	w=(r>0)?8*(q+1):8*q;
	byte* ptr=memory+dir;
	for(int j=0;j<h;j++) {
		byte flag=128;
		for(int i=0;i<w;i++) {
			int px=x+i;
			int py=y+j;
			if(inscrx && inscry) {
				if((*ptr & flag) || plotted(px,py)) plot(px,py);
				else unplot(px,py);
			}
			flag=flag>>1;
			if(flag==0) {
				flag=128;
				ptr++;
			}
		}
		puts("");//dbg
	}
}

begin 
	char* data[]={	"00000001110000000",
					"00000011100000000",
					"00011111111000000",
					"00100100000100011",
					"01000010000010101",
					"10011010000011001",
					"10011010000011001",
					"01000010000010101",
					"00100100000100011",
					"00011111111000000"
	};
	sprite_new(ORAM,10,data);
	for(int x=300;x>=-20;x--) {
		sprite_screen(ORAM,17,10,x,100);
		show;
		pause(0.01);
		cls;
	}
	while(inkey('q')==0) listen;
end


