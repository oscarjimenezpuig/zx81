/*
============================================================
  Fichero: memory.c
  Creado: 18-10-2025
  Ultima Modificacion: diumenge, 19 d’octubre de 2025, 20:36:22
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "memory.h"

//XVIDEO

long unsigned int X_BLACK=0;
long unsigned int X_WHITE=0;

unsigned int X_WINW=0;
unsigned int X_WINH=0;
static unsigned char X_PIX=4; //dimension del square
static Display* display=NULL;
static Window window;
static Pixmap virtual;
static GC graphic;
static int min_key_code=0;
static int max_key_code=0;

static int x_init(unsigned int w,unsigned int h,unsigned char pixdim) {
	X_WINW=w;
	X_WINH=h;
	X_PIX=pixdim;
	display=XOpenDisplay(0);
	if(display) {
		int screennum=XDefaultScreen(display);
		X_BLACK=BlackPixel(display,screennum);
		X_WHITE=WhitePixel(display,screennum);
		window=XCreateSimpleWindow(display,RootWindow(display,screennum),0,0,X_WINW,X_WINH,0,0,0);
		XWindowAttributes xwa;
		XGetWindowAttributes(display,window,&xwa);
		virtual=XCreatePixmap(display,window,X_WINW,X_WINH,xwa.depth);
		static XGCValues gv;
		gv.line_width=1;
		gv.foreground=X_BLACK;
		gv.background=X_WHITE;
		graphic=XCreateGC(display,window,GCForeground|GCBackground|GCLineWidth,&gv);
		XSelectInput(display,window,StructureNotifyMask);
		XMapWindow(display,window);
		XEvent event;
		int mapped=0;
		while(XEventsQueued(display,QueuedAlready) || !mapped) {
			XNextEvent(display,&event);
			mapped=(!mapped)?(event.type==MapNotify):1;
		}
		XDisplayKeycodes(display,&min_key_code,&max_key_code);
		return 1;
	}
	return 0;
}

static void x_end() {
	XUnmapWindow(display,window);
	XDestroyWindow(display,window);
	XFreePixmap(display,virtual);
	XFreeGC(display,graphic);
	XEvent e;
	while(XEventsQueued(display,QueuedAlready)) {
			XNextEvent(display,&e);
	}
	XCloseDisplay(display);
}


void x_pixel(short x,short y,unsigned char mode) {
	XSetForeground(display,graphic,(mode)?X_WHITE:X_BLACK);
	XFillRectangle(display,virtual,graphic,x,y,X_PIX,X_PIX);
}

void x_character(short x,short y,unsigned char mode) {
	const short FACT=X_PIX*8;
	XSetForeground(display,graphic,(mode)?X_BLACK:X_WHITE);
	XFillRectangle(display,virtual,graphic,x,y,FACT,FACT);
}

static void x_flush() {
	XCopyArea(display,virtual,window,graphic,0,0,X_WINW,X_WINH,0,0);
	while(XPending(display)==0);
	XFlush(display);
}

static int x_inkey(KeySym* keysym) {
	const int KCC=1;
	const unsigned char POS=0;
	int return_type=0;
	XEvent event;
	long event_mask=KeyPressMask|KeyReleaseMask;
	XSelectInput(display,window,event_mask);
	if(XPending(display)>0 && XCheckMaskEvent(display,event_mask,&event)) {
		int counter=0;
		XKeyEvent kevent=event.xkey;
		if(kevent.keycode>=min_key_code && kevent.keycode+KCC-1<=max_key_code) {
			KeySym* ks=XGetKeyboardMapping(display,kevent.keycode,KCC,&counter);
			if(POS<KCC*counter) {
				return_type=(kevent.type==KeyPress)?1:(kevent.type==KeyRelease)?-1:0;
				*keysym=*(ks+POS);
			}
			XFree(ks);
		}
	}
	return return_type;
}

//MEMORIA

byte memory[DMEM];

static void mem_clr() {
	//inicio de la memoria
	byte* ptr=memory;
	while(ptr!=memory+DMEM) *ptr++=0;
}

static void asc_new(char c,byte* data) {
	address dir=OASC+c*8;
	byte* ptm=memory+dir;
	byte* ptd=data;
	while(ptm!=memory+dir+8) {
		*ptm++=*ptd++;
	}
}	

#define DATA(A,B,C,D,E,F,G,H) (byte[]){A,B,C,D,E,F,G,H}

static void asc_init() {
	asc_new('0',DATA(0,60,70,74,82,98,60,0));
	asc_new('1',DATA(0,48,80,16,16,16,124,0));
	asc_new('2',DATA(0,60,66,2,60,64,126,0));
	asc_new('3',DATA(0,60,66,12,2,66,60,0));
	asc_new('4',DATA(0,4,12,20,36,62,4,0));
	asc_new('5',DATA(0,126,64,124,2,66,60,0));
	asc_new('6',DATA(0,60,64,124,66,66,60,0));
	asc_new('7',DATA(0,126,2,4,8,16,16,0));
	asc_new('8',DATA(0,60,66,60,66,66,60,0));
	asc_new('9',DATA(0,60,66,66,62,2,60,0));
	asc_new('A',DATA(0,60,66,66,126,66,66,0));
	asc_new('B',DATA(0,124,66,124,66,66,124,0));
	asc_new('C',DATA(0,60,66,64,64,66,60,0));	
	asc_new('D',DATA(0,120,68,66,66,68,120,0));
	asc_new('E',DATA(0,126,64,120,64,64,126,0));
	asc_new('F',DATA(0,126,64,120,64,64,64,0));
	asc_new('G',DATA(0,60,66,64,78,66,60,0));
	asc_new('H',DATA(0,66,66,126,66,66,66,0));
	asc_new('I',DATA(0,124,16,16,16,16,124,0));
	asc_new('J',DATA(0,2,2,2,66,66,60,0));
	asc_new('K',DATA(0,68,72,112,72,68,66,0));
	asc_new('L',DATA(0,64,64,64,64,64,126,0));
	asc_new('M',DATA(0,66,102,90,66,66,66,0));
	asc_new('N',DATA(0,66,98,82,74,70,66,0));
	asc_new('O',DATA(0,60,66,66,66,66,60,0));
	asc_new('P',DATA(0,124,66,66,124,64,64,0));
	asc_new('Q',DATA(0,60,66,66,82,74,60,0));
	asc_new('R',DATA(0,124,66,66,124,68,66,0));
	asc_new('S',DATA(0,60,64,60,2,66,60,0));
	asc_new('T',DATA(0,124,16,16,16,16,16,0));
	asc_new('U',DATA(0,66,66,66,66,66,60,0));
	asc_new('V',DATA(0,66,66,66,66,36,24,0));
	asc_new('W',DATA(0,66,66,66,66,90,36,0));
	asc_new('X',DATA(0,66,36,24,24,36,66,0));
	asc_new('Y',DATA(0,68,40,16,16,16,16,0));
	asc_new('Z',DATA(0,126,4,8,16,32,126,0));
	asc_new('a',DATA(0,0,56,4,60,68,60,0));
	asc_new('b',DATA(0,64,64,120,68,68,120,0));
	asc_new('c',DATA(0,0,28,32,32,32,28,0));
	asc_new('d',DATA(0,4,4,60,68,68,60,0));
	asc_new('e',DATA(0,0,56,68,120,64,60,0));
	asc_new('f',DATA(0,24,32,48,32,32,32,0));
	asc_new('g',DATA(0,0,60,68,68,60,4,56));
	asc_new('h',DATA(0,64,64,120,68,68,68,0));
	asc_new('i',DATA(0,16,0,48,16,16,56,0));
	asc_new('j',DATA(0,4,0,4,4,4,36,24));
	asc_new('k',DATA(0,32,40,48,48,40,36,0));
	asc_new('l',DATA(0,32,32,32,32,32,24,0));
	asc_new('m',DATA(0,0,104,84,84,84,84,0));
	asc_new('n',DATA(0,0,120,68,68,68,68,0));
	asc_new('o',DATA(0,0,56,68,68,68,56,0));
	asc_new('p',DATA(0,0,120,68,68,120,64,64));
	asc_new('q',DATA(0,0,60,68,68,60,4,6));
	asc_new('r',DATA(0,0,60,64,64,64,64,0));
	asc_new('s',DATA(0,0,56,64,56,4,120,0));
	asc_new('t',DATA(0,16,56,16,16,16,12,0));
	asc_new('u',DATA(0,0,68,68,68,68,56,0));
	asc_new('v',DATA(0,0,68,68,40,40,16,0));
	asc_new('w',DATA(0,0,68,84,84,84,40,0));
	asc_new('x',DATA(0,0,68,40,16,40,68,0));
	asc_new('y',DATA(0,0,68,68,68,60,4,56));
	asc_new('z',DATA(0,0,124,8,16,32,124,0));
	asc_new('!',DATA(0,16,16,16,16,0,16,0));
	asc_new('"',DATA(0,36,36,0,0,0,0,0));
	asc_new('#',DATA(0,36,126,36,36,126,36,0));
	asc_new('$',DATA(0,16,124,80,124,20,124,16));
	asc_new('%',DATA(0,98,100,8,16,38,70,0));
	asc_new('&',DATA(0,16,40,16,42,68,58,0));
	asc_new('/',DATA(0,2,4,8,16,32,64,0));
	asc_new('(',DATA(0,8,16,16,16,16,8,0));
	asc_new(')',DATA(0,16,8,8,8,8,16,0));
	asc_new('[',DATA(0,24,16,16,16,16,24,0));
	asc_new(']',DATA(0,24,8,8,8,8,24,0));
	asc_new('\'',DATA(0,8,16,0,0,0,0,0));
	asc_new('*',DATA(0,0,40,16,124,16,40,0));
	asc_new('+',DATA(0,0,16,16,124,16,16,0));
	asc_new(',',DATA(0,0,0,0,0,16,16,32));
	asc_new('-',DATA(0,0,0,0,124,0,0,0));
	asc_new('.',DATA(0,0,0,0,0,24,24,0));
	asc_new(':',DATA(0,0,0,16,0,0,16,0));
	asc_new(';',DATA(0,0,8,0,0,8,8,16));
	asc_new('<',DATA(0,0,8,16,32,16,8,0));
	asc_new('>',DATA(0,0,32,16,8,16,32,0));
	asc_new('?',DATA(0,60,66,4,8,0,8,0));
	asc_new('@',DATA(0,60,74,86,94,64,60,0));
	asc_new('=',DATA(0,0,0,60,0,60,0,0));
	asc_new('\\',DATA(0,64,32,16,8,4,2,0));
	asc_new('^',DATA(0,16,40,68,0,0,0,0));
	asc_new('_',DATA(0,0,0,0,0,0,0,255));
	asc_new('{',DATA(0,28,16,96,16,16,28,0));
	asc_new('}',DATA(0,112,16,12,16,16,112,0));
	asc_new('|',DATA(0,16,16,16,16,16,16,0));
	asc_new('~',DATA(0,20,40,0,0,0,0,0));
}

#undef DATA

static void char_output(byte x,byte y) {
	static byte* pinv=memory+OINV;
	static byte flag=128;
	if(pinv==memory+OINV+DINV) {
		pinv=memory+OINV;
		flag=128;
	} else if(flag==0) flag=128;
	byte mode=(*pinv & flag);
	flag=flag>>1;
	pinv++;
	byte* ppix=memory+OSCR+x+y*8*SCRWC;
	byte px=x*8;
	byte py=y*8;
	x_character(px*DPIX,py*DPIX,mode);
	for(byte n=0;n<8;n++) {
		byte flagp=128;
		while(flagp) {
			if(flagp & *ppix) x_pixel(px*DPIX,py*DPIX,mode);
			px++;
			flagp=flagp>>1;
		}
		py++;
		px=x*8;
		ppix+=SCRWC;
	}
}

void output() {
	for(byte x=0;x<SCRWC;x++) {
		for(byte y=0;y<SCRHC;y++) {
			char_output(x,y);
		}
	}
	x_flush();
}

static void ks_ins(byte val) {
	byte* ptr=memory+OKEY;
	while(ptr<memory+OKEY+DKEY && *ptr!=0) ptr++;
	if(ptr!=memory+OKEY+DKEY) *ptr=val;
}

static void ks_era(byte val) {
	byte* ptr=memory+OKEY;
	while(ptr!=memory+OKEY+DKEY) {
		if(*ptr==val) *ptr=0;
		ptr++;
	}
}

void input() {
	KeySym k;
	int stat=0;
	if((stat=x_inkey(&k))) {
		if(stat==1) ks_ins((byte)k);
		else ks_era(k);
	}
}

int main() {
	mem_clr();
	asc_init();
	x_init(SCRWP*DPIX,SCRHP*DPIX,DPIX);
	output();
	program();
	x_end();
}
