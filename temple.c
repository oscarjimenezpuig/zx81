/*
============================================================
  Fichero: temple.c
  Creado: 05-11-2025
  Ultima Modificacion: dimecres, 5 de novembre de 2025, 05:00:17
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "zxou.h"

#define SCRAREA (SCRWC*SCRHC)

#define TRAPS 30 //numero maximo de trampas
#define TMPM 15 //tiempo maximo en segundos para coger una moneda


#define GMANN 128
#define GMANF 131
#define GPARED 134
#define GTREE 137
#define GMONEDA 139
#define GIDOLN 140
#define GIDOLF 141
#define GFLEN 142
#define GFLEF 143
#define GVAMP 144
#define GMAND 146
#define GMANW 147
#define GTRAP 148
#define GTBAR 149

#define obsget(X,Y) obstaculo[(X)+(Y)*SCRWC] //mira si hay obstaculo en la posicion
#define obsset(X,Y) obstaculo[(X)+(Y)*SCRWC]=1 //coloca obstaculo en un posicion
#define obsunset(X,Y)  obstaculo[(X)+(Y)*SCRWC]=0 //quita obstaculo de una posicion
#define buginsu (obsget(bugabo.x,bugabo.y+1)==1) //dice si el bugabo esta en el suelo
#define velinv(V) ((V)>0)?(-(V)+1):((V)<0)?(-(V)-1):0
#define sgn(A) (((A)>0)?1:((A)<0)?-1:0)
#define colision(A,B) ((A.x)==(B.x) && (A.y)==(B.y))

typedef byte u1;

struct {
	byte gdu; //grafico
	byte gin; //animacioni
	int x,y; //posicion
	int vx,vy; //velocidad
	int a; //aceleracion
	u1 max_delay;
	u1 delay;
	int monedas;
	u1 vivo;
} bugabo;

struct {
	int x,y;
	u1 max_delay;
	u1 delay;
} moneda;

struct {
	int x,y;
} idol;

struct {
	u1 gdu;
	int x,y;
	int dx;
	u1 activa;
	u1 max_delay;
	u1 delay;
} flecha;

struct {
	u1 gdu;
	u1 gin;
	int x,y;
	int fx,fy;
	int dx,dy;
	u1 max_delay;
	u1 delay;
	u1 max_tras;
	u1 tras;
} vamp;

struct {
	int x,y;
} trap[TRAPS];

static u1 obstaculo[SCRAREA];
static u1 traps=0; 

void gdu_init() {
	gdu(GMANN,24,16,0,24,28,24,24,24);
	gdu(GMANN+1,24,16,0,28,28,24,24,36);
	gdu(GMANN+2,24,16,0,30,24,40,196,4);
	gdu(GPARED,255,145,133,161,137,161,133,255);
	gdu(GPARED+1,255,137,209,133,163,129,169,255);
	gdu(GPARED+2,255,165,129,211,133,145,197,255);
	gdu(GTREE,30,33,202,161,130,82,129,139);
	gdu(GTREE+1,100,20,28,8,8,8,24,60);
	gdu(GMONEDA,60,66,153,177,177,153,66,60);
	gdu(GIDOLN,124,198,215,199,255,248,248,255);
	flip(GIDOLF,GIDOLN);
	gdu(GFLEN,0,0,0,0,0,2,255,2);
	flip(GFLEF,GFLEN);
	gdu(GVAMP,36,153,90,60,24,36,66,165);
	gdu(GVAMP+1,36,24,24,126,153,36,66,165);
	gdu(GMAND,16,56,16,16,16,1,159,223);
	gdu(GMANW,90,90,66,60,24,36,36,36);
	gdu(GTRAP,129,161,165,165,165,165,165,255);
	gdu(GTBAR,0,0,0,255,255,0,0,0);
	for(byte k=0;k<3;k++) flip(GMANF+k,GMANN+k);
}

void obstaculo_init() {
	u1* ptr=obstaculo;
	while(ptr!=obstaculo+SCRAREA) *ptr++=0;
}

void obstaculo_print() {
#ifndef STARTED
#define STARTED
#include<stdio.h>
#endif
	u1*ptr=obstaculo;
	while(ptr!=obstaculo+SCRAREA) {
		int dis=ptr-obstaculo;
		if(dis!=0 && dis%SCRWC==0) printf("\n");
		printf("%i",*ptr);
		ptr++;
	}
	printf("\n");
}

void screen_init() {
	//definicion de las pantallas como cadenas de caracteres
	//las letras minusculas nos dicen que el estado es inverse y las mayusculas
	//que el estado es normal. a,b,c son los tres tipos de paredes.
	u1 initx=0;
	u1 inity=2;
	char* pantalla[]={	"            p pppppp            ",
						"        ppppp      ppppp        ",
						"    ppppp    A         ppppp    ",
						"ppppp        B             ppppp",
						"ppp          ppppp           ppp",
						"pp     A                      pp",
						"p      B                       p",
						"p     ppp    pppp   pppp     ppp",
						"p                          A   p",
						"p                          B   p",
						"pppppppp                 ppppppp",
						"pppp         A                 p",
						"ppp          B                 p",
						"p           pp        pp       p",
						"p                 A            p",
						"p                 B            p",
						"p       p        ppppp         p",
						"p      ppp        ppp          p",
						"pA    ppppp        p           p",
						"pB      p                   A  p",
						"ppp     p                   B  p",
						"p       p    pp          ppppppp",
						"p       p    pp             pppp",
						"p       p   pppp              pp",
						"ppp     p          p           p",
						"p       p          pp          p",
						"p    A  p          ppp         p",
						"p    B  p A        pppp   A  A p",
						"ppppppppp B        ppppp  B  B p", 
						"pppppppppppppppppppppppppppppppp"
	};
	u1 filas=30;
	for(u1 f=0;f<filas;f++) {
		char *ptr,*init;
		ptr=init=pantalla[f];
		while(*ptr!='\0') {
			int x=ptr-init+initx;
			int y=f+inity;
			locate(x,y);
			u1 gdup=0;
			if(*ptr=='p') gdup=GPARED+rnd(0,2); 
			else if(*ptr==' ') gdup=' ';
			else if(*ptr=='A') gdup=GTREE;
			else if(*ptr=='B') gdup=GTREE+1;
			printc(gdup);
			if(gdup!=' ') obsset(x,y);
			ptr++;
		}
	}
	obstaculo_print();//dbg
}

void moneda_init() {
	const u1 MAX_DELAY=7;
	int x,y;
	do {
		x=rnd(1,30);
		y=rnd(1,30);
	}while(obsget(x,y));
	moneda.x=x;
	moneda.y=y;
	moneda.max_delay=MAX_DELAY;
	moneda.delay=0;
	locate(moneda.x,moneda.y);
	printc(GMONEDA);
	times;
}

void moneda_quita(u1 gpu) {
	locate(moneda.x,moneda.y);
	printc(gpu);
	moneda.x=moneda.y=-1;
}

void moneda_move() {
	if(!obsget(moneda.x,moneda.y+1)) {
		if(moneda.delay==moneda.max_delay) {
			locate(moneda.x,moneda.y);
			printc(' ');
			moneda.y++;
			locate(moneda.x,moneda.y);
			printc(GMONEDA);
			moneda.delay=0;
		} else {
			moneda.delay++;
		}
	}
}

void moneda_coge() {
	if(colision(bugabo,moneda)) {
		bugabo.monedas++;
		moneda_quita(GMANW);
		show;
		pause(0.5);
		moneda_init();
	}
}

void idol_init() {
	const u1 MAX_DELAY=3;
	do {
		idol.x=(rnd(0,1))?30:1;
		idol.y=rnd(7,25);
	}while(obsget(idol.x,idol.y));
	u1 gdu=(idol.x==1)?GIDOLN:GIDOLF;
	locate(idol.x,idol.y);
	printc(gdu);
	flecha.activa=0;
	flecha.dx=(idol.x==1)?1:-1;
	flecha.gdu=(idol.x==1)?GFLEN:GFLEF;
	flecha.max_delay=MAX_DELAY;
	flecha.delay=0;
	flecha.x=(idol.x==1)?2:29;
	flecha.y=idol.y;
}

void idol_quita() {
	locate(idol.x,idol.y);
	printc(' ');
}

void flecha_move() {
	if(!flecha.activa) {
		if(bugabo.y==idol.y) {
			flecha.activa=1;
		}
	}
	if(flecha.activa) {
		if(flecha.delay==flecha.max_delay) {
			locate(flecha.x,flecha.y);
			printc(' ');
			int fx=flecha.x+flecha.dx;
			if(obsget(fx,flecha.y)) {
				idol_quita();
				idol_init();
			} else {
				locate(fx,flecha.y);
				printc(flecha.gdu);
				flecha.x=fx;
			}
			flecha.delay=0;
		} else flecha.delay++;
	}
}

void flecha_colisiona() {
	if(colision(flecha,bugabo)) {
		bugabo.vivo=0;
	}
	if(colision(flecha,moneda)) {
		moneda_quita(flecha.gdu);
		moneda_init();
	}
}

void vamp_init() {
	const u1 MAX_DELAY=6;
	const u1 MAX_TRAS=255;
	vamp.gdu=GVAMP;
	vamp.gin=0;
	int x,y;
	do {
		x=rnd(1,30);
		y=rnd(1,30);
	}while(obsget(x,y));
	vamp.x=vamp.fx=x;
	vamp.y=vamp.fy=y;
	vamp.dx=vamp.dy=0;
	vamp.max_tras=rnd(MAX_TRAS/2,MAX_TRAS);
	vamp.tras=0;
	vamp.delay=0;
	vamp.max_delay=MAX_DELAY;
}

void vamp_quita() {
	locate(vamp.x,vamp.y);
	printc(' ');
}

void vamp_move() {
	if(vamp.fx==vamp.x && vamp.fy==vamp.y) {
		vamp.fx=rnd(1,30);
		vamp.fy=rnd(1,30);
		vamp.dx=vamp.dy=0;
	}
	if(vamp.fx!=vamp.x && vamp.fy==vamp.y) {
		vamp.dx=(vamp.fx>vamp.x)?1:-1;
	} else if(vamp.fx==vamp.x && vamp.fy!=vamp.y) {
		vamp.dy=(vamp.fy>vamp.y)?1:-1;
	} else if(rnd(0,1)) {
		vamp.dx=(vamp.fx>vamp.x)?1:-1;
	} else {
		vamp.dy=(vamp.fy>vamp.y)?1:-1;
	}
	if(vamp.delay==vamp.max_delay) {
		if(vamp.tras==vamp.max_tras) {
			vamp_quita();
			vamp_init();
		} else {
			vamp.tras++;
			int fx=vamp.x+vamp.dx;
			int fy=vamp.y+vamp.dy;
			if(obsget(fx,fy)) {
				vamp.fx=vamp.x;
				vamp.fy=vamp.y;
			} else {
				vamp_quita();
				vamp.x=fx;
				vamp.y=fy;
				locate(fx,fy);
				printc(vamp.gdu+vamp.gin/3);
				vamp.gin=(vamp.gin+1)%6;
			}
		}
		vamp.delay=0;
	} else vamp.delay++;
}


void vamp_colisiona() {
	if(colision(vamp,flecha)) {
		vamp_quita();
		vamp_init();
	}
	if(colision(vamp,idol)) {
		idol_quita();
		idol_init();
	}
	if(colision(vamp,moneda)) {
		moneda_quita(vamp.gdu);
		moneda_init();
	}
	if(colision(vamp,bugabo)) {
		bugabo.vivo=0;
	}
}

void trap_new() {
	if(timeg>=TMPM && traps<TRAPS) {
		times;
		int x,y;
		u1 place=0;
		while(!place) {
			x=rnd(1,31);
			y=rnd(1,31);
			for(int j=y;j<32 && !place;j++) {
				for(int i=x;i<32 && !place;i++) {
					if(obsget(i,j) && !obsget(i,j-1)) {
						x=i;
						y=j;
						place=1;
						locate(x,y);
						printc(GTRAP);
					}
				}
			}
		}
		obsunset(x,y);
		trap[traps].x=x;
		trap[traps].y=y;
		traps++;
	}
}

void trap_colisiona() {
	for(u1 k=0;k<traps;k++) {
		if(colision(trap[k],bugabo)) {
			bugabo.vivo=0;
			break;
		}
		if(colision(trap[k],moneda)) {
			moneda_quita(GTRAP);
			moneda_init();
		}
	}
}

void bugabo_init() {
	const u1 MAX_DELAY=5;
	const int BXI=16;
	const int BYI=30;
	bugabo.gdu=GMANN;
	bugabo.gin=0;
	bugabo.x=BXI;
	bugabo.y=BYI;
	bugabo.vx=0;
	bugabo.a=1;
	bugabo.max_delay=MAX_DELAY;
	bugabo.delay=0;
	locate(bugabo.x,bugabo.y);
	printc(bugabo.gdu+bugabo.gin);
	bugabo.monedas=0;
	bugabo.vivo=1;
}

u1 bugabo_can_move() {
	if(bugabo.vivo) {
		if(bugabo.delay==bugabo.max_delay) {
			bugabo.delay=0;
			return 1;
		} else bugabo.delay++;
	} 
	return 0;
}

u1 bugabo_find_obstacle(int* fx,int* fy) {
	const u1 FH=1;
	const u1 FV=2;
	u1 obstacle=0;
	if(obsget(*fx,*fy)) {
		obstacle|=(FH|FV);
		if(*fx!=bugabo.x && !obsget(*fx,bugabo.y)) obstacle&=(~FH);
		else if(*fy!=bugabo.y && !obsget(bugabo.x,*fy)) obstacle&=(~FV);
	}
	if(obstacle & FH) *fx=bugabo.x;
	if(obstacle & FV) {
		bugabo.vy=0;
		*fy=bugabo.y;
	}
	return obstacle;
}

void bugabo_move() {
	if(bugabo_can_move()) {
		bugabo.vy+=bugabo.a;
		int fx=bugabo.x+sgn(bugabo.vx);
		bugabo.vx=0;
		int fy=bugabo.y+sgn(bugabo.vy);
		bugabo_find_obstacle(&fx,&fy);
		if(fx!=bugabo.x || fy!=bugabo.y) {
			locate(bugabo.x,bugabo.y);
			printc(' ');
			locate(fx,fy);
			printc(bugabo.gdu+(bugabo.gin/3));
			bugabo.x=fx;
			bugabo.y=fy;
		}
	}
}

int bugabo_altura_maxima() {
	const int MAXIMP=-6;
	int dif=-bugabo.y;
	int altmax=(dif>MAXIMP)?dif:MAXIMP;
	return altmax;
}

void bugabo_inkey() {
	if(inkey('j') && bugabo.x>0) {
		if(bugabo.gdu==GMANN) bugabo.gdu=GMANF;
		else bugabo.gin=((bugabo.gin+1)%9);
		bugabo.vx=-1;
	} else if(inkey('l') && bugabo.x<31) {
		if(bugabo.gdu==GMANF) bugabo.gdu=GMANN;
		else bugabo.gin=((bugabo.gin+1)%9);
		bugabo.vx=1;
	}
	if(buginsu) {
		while(inkey('i') && bugabo.vy>bugabo_altura_maxima()) {
			bugabo.vy-=1;
		}
	}
};

u1 game_over() {
	if(bugabo.vivo==0) {
		locate(bugabo.x,bugabo.y);
		printc(GMAND);
		show;
		pause(1);
		locate(11,16);
		inverse;
		prints("GAME OVER");
		normal;
		show;
		pause(3);
		return 1;
	}
	return 0;
}

void marcador() {
	locate(0,0);
	prints("COINS ");
	printn(bugabo.monedas);
	int time=(int)timeg;
	locate(SCRWC-TMPM,0);
	for(int k=0;k<TMPM;k++) {
		if(k<time) printc(' ');
		else printc(GTBAR);
	}
}

begin_program
	randomize(-1);
	gdu_init();
	screen_init();
	bugabo_init();
	moneda_init();
	idol_init();
	vamp_init();
	marcador();
	show;
	while(inkey('q')==0 && !game_over()) {	
		trap_new();
		listen;
		bugabo_inkey();
		bugabo_move();
		moneda_move();
		flecha_move();
		vamp_move();
		moneda_coge();
		flecha_colisiona();
		vamp_colisiona();
		trap_colisiona();
		marcador();
		show;
		pause(0.01);
	}
end_program


