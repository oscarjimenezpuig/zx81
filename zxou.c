/*
============================================================
  Fichero: zxou.c
  Creado: 18-10-2025
  Ultima Modificacion: dimecres, 29 d’octubre de 2025, 20:32:41
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "zxou.h"

typedef struct {
	int x,y;
	int x2,y2;
	int avr;
	int av;
	int avi;
	short incxi,incyi;
	short incxr,incyr;
} Bresenhan;

static void swap(short* a,short* b) {
	short k=*a;
	*a=*b;
	*b=k;
}

static Bresenhan bresnew(byte x1,byte y1,byte x2,byte y2) {
	Bresenhan b={x1,y1,x2,y2,0,0,0,0,0,0,0};
	if(x1!=x2 || y1!=y2) {
		short dx,dy,incxi,incyi,incxr,incyr;
		dy=(y2-y1);
		dx=(x2-x1);
		incxi=incyi=-1;
		incxr=incyr=0;
		if(dy>=0) incyi=1;
		else dy=-dy;
		if(dx>=0) incxi=1;
		else dx=-dx;
		if(dx>=dy) incxr=incxi;
		else {
			incyr=incyi;
			swap(&dx,&dy);
		}
		b=(Bresenhan){x1,y1,x2,y2,2*dy,2*dy-dx,2*(dy-dx),incxi,incyi,incxr,incyr};
	}
	return b;
}

static byte bresget(Bresenhan* b) {
	if(b->x!=b->x2 || b->y!=b->y2) {
		if(b->av>=0) {
			b->x+=b->incxi;
			b->y+=b->incyi;
			b->av+=b->avi;
		} else {
			b->x+=b->incxr;
			b->y+=b->incyr;
			b->av+=b->avr;
		}
		return 1;
	} else {
		return 0;
	}
}

static void cpoc(double cx,double cy,double r,double a,byte* x,byte* y) {
	//calcula un punto de la circunferencia que se encuentra en angulo a dado
	const double PI=3.1415926532;
	double sn=sin(a*PI/180);
	double cs=cos(a*PI/180);
	double dx=cx+r*cs;
	double dy=cy+r*sn;
	*x=(byte)dx;
	*y=(byte)dy;
}

static void dpas(int cx,int cy,int x,int y) {
	//dibuja un punto y todos los simetricos a ese punto
	int dx=x-cx;
	int dy=y-cy;
	int cxs[]={x,cx-dx,cx-dx,x};
	int cys[]={y,y,cy-dy,cy-dy};
	int* px=cxs;
	int* py=cys;
	for(byte k=0;k<4;k++) {
		if(*px>=0 && *py>=0) z_plot(*px,*py);
		px++;
		py++;
	}
}

void z_circle(byte cx,byte cy,byte r) {
	for(int g=0;g<90;g++) {
		byte xi,yi,xf,yf;
		cpoc(cx,cy,r,g,&xi,&yi);
		cpoc(cx,cy,r,g+1,&xf,&yf);
		dpas(cx,cy,xf,yf);
		if(xi!=xf || yi!=yf) {
			Bresenhan b=bresnew(xi,yi,xf,yf);
			do {
				dpas(cx,cy,b.x,b.y);
			}while(!bresget(&b));
		}
	}
}

void z_cls() {
	byte* ptr=memory+OSCR;
	while(ptr!=memory+OSCR+DSCR) *ptr++=0;
	memory[CURSX]=memory[CURSY]=0;

}

void z_gdu(byte n,byte a,byte b,byte c,byte d,byte e,byte f,byte g,byte h) {
	byte data[]={a,b,c,d,e,f,g,h};
	byte *ptr,*ini,*pda;
	ptr=ini=memory+(OASC+n*8);
	pda=data;
	while(ptr!=ini+8) *ptr++=*pda++;
}

byte z_inkey(char k) {
	byte* p=memory+OKEY;
	while(p!=memory+OKEY+DKEY) {
		if(*p==k) return 1;
		p++;
	}
	return 0;
}

void z_line(byte x1,byte y1,byte x2,byte y2) {
	z_plot(x2,y2);
	if(x1!=x2 || y1!=y2) {
		Bresenhan b=bresnew(x1,y1,x2,y2);
		do {
			z_plot(b.x,b.y);
		} while(bresget(&b));
	}
}

void z_locate(byte x,byte y) {
	byte* ptr=memory+CURSX;
	*ptr++=8*x;
	*ptr=8*y;
}

void z_mode(byte m) {
	const short LCI=SCRWC/8;
	byte* pc=memory+CURSX;
	byte x=(*pc++)/8;
	byte y=*pc/8;
	byte* pinv=memory+OINV+(x/8)+y*LCI;
	byte iflag=128>>(x%8);
	byte flag=0;
	while(iflag!=0) {
		flag|=iflag;
		iflag=iflag>>1;
	}
	if(m==0) *pinv &= (~flag);
	else *pinv |= flag;
	pinv++;
	while(pinv!=memory+OINV+DINV) {
		if(m==0) *pinv=0;
		else *pinv=255;
		pinv++;
	}
}

void z_pause(double s) {
	clock_t lim=clock()+s*CLOCKS_PER_SEC;
	while(clock()<lim);
}

void z_plot(byte x,byte y) {
	byte q=x/8;
	byte r=x%8;
	byte* dir=memory+OSCR+y*(SCRWP/8)+q;
	*dir|=(128>>r);
}

void z_printc(byte chr) {
	byte* cx=memory+CURSX;
	byte* cy=cx+1;
	byte *pac,*ini;
	pac=ini=memory+OASC+(chr*8);
	byte* psc=memory+OSCR+*cx/8+*cy*SCRWC;
	while(pac!=ini+8) {
		*psc=*pac++;
		psc+=SCRWC;
	}
	*cx+=8;
	if(*cx==0) *cy+=8;
}

void z_prints(char* str) {
	char* ptr=str;
	while(*ptr!='\0') z_printc(*ptr++);
}

void z_printn(double num) {
	char sn[24];
	sprintf(sn,"%lf",num);
	char* p=sn;
	byte coma=0;
	while(*p!='\0') {
		if(*p=='.') coma=1;
		p++;
	}
	p=(p!=sn)?p-1:p;
	while(coma && (*p=='0' || *p=='.')) {
		if(*p=='.') coma=0;
		*p='\0';
		p--;
	}
	z_prints(sn);
}

void z_randomize(int v) {
	unsigned int seed=(v>0)?v:time(NULL);
	srand(seed);
}	

int z_rnd(int a,int b) {
	int max=(a>b)?a:b;
	int min=(a>b)?b:a;
	int dif=max-min+1;
	return min+(rand()%dif);
}

//prueba

void program() {
	locate(10,10);
	inverse;
	printc('a');
	normal;
	output();
wait:
	input();
	if(memory[OKEY]==0) goto wait;
}


