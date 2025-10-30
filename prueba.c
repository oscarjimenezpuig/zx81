/*
============================================================
  Fichero: prueba.c
  Creado: 30-10-2025
  Ultima Modificacion: jue 30 oct 2025 12:26:04
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

//Prueba de ZX81

#include "zxou.h"

#define PELOTA 128
#define PAREDA 129
#define PAREDB 130
#define PAREDC 131

typedef byte u1;

struct {
	u1 x,y;
} pelota ={0,0};

void gdu_init() {
	gdu(PELOTA,60,126,255,255,255,255,126,60);
	gdu(PAREDA,138,36,129,36,16,18,64,8);
	gdu(PAREDB,128,18,32,1,68,0,34,8);
	gdu(PAREDC,0,36,128,9,64,16,2,32);
}

void screen_init() {
	//definicion de las pantallas como cadenas de caracteres
	//las letras minusculas nos dicen que el estado es inverse y las mayusculas
	//que el estado es normal. a,b,c son los tres tipos de paredes.
	u1 initx=0;
	u1 inity=2;
	char* pantalla[]={	"p                              p",
						"pppppppppppp   ppppppppppppppppp",
						"p                              p",
						"p                              p",
						"pppppp      ppppp      pppp   pp",
						"p                              p",
						"p                              p",
						"p     ppp    pppp   pppp     ppp",
						"p                              p",
						"p                              p",
						"pppppppp                 ppppppp",
						"pppp                           p",
						"ppp                            p",
						"p           pp        pp       p",
						"p                              p",
						"pppppppppppppppppppppppppppppppp"
	};
	u1 filas=16;
	for(u1 f=0;f<filas;f++) {
		char *ptr,*init;
		ptr=init=pantalla[f];
		while(*ptr!='\0') {
			locate(ptr-init+initx,f+inity);
			if(*ptr>='a' && *ptr<='z') inverse;
			u1 gdup=0;
			if(*ptr=='p') gdup=rnd(PAREDA,PAREDC); 
			else if(*ptr==' ') gdup=' ';
			printc(gdup);
			if(*ptr>='a' && *ptr<='z') normal;
			ptr++;
		}
	}
}

begin_program
	gdu_init();
	screen_init();
	show;
	while(inkey('q')==0) listen;
end_program


