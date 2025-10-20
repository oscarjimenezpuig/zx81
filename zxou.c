/*
============================================================
  Fichero: zxou.c
  Creado: 18-10-2025
  Ultima Modificacion: diumenge, 19 d’octubre de 2025, 20:52:19
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "zxou.h"

void z_cls() {
	byte* ptr=memory;
	while(ptr!=memory+DMEM) *ptr++=0;
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

static void swap(short* a,short* b) {
	short k=*a;
	*a=*b;
	*b=k;
}

void z_line(byte x1,byte y1,byte x2,byte y2) {
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
	int x=x1;
	int y=y1;
	int avr=2*dy;
	int av=(avr-dx);
	int avi=(av-dx);
	do {
		z_plot(x,y);
		if(av>=0) {
			x+=incxi;
			y+=incyi;
			av+=avi;
		} else {
			x+=incxr;
			y+=incyr;
			av+=avr;
		}
	} while(x!=x2 || y!=y2);
	z_plot(x2,y2);
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
	printf("%i,%i\n",x,y);//dbg
	byte* pinv=memory+OINV+(x/8)+y*LCI;
	printf("%i %li\n",DMEM,pinv-memory+OINV);//DBG
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

double z_power(double b,double e) {
	return pow(b,e);
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

double z_sin(double a) {
	return sin(a);
}

//prueba

void program() {
	z_locate(10,10);
	z_mode(INVERSE);
	z_printc('B');
	output();
wait:
	input();
	if(memory[OKEY]==0) goto wait;
}


