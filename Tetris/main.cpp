#include "hge.h"
#include "hgefont.h"
#include "hgegui.h"
#include "hgesprite.h"
#include "menuitem.h"
#include "Figures.h"
#include "Borders.h"
#include <math.h>
#include <string.h>
#include <fstream>
using namespace std;

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// Pointer to the HGE interface.
// Helper classes require this to work.
HGE *hge=0;

// Some resource handles
HTEXTURE			tex, texc;
hgeQuad				quad;

Figures figure; //All figures
Borders border; //borders of field

struct sprObject
{
	float x,y;
	float dx, dy;
	float scale,rot;
	float dscale,drot;
	DWORD color;	
};

enum menuitem {
		CONTINUE=1, 
		NEWGAME, 
		SETTINGS, 
		EXIT,
		BLUE, 
		GREEN, 
		RED,
		RAND,
		BACK
	};

// Pointers to the HGE objects we will use
sprObject*	pObjects;
hgeGUI				*gui;
hgeFont				*fnt;
hgeSprite			*sprc;
hgeSprite			*spr;

int	nObjects;
const DWORD noncolor =-842150451;

static DWORD Colors[4]=
	{
		0x00000000, 0xFF80A0FF, 0xFFA0FF80, 0xFFFF80A0 // blue, green, red
	};

float delay=0;
float level=3;
int score=0;
int count=0;
bool mMenu = true;
bool gameover = false;
bool cBlue=false, cGreen=false, cRed=false, cRandom=true; 
int levelgame[9];
int figures[29];
int MAX_OBJECTS; //size of field

int clearCells (int _LS, int _RS) {
	//if line is colored
	if (pObjects[_LS].color!=noncolor && pObjects[_RS].color!=noncolor && 
		pObjects[_LS+1].color!=noncolor && pObjects[_RS-1].color!=noncolor &&
		pObjects[_LS+2].color!=noncolor && pObjects[_RS-2].color!=noncolor &&
		pObjects[_LS+3].color!=noncolor && pObjects[_RS-3].color!=noncolor &&
		pObjects[_LS+4].color!=noncolor && pObjects[_RS-4].color!=noncolor) {
		//clear cells
		pObjects[_LS].color=noncolor; pObjects[_RS].color=noncolor;
		pObjects[_LS+1].color=noncolor; pObjects[_RS-1].color=noncolor;
		pObjects[_LS+2].color=noncolor; pObjects[_RS-2].color=noncolor;
		pObjects[_LS+3].color=noncolor; pObjects[_RS-3].color=noncolor;
		pObjects[_LS+4].color=noncolor; pObjects[_RS-4].color=noncolor;
		count++;
		score+=100*count*2;
		for (int p=0; p<9;p++) {
			if (score==levelgame[p]) {
				level++; 
				break;
			}
		}
	}
	
	if ((pObjects[_LS].color==noncolor && pObjects[_RS].color==noncolor && 
		pObjects[_LS+1].color==noncolor && pObjects[_RS-1].color==noncolor &&
		pObjects[_LS+2].color==noncolor && pObjects[_RS-2].color==noncolor &&
		pObjects[_LS+3].color==noncolor && pObjects[_RS-3].color==noncolor &&
		pObjects[_LS+4].color==noncolor && pObjects[_RS-4].color==noncolor) &&
		
		(pObjects[_LS-10].color!=noncolor || pObjects[_RS-10].color!=noncolor || 
		pObjects[_LS+1-10].color!=noncolor || pObjects[_RS-1-10].color!=noncolor ||
		pObjects[_LS+2-10].color!=noncolor || pObjects[_RS-2-10].color!=noncolor ||
		pObjects[_LS+3-10].color!=noncolor || pObjects[_RS-3-10].color!=noncolor ||
		pObjects[_LS+4-10].color!=noncolor || pObjects[_RS-4-10].color!=noncolor))
	{
		pObjects[_LS].color=pObjects[_LS-10].color; pObjects[_RS].color=pObjects[_RS-10].color;
		pObjects[_LS-10].color=noncolor; pObjects[_RS-10].color=noncolor;

		pObjects[_LS+1].color=pObjects[_LS+1-10].color; pObjects[_RS-1].color=pObjects[_RS-1-10].color;
		pObjects[_LS+1-10].color=noncolor; pObjects[_RS-1-10].color=noncolor;

		pObjects[_LS+2].color=pObjects[_LS+2-10].color; pObjects[_RS-2].color=pObjects[_RS-2-10].color;
		pObjects[_LS+2-10].color=noncolor; pObjects[_RS-2-10].color=noncolor;

		pObjects[_LS+3].color=pObjects[_LS+3-10].color; pObjects[_RS-3].color=pObjects[_RS-3-10].color;
		pObjects[_LS+3-10].color=noncolor; pObjects[_RS-3-10].color=noncolor;

		pObjects[_LS+4].color=pObjects[_LS+4-10].color; pObjects[_RS-4].color=pObjects[_RS-4-10].color;
		pObjects[_LS+4-10].color=noncolor; pObjects[_RS-4-10].color=noncolor;
	}
	if (_LS!=10 && _RS!=19) 
	{
		clearCells (_LS-10, _RS-10);
	} else {
		count=0; 
		return 0;
	}
}

void clearLine () {
	//when one of lines have a color
	if ((pObjects[border.DL].color!=noncolor && pObjects[border.DR].color!=noncolor && 
			pObjects[border.DL+1].color!=noncolor && pObjects[border.DR-1].color!=noncolor &&
			pObjects[border.DL+2].color!=noncolor && pObjects[border.DR-2].color!=noncolor &&
			pObjects[border.DL+3].color!=noncolor && pObjects[border.DR-3].color!=noncolor &&
			pObjects[border.DL+4].color!=noncolor && pObjects[border.DR-4].color!=noncolor) ||
			
			(pObjects[border.DL-10].color!=noncolor && pObjects[border.DR-10].color!=noncolor && 
			pObjects[border.DL+1-10].color!=noncolor && pObjects[border.DR-1-10].color!=noncolor &&
			pObjects[border.DL+2-10].color!=noncolor && pObjects[border.DR-2-10].color!=noncolor &&
			pObjects[border.DL+3-10].color!=noncolor && pObjects[border.DR-3-10].color!=noncolor &&
			pObjects[border.DL+4-10].color!=noncolor && pObjects[border.DR-4-10].color!=noncolor) ||

			(pObjects[border.DL-20].color!=noncolor && pObjects[border.DR-20].color!=noncolor && 
			pObjects[border.DL+1-20].color!=noncolor && pObjects[border.DR-1-20].color!=noncolor &&
			pObjects[border.DL+2-20].color!=noncolor && pObjects[border.DR-2-20].color!=noncolor &&
			pObjects[border.DL+3-20].color!=noncolor && pObjects[border.DR-3-20].color!=noncolor &&
			pObjects[border.DL+4-20].color!=noncolor && pObjects[border.DR-4-20].color!=noncolor) ||
			
			(pObjects[border.DL-30].color!=noncolor && pObjects[border.DR-30].color!=noncolor && 
			pObjects[border.DL+1-30].color!=noncolor && pObjects[border.DR-1-30].color!=noncolor &&
			pObjects[border.DL+2-30].color!=noncolor && pObjects[border.DR-2-30].color!=noncolor &&
			pObjects[border.DL+3-30].color!=noncolor && pObjects[border.DR-3-30].color!=noncolor &&
			pObjects[border.DL+4-30].color!=noncolor && pObjects[border.DR-4-30].color!=noncolor) ||
		
			(pObjects[border.DL-40].color!=noncolor && pObjects[border.DR-40].color!=noncolor && 
			pObjects[border.DL+1-40].color!=noncolor && pObjects[border.DR-1-40].color!=noncolor &&
			pObjects[border.DL+2-40].color!=noncolor && pObjects[border.DR-2-40].color!=noncolor &&
			pObjects[border.DL+3-40].color!=noncolor && pObjects[border.DR-3-40].color!=noncolor &&
			pObjects[border.DL+4-40].color!=noncolor && pObjects[border.DR-4-40].color!=noncolor) ||
			
			(pObjects[border.DL-50].color!=noncolor && pObjects[border.DR-50].color!=noncolor && 
			pObjects[border.DL+1-50].color!=noncolor && pObjects[border.DR-1-50].color!=noncolor &&
			pObjects[border.DL+2-50].color!=noncolor && pObjects[border.DR-2-50].color!=noncolor &&
			pObjects[border.DL+3-50].color!=noncolor && pObjects[border.DR-3-50].color!=noncolor &&
			pObjects[border.DL+4-50].color!=noncolor && pObjects[border.DR-4-50].color!=noncolor) || 

			(pObjects[border.DL-60].color!=noncolor && pObjects[border.DR-60].color!=noncolor && 
			pObjects[border.DL+1-60].color!=noncolor && pObjects[border.DR-1-60].color!=noncolor &&
			pObjects[border.DL+2-60].color!=noncolor && pObjects[border.DR-2-60].color!=noncolor &&
			pObjects[border.DL+3-60].color!=noncolor && pObjects[border.DR-3-60].color!=noncolor &&
			pObjects[border.DL+4-60].color!=noncolor && pObjects[border.DR-4-60].color!=noncolor) ||

			(pObjects[border.DL-70].color!=noncolor && pObjects[border.DR-70].color!=noncolor && 
			pObjects[border.DL+1-70].color!=noncolor && pObjects[border.DR-1-70].color!=noncolor &&
			pObjects[border.DL+2-70].color!=noncolor && pObjects[border.DR-2-70].color!=noncolor &&
			pObjects[border.DL+3-70].color!=noncolor && pObjects[border.DR-3-70].color!=noncolor &&
			pObjects[border.DL+4-70].color!=noncolor && pObjects[border.DR-4-70].color!=noncolor) ||

			(pObjects[border.DL-80].color!=noncolor && pObjects[border.DR-80].color!=noncolor && 
			pObjects[border.DL+1-80].color!=noncolor && pObjects[border.DR-1-80].color!=noncolor &&
			pObjects[border.DL+2-80].color!=noncolor && pObjects[border.DR-2-80].color!=noncolor &&
			pObjects[border.DL+3-80].color!=noncolor && pObjects[border.DR-3-80].color!=noncolor &&
			pObjects[border.DL+4-80].color!=noncolor && pObjects[border.DR-4-80].color!=noncolor) ||

			(pObjects[border.DL-90].color!=noncolor && pObjects[border.DR-90].color!=noncolor && 
			pObjects[border.DL+1-90].color!=noncolor && pObjects[border.DR-1-90].color!=noncolor &&
			pObjects[border.DL+2-90].color!=noncolor && pObjects[border.DR-2-90].color!=noncolor &&
			pObjects[border.DL+3-90].color!=noncolor && pObjects[border.DR-3-90].color!=noncolor &&
			pObjects[border.DL+4-90].color!=noncolor && pObjects[border.DR-4-90].color!=noncolor) ||

			(pObjects[border.DL-100].color!=noncolor && pObjects[border.DR-100].color!=noncolor && 
			pObjects[border.DL+1-100].color!=noncolor && pObjects[border.DR-1-100].color!=noncolor &&
			pObjects[border.DL+2-100].color!=noncolor && pObjects[border.DR-2-100].color!=noncolor &&
			pObjects[border.DL+3-100].color!=noncolor && pObjects[border.DR-3-100].color!=noncolor &&
			pObjects[border.DL+4-100].color!=noncolor && pObjects[border.DR-4-100].color!=noncolor) ||

			(pObjects[border.DL-110].color!=noncolor && pObjects[border.DR-110].color!=noncolor && 
			pObjects[border.DL+1-110].color!=noncolor && pObjects[border.DR-1-110].color!=noncolor &&
			pObjects[border.DL+2-110].color!=noncolor && pObjects[border.DR-2-110].color!=noncolor &&
			pObjects[border.DL+3-110].color!=noncolor && pObjects[border.DR-3-110].color!=noncolor &&
			pObjects[border.DL+4-110].color!=noncolor && pObjects[border.DR-4-110].color!=noncolor) ||

			(pObjects[border.DL-120].color!=noncolor && pObjects[border.DR-120].color!=noncolor && 
			pObjects[border.DL+1-120].color!=noncolor && pObjects[border.DR-1-120].color!=noncolor &&
			pObjects[border.DL+2-120].color!=noncolor && pObjects[border.DR-2-120].color!=noncolor &&
			pObjects[border.DL+3-120].color!=noncolor && pObjects[border.DR-3-120].color!=noncolor &&
			pObjects[border.DL+4-120].color!=noncolor && pObjects[border.DR-4-120].color!=noncolor) ||

			(pObjects[border.DL-130].color!=noncolor && pObjects[border.DR-130].color!=noncolor && 
			pObjects[border.DL+1-130].color!=noncolor && pObjects[border.DR-1-130].color!=noncolor &&
			pObjects[border.DL+2-130].color!=noncolor && pObjects[border.DR-2-130].color!=noncolor &&
			pObjects[border.DL+3-130].color!=noncolor && pObjects[border.DR-3-130].color!=noncolor &&
			pObjects[border.DL+4-130].color!=noncolor && pObjects[border.DR-4-130].color!=noncolor) ||

			(pObjects[border.DL-140].color!=noncolor && pObjects[border.DR-140].color!=noncolor && 
			pObjects[border.DL+1-140].color!=noncolor && pObjects[border.DR-1-140].color!=noncolor &&
			pObjects[border.DL+2-140].color!=noncolor && pObjects[border.DR-2-140].color!=noncolor &&
			pObjects[border.DL+3-140].color!=noncolor && pObjects[border.DR-3-140].color!=noncolor &&
			pObjects[border.DL+4-140].color!=noncolor && pObjects[border.DR-4-140].color!=noncolor) ||

			(pObjects[border.DL-150].color!=noncolor && pObjects[border.DR-150].color!=noncolor && 
			pObjects[border.DL+1-150].color!=noncolor && pObjects[border.DR-1-150].color!=noncolor &&
			pObjects[border.DL+2-150].color!=noncolor && pObjects[border.DR-2-150].color!=noncolor &&
			pObjects[border.DL+3-150].color!=noncolor && pObjects[border.DR-3-150].color!=noncolor &&
			pObjects[border.DL+4-150].color!=noncolor && pObjects[border.DR-4-150].color!=noncolor) ||

			(pObjects[border.DL-160].color!=noncolor && pObjects[border.DR-160].color!=noncolor && 
			pObjects[border.DL+1-160].color!=noncolor && pObjects[border.DR-1-160].color!=noncolor &&
			pObjects[border.DL+2-160].color!=noncolor && pObjects[border.DR-2-160].color!=noncolor &&
			pObjects[border.DL+3-160].color!=noncolor && pObjects[border.DR-3-160].color!=noncolor &&
			pObjects[border.DL+4-160].color!=noncolor && pObjects[border.DR-4-160].color!=noncolor) ||

			(pObjects[border.DL-170].color!=noncolor && pObjects[border.DR-170].color!=noncolor && 
			pObjects[border.DL+1-170].color!=noncolor && pObjects[border.DR-1-170].color!=noncolor &&
			pObjects[border.DL+2-170].color!=noncolor && pObjects[border.DR-2-170].color!=noncolor &&
			pObjects[border.DL+3-170].color!=noncolor && pObjects[border.DR-3-170].color!=noncolor &&
			pObjects[border.DL+4-170].color!=noncolor && pObjects[border.DR-4-170].color!=noncolor) ||

			(pObjects[border.DL-180].color!=noncolor && pObjects[border.DR-180].color!=noncolor && 
			pObjects[border.DL+1-180].color!=noncolor && pObjects[border.DR-1-180].color!=noncolor &&
			pObjects[border.DL+2-180].color!=noncolor && pObjects[border.DR-2-180].color!=noncolor &&
			pObjects[border.DL+3-180].color!=noncolor && pObjects[border.DR-3-180].color!=noncolor &&
			pObjects[border.DL+4-180].color!=noncolor && pObjects[border.DR-4-180].color!=noncolor) ||

			(pObjects[border.DL-190].color!=noncolor && pObjects[border.DR-190].color!=noncolor && 
			pObjects[border.DL+1-190].color!=noncolor && pObjects[border.DR-1-190].color!=noncolor &&
			pObjects[border.DL+2-190].color!=noncolor && pObjects[border.DR-2-190].color!=noncolor &&
			pObjects[border.DL+3-190].color!=noncolor && pObjects[border.DR-3-190].color!=noncolor &&
			pObjects[border.DL+4-190].color!=noncolor && pObjects[border.DR-4-190].color!=noncolor))
			{
			 //clear cells
				clearCells (border.DL, border.DR);
			}
}

void createFigure() {
	if (figure.createNewFigure)
		{
			figure.I=1, figure.J=1, figure.L=1, figure.S=1, figure.T=1, figure.Z=1;
			switch(figure.randFigure) 
			{
				case figure.I_figure:
					{
						figure.f1=figures[0];//3;
						figure.f2=figures[1];//4;
						figure.f3=figures[2];//5; 
						figure.f4=figures[3];//6;
						figure.createNewFigure=false;
						break;
					}
				case figure.J_figure:
					{
						figure.f1=figures[4];//4;
						figure.f2=figures[5];//14;
						figure.f3=figures[6];//15; 
						figure.f4=figures[7];//16;
						figure.createNewFigure=false;
						break;
					}
				case figure.L_figure:
					{
						figure.f1=figures[8];//14;
						figure.f2=figures[9];//15; 
						figure.f3=figures[10];//16;
						figure.f4=figures[11];//6;
						figure.createNewFigure=false;
						break;
					}
				case figure.O_figure:
					{
						figure.f1=figures[12];//5;
						figure.f2=figures[13];//6; 
						figure.f3=figures[14];//15; 
						figure.f4=figures[15];//16;
						figure.createNewFigure=false;
						break;
					}
				case figure.S_figure:
					{
						figure.f1=figures[16];//6;
						figure.f2=figures[17];//5; 
						figure.f3=figures[18];//15; 
						figure.f4=figures[19];//14;
						figure.createNewFigure=false;
						break;
					}
				case figure.T_figure:
					{
						figure.f1=figures[20];//5;
						figure.f2=figures[21];//14; 
						figure.f3=figures[22];//15; 
						figure.f4=figures[23];//16;
						figure.createNewFigure=false;
						break;
					}
				case figure.Z_figure:
					{
						figure.f1=figures[24];//4;
						figure.f2=figures[25];//5; 
						figure.f3=figures[26];//15; 
						figure.f4=figures[27];//16;
						figure.createNewFigure=false;
						break;
					}
			}
			if (cRandom) {
				pObjects[figure.f1].color=Colors[figure.Color];
				pObjects[figure.f2].color=Colors[figure.Color];
				pObjects[figure.f3].color=Colors[figure.Color];
				pObjects[figure.f4].color=Colors[figure.Color];
			} 
			else if (cBlue) {
				pObjects[figure.f1].color=Colors[1];
				pObjects[figure.f2].color=Colors[1];
				pObjects[figure.f3].color=Colors[1];
				pObjects[figure.f4].color=Colors[1];
			} else if (cGreen) {
				pObjects[figure.f1].color=Colors[2];
				pObjects[figure.f2].color=Colors[2];
				pObjects[figure.f3].color=Colors[2];
				pObjects[figure.f4].color=Colors[2];
			} else if (cRed) {
				pObjects[figure.f1].color=Colors[3];
				pObjects[figure.f2].color=Colors[3];
				pObjects[figure.f3].color=Colors[3];
				pObjects[figure.f4].color=Colors[3];
			}
		}
}

bool gameOver () {
	//if Game Over
	if ((pObjects[border.DL].color!=noncolor || pObjects[border.DR].color!=noncolor || 
		pObjects[border.DL+1].color!=noncolor || pObjects[border.DR-1].color!=noncolor ||
		pObjects[border.DL+2].color!=noncolor || pObjects[border.DR-2].color!=noncolor ||
		pObjects[border.DL+3].color!=noncolor || pObjects[border.DR-3].color!=noncolor ||
		pObjects[border.DL+4].color!=noncolor || pObjects[border.DR-4].color!=noncolor) &&
			
		(pObjects[border.DL-10].color!=noncolor || pObjects[border.DR-10].color!=noncolor || 
		pObjects[border.DL+1-10].color!=noncolor || pObjects[border.DR-1-10].color!=noncolor ||
		pObjects[border.DL+2-10].color!=noncolor || pObjects[border.DR-2-10].color!=noncolor ||
		pObjects[border.DL+3-10].color!=noncolor || pObjects[border.DR-3-10].color!=noncolor ||
		pObjects[border.DL+4-10].color!=noncolor || pObjects[border.DR-4-10].color!=noncolor) &&

		(pObjects[border.DL-20].color!=noncolor || pObjects[border.DR-20].color!=noncolor || 
		pObjects[border.DL+1-20].color!=noncolor || pObjects[border.DR-1-20].color!=noncolor ||
		pObjects[border.DL+2-20].color!=noncolor || pObjects[border.DR-2-20].color!=noncolor ||
		pObjects[border.DL+3-20].color!=noncolor || pObjects[border.DR-3-20].color!=noncolor ||
		pObjects[border.DL+4-20].color!=noncolor || pObjects[border.DR-4-20].color!=noncolor) &&
			
		(pObjects[border.DL-30].color!=noncolor || pObjects[border.DR-30].color!=noncolor || 
		pObjects[border.DL+1-30].color!=noncolor || pObjects[border.DR-1-30].color!=noncolor ||
		pObjects[border.DL+2-30].color!=noncolor || pObjects[border.DR-2-30].color!=noncolor ||
		pObjects[border.DL+3-30].color!=noncolor || pObjects[border.DR-3-30].color!=noncolor ||
		pObjects[border.DL+4-30].color!=noncolor || pObjects[border.DR-4-30].color!=noncolor) &&
		
		(pObjects[border.DL-40].color!=noncolor || pObjects[border.DR-40].color!=noncolor || 
		pObjects[border.DL+1-40].color!=noncolor || pObjects[border.DR-1-40].color!=noncolor ||
		pObjects[border.DL+2-40].color!=noncolor || pObjects[border.DR-2-40].color!=noncolor ||
		pObjects[border.DL+3-40].color!=noncolor || pObjects[border.DR-3-40].color!=noncolor ||
		pObjects[border.DL+4-40].color!=noncolor || pObjects[border.DR-4-40].color!=noncolor) &&
			
		(pObjects[border.DL-50].color!=noncolor || pObjects[border.DR-50].color!=noncolor || 
		pObjects[border.DL+1-50].color!=noncolor || pObjects[border.DR-1-50].color!=noncolor ||
		pObjects[border.DL+2-50].color!=noncolor || pObjects[border.DR-2-50].color!=noncolor ||
		pObjects[border.DL+3-50].color!=noncolor || pObjects[border.DR-3-50].color!=noncolor ||
		pObjects[border.DL+4-50].color!=noncolor || pObjects[border.DR-4-50].color!=noncolor) && 

		(pObjects[border.DL-60].color!=noncolor || pObjects[border.DR-60].color!=noncolor || 
		pObjects[border.DL+1-60].color!=noncolor || pObjects[border.DR-1-60].color!=noncolor ||
		pObjects[border.DL+2-60].color!=noncolor || pObjects[border.DR-2-60].color!=noncolor ||
		pObjects[border.DL+3-60].color!=noncolor || pObjects[border.DR-3-60].color!=noncolor ||
		pObjects[border.DL+4-60].color!=noncolor || pObjects[border.DR-4-60].color!=noncolor) &&

		(pObjects[border.DL-70].color!=noncolor || pObjects[border.DR-70].color!=noncolor || 
		pObjects[border.DL+1-70].color!=noncolor || pObjects[border.DR-1-70].color!=noncolor ||
		pObjects[border.DL+2-70].color!=noncolor || pObjects[border.DR-2-70].color!=noncolor ||
		pObjects[border.DL+3-70].color!=noncolor || pObjects[border.DR-3-70].color!=noncolor ||
		pObjects[border.DL+4-70].color!=noncolor || pObjects[border.DR-4-70].color!=noncolor) &&

		(pObjects[border.DL-80].color!=noncolor || pObjects[border.DR-80].color!=noncolor || 
		pObjects[border.DL+1-80].color!=noncolor || pObjects[border.DR-1-80].color!=noncolor ||
		pObjects[border.DL+2-80].color!=noncolor || pObjects[border.DR-2-80].color!=noncolor ||
		pObjects[border.DL+3-80].color!=noncolor || pObjects[border.DR-3-80].color!=noncolor ||
		pObjects[border.DL+4-80].color!=noncolor || pObjects[border.DR-4-80].color!=noncolor) &&

		(pObjects[border.DL-90].color!=noncolor || pObjects[border.DR-90].color!=noncolor || 
		pObjects[border.DL+1-90].color!=noncolor || pObjects[border.DR-1-90].color!=noncolor ||
		pObjects[border.DL+2-90].color!=noncolor || pObjects[border.DR-2-90].color!=noncolor ||
		pObjects[border.DL+3-90].color!=noncolor || pObjects[border.DR-3-90].color!=noncolor ||
		pObjects[border.DL+4-90].color!=noncolor || pObjects[border.DR-4-90].color!=noncolor) &&

		(pObjects[border.DL-100].color!=noncolor || pObjects[border.DR-100].color!=noncolor || 
		pObjects[border.DL+1-100].color!=noncolor || pObjects[border.DR-1-100].color!=noncolor ||
		pObjects[border.DL+2-100].color!=noncolor || pObjects[border.DR-2-100].color!=noncolor ||
		pObjects[border.DL+3-100].color!=noncolor || pObjects[border.DR-3-100].color!=noncolor ||
		pObjects[border.DL+4-100].color!=noncolor || pObjects[border.DR-4-100].color!=noncolor) &&

		(pObjects[border.DL-110].color!=noncolor || pObjects[border.DR-110].color!=noncolor || 
		pObjects[border.DL+1-110].color!=noncolor || pObjects[border.DR-1-110].color!=noncolor ||
		pObjects[border.DL+2-110].color!=noncolor || pObjects[border.DR-2-110].color!=noncolor ||
		pObjects[border.DL+3-110].color!=noncolor || pObjects[border.DR-3-110].color!=noncolor ||
		pObjects[border.DL+4-110].color!=noncolor || pObjects[border.DR-4-110].color!=noncolor) &&

		(pObjects[border.DL-120].color!=noncolor || pObjects[border.DR-120].color!=noncolor || 
		pObjects[border.DL+1-120].color!=noncolor || pObjects[border.DR-1-120].color!=noncolor ||
		pObjects[border.DL+2-120].color!=noncolor || pObjects[border.DR-2-120].color!=noncolor ||
		pObjects[border.DL+3-120].color!=noncolor || pObjects[border.DR-3-120].color!=noncolor ||
		pObjects[border.DL+4-120].color!=noncolor || pObjects[border.DR-4-120].color!=noncolor) &&

		(pObjects[border.DL-130].color!=noncolor || pObjects[border.DR-130].color!=noncolor || 
		pObjects[border.DL+1-130].color!=noncolor || pObjects[border.DR-1-130].color!=noncolor ||
		pObjects[border.DL+2-130].color!=noncolor || pObjects[border.DR-2-130].color!=noncolor ||
		pObjects[border.DL+3-130].color!=noncolor || pObjects[border.DR-3-130].color!=noncolor ||
		pObjects[border.DL+4-130].color!=noncolor || pObjects[border.DR-4-130].color!=noncolor) &&

		(pObjects[border.DL-140].color!=noncolor || pObjects[border.DR-140].color!=noncolor || 
		pObjects[border.DL+1-140].color!=noncolor || pObjects[border.DR-1-140].color!=noncolor ||
		pObjects[border.DL+2-140].color!=noncolor || pObjects[border.DR-2-140].color!=noncolor ||
		pObjects[border.DL+3-140].color!=noncolor || pObjects[border.DR-3-140].color!=noncolor ||
		pObjects[border.DL+4-140].color!=noncolor || pObjects[border.DR-4-140].color!=noncolor) &&

		(pObjects[border.DL-150].color!=noncolor || pObjects[border.DR-150].color!=noncolor || 
		pObjects[border.DL+1-150].color!=noncolor || pObjects[border.DR-1-150].color!=noncolor ||
		pObjects[border.DL+2-150].color!=noncolor || pObjects[border.DR-2-150].color!=noncolor ||
		pObjects[border.DL+3-150].color!=noncolor || pObjects[border.DR-3-150].color!=noncolor ||
		pObjects[border.DL+4-150].color!=noncolor || pObjects[border.DR-4-150].color!=noncolor) &&

		(pObjects[border.DL-160].color!=noncolor || pObjects[border.DR-160].color!=noncolor || 
		pObjects[border.DL+1-160].color!=noncolor || pObjects[border.DR-1-160].color!=noncolor ||
		pObjects[border.DL+2-160].color!=noncolor || pObjects[border.DR-2-160].color!=noncolor ||
		pObjects[border.DL+3-160].color!=noncolor || pObjects[border.DR-3-160].color!=noncolor ||
		pObjects[border.DL+4-160].color!=noncolor || pObjects[border.DR-4-160].color!=noncolor) &&

		(pObjects[border.DL-170].color!=noncolor || pObjects[border.DR-170].color!=noncolor || 
		pObjects[border.DL+1-170].color!=noncolor || pObjects[border.DR-1-170].color!=noncolor ||
		pObjects[border.DL+3-170].color!=noncolor || pObjects[border.DR-3-170].color!=noncolor ||
		pObjects[border.DL+4-170].color!=noncolor || pObjects[border.DR-4-170].color!=noncolor) &&

		(pObjects[border.DL-180].color!=noncolor || pObjects[border.DR-180].color!=noncolor || 
		pObjects[border.DL+1-180].color!=noncolor || pObjects[border.DR-1-180].color!=noncolor ||
		pObjects[border.DL+2-180].color!=noncolor || pObjects[border.DR-2-180].color!=noncolor ||
		pObjects[border.DL+3-180].color!=noncolor || pObjects[border.DR-3-180].color!=noncolor ||
		pObjects[border.DL+4-180].color!=noncolor || pObjects[border.DR-4-180].color!=noncolor) &&

		(pObjects[border.DL-190].color!=noncolor || pObjects[border.DR-190].color!=noncolor || 
		pObjects[border.DL+1-190].color!=noncolor || pObjects[border.DR-1-190].color!=noncolor ||
		pObjects[border.DL+2-190].color!=noncolor || pObjects[border.DR-2-190].color!=noncolor ||
		pObjects[border.DL+3-190].color!=noncolor || pObjects[border.DR-3-190].color!=noncolor ||
		pObjects[border.DL+4-190].color!=noncolor || pObjects[border.DR-4-190].color!=noncolor))
		{
			gameover=true;
		}
		return gameover;
}

bool FrameFunc()
{
	float dt=hge->Timer_GetDelta();
	delay=delay+dt;
	static float t=0.0f;
	float tx,ty;
	int id;
	static int lastid=0;
	
	if (!figure.randFigure) figure.randFigure=static_cast<Figures::AllFigures>(hge->Random_Int(1,7));
	if (cRandom) {
		if (!figure.Color) figure.Color=hge->Random_Int(0,3);
	} else if (cBlue) {
		figure.Color=1;
	} else if (cGreen) {
		figure.Color=2;
	} else if (cRed) {
		figure.Color=3;
	}

	 if (!gameOver()) {
		 //Render Random figure and color if game not over
		 createFigure();
	}
		//clear old position
			if ((figure.f1<190 && figure.f2<190 && figure.f3<190 && figure.f4<190) && 
				(pObjects[figure.f1+10].color==noncolor && figure.f1+10!=figure.f2 && figure.f1+10!=figure.f3 && figure.f1+10!=figure.f4) || 
				(pObjects[figure.f2+10].color==noncolor && figure.f2+10!=figure.f1 && figure.f2+10!=figure.f3 && figure.f2+10!=figure.f4) || 
				(pObjects[figure.f3+10].color==noncolor && figure.f3+10!=figure.f1 && figure.f3+10!=figure.f2 && figure.f3+10!=figure.f4) || 
				(pObjects[figure.f4+10].color==noncolor && figure.f4+10!=figure.f1 && figure.f4+10!=figure.f2 && figure.f4+10!=figure.f3)) {
					if ((pObjects[figure.f1+10].color!=noncolor && figure.f1+10!=figure.f2 && figure.f1+10!=figure.f3 && figure.f1+10!=figure.f4) ||
						(pObjects[figure.f2+10].color!=noncolor && figure.f2+10!=figure.f1 && figure.f2+10!=figure.f3 && figure.f2+10!=figure.f4) ||
						(pObjects[figure.f3+10].color!=noncolor && figure.f3+10!=figure.f1 && figure.f3+10!=figure.f2 && figure.f3+10!=figure.f4) ||
						(pObjects[figure.f4+10].color!=noncolor && figure.f4+10!=figure.f1 && figure.f4+10!=figure.f2 && figure.f4+10!=figure.f3)) {
							//to do nothing
					} else {       
						pObjects[figure.f1].color=noncolor;
						pObjects[figure.f2].color=noncolor;
						pObjects[figure.f3].color=noncolor;
						pObjects[figure.f4].color=noncolor;
					}
			}

		clearLine(); //when one of lines have a color

		if ((figure.f1>=190 || figure.f2>=190 || figure.f3>=190 || figure.f4>=190) || 
				(pObjects[figure.f1+10].color!=noncolor && figure.f1+10!=figure.f2 && figure.f1+10!=figure.f3 && figure.f1+10!=figure.f4) ||
				(pObjects[figure.f2+10].color!=noncolor && figure.f2+10!=figure.f1 && figure.f2+10!=figure.f3 && figure.f2+10!=figure.f4) ||
				(pObjects[figure.f3+10].color!=noncolor && figure.f3+10!=figure.f1 && figure.f3+10!=figure.f2 && figure.f3+10!=figure.f4) ||
				(pObjects[figure.f4+10].color!=noncolor && figure.f4+10!=figure.f1 && figure.f4+10!=figure.f2 && figure.f4+10!=figure.f3)) {

				figure.f1=MAX_OBJECTS;
				figure.f2=MAX_OBJECTS;
				figure.f3=MAX_OBJECTS;
				figure.f4=MAX_OBJECTS;
				figure.createNewFigure = true;

				figure.randFigure=static_cast<Figures::AllFigures>(hge->Random_Int(1,7));
				if (cRandom) {
					figure.Color=hge->Random_Int(0,3);
				} else if (cBlue) {
					figure.Color=1;
				} else if (cGreen) {
					figure.Color=2;
				} else if (cRed) {
					figure.Color=3;
				}
				
				border.LS=0;
				border.RS=9;
		}
		switch(hge->Input_GetKey())
		{
			case HGEK_ESCAPE:	
				{
					if (mMenu) {
						gui->Leave();
						quad.tex=hge->Texture_Load("bg2.png");
						mMenu = false;
					} else {
						gui->SetFocus(1);
						gui->Enter();
						quad.tex=hge->Texture_Load("bg.png");
						mMenu = true;
					}
					break;
				}
			// Do some movement calculations and collision detection
			case HGEK_LEFT:	
				{
					if (!mMenu)
					{
						if(delay>(1.0/level)) 
						{
							if (pObjects[figure.f1-1].color==noncolor && pObjects[figure.f2-1].color==noncolor && pObjects[figure.f3-1].color==noncolor && pObjects[figure.f4-1].color==noncolor) 
							{
								figure.goLeft(&border.LS);
							}
							delay=0;
						}
					} 
					break;
				}
			case HGEK_RIGHT:	
				{
					if (!mMenu)
					{
						if(delay>(1.0/level)) 
						{
							if (pObjects[figure.f1+1].color==noncolor && pObjects[figure.f2+1].color==noncolor && pObjects[figure.f3+1].color==noncolor && pObjects[figure.f4+1].color==noncolor) 
							{
								figure.goRight(&border.RS);
							}
							delay=0;
						}
					} 
					break;
				}
			case HGEK_UP:	    
				{
					if (!mMenu)
					{
						figure.rotateFigure(&border.LS, &border.RS);		
					} 
					break;
				}
			case HGEK_DOWN:	    
				{
					if (!mMenu)
					{
						figure.goDown(&border.LS, &border.RS);
					} 
					break;
				}
			default: 
			{
				if (!mMenu) 
				{
					if(delay>(3.0/level)) 
					{
						if (figure.f1<190 && figure.f2<190 && figure.f3<190 && figure.f4<190) 
						{
							figure.f1+=10; figure.f2+=10; figure.f3+=10; figure.f4+=10; 
							border.LS+=10;	border.RS+=10;
						}
						delay=0;
					}
				}
			}
		}

		if (figure.f1<MAX_OBJECTS && figure.f2<MAX_OBJECTS && figure.f3<MAX_OBJECTS && figure.f4<MAX_OBJECTS) {
			pObjects[figure.f1].color=Colors[figure.Color];
			pObjects[figure.f2].color=Colors[figure.Color];
			pObjects[figure.f3].color=Colors[figure.Color];
			pObjects[figure.f4].color=Colors[figure.Color];
		}
	
		// We update the GUI and take an action if
		// one of the menu items was selected
		id=gui->Update(dt);
		if(id == -1)
		{
			switch(lastid)
			{
				case CONTINUE:
					{
						quad.tex=hge->Texture_Load("bg2.png");
						lastid=0;
						gui->Leave();
						mMenu = false;
						//gui->SetFocus(1);
						//gui->Enter();
						break;
					}
				case NEWGAME:
					{
						quad.tex=hge->Texture_Load("bg2.png");
						lastid=0;
						gui->Leave();
						mMenu = false;
						for (int n=0; n<MAX_OBJECTS; n++) {
							pObjects[n].color=noncolor;
						}
						figure.f1=MAX_OBJECTS; figure.f2=MAX_OBJECTS; figure.f3=MAX_OBJECTS; figure.f4=MAX_OBJECTS;
						level=3;
						score=0;
						count=0;
						border.LS=0, border.RS=9;
						border.DL=190, border.DR=199;
						figure.I=1, figure.J=1, figure.L=1, figure.S=1, figure.T=1, figure.Z=1;
						gameover=false;
						figure.createNewFigure = true;
						//gui->SetFocus(1);
						//gui->Enter();
						break;
					}
				case SETTINGS:
					{
						gui->ShowCtrl(1, false);
						gui->ShowCtrl(2, false);
						gui->ShowCtrl(3, false);
						gui->ShowCtrl(4, false);
						gui->ShowCtrl(5, cBlue);
						gui->ShowCtrl(6, cGreen);
						gui->ShowCtrl(7, cRed);
						gui->ShowCtrl(8, cRandom);
						gui->ShowCtrl(9, true);

						gui->EnableCtrl(1, false);
						gui->EnableCtrl(2, false);
						gui->EnableCtrl(3, false);
						gui->EnableCtrl(4, false);
						gui->EnableCtrl(5, cBlue);
						gui->EnableCtrl(6, cGreen);
						gui->EnableCtrl(7, cRed);
						gui->EnableCtrl(8, cRandom);
						gui->EnableCtrl(9, true);
						gui->SetFocus(1);
						gui->Enter();
						break;
					}
				case EXIT: 
					{
						return true;
					}
				case BLUE:
					{
						gui->ShowCtrl(6, true);
						gui->ShowCtrl(5, false);
						gui->EnableCtrl(6, true);
						gui->EnableCtrl(5, false);
						cBlue=false, cGreen=true, cRed=false, cRandom=false;
						gui->SetFocus(1);
						gui->Enter();
						break;
					}
				case GREEN:
					{
						gui->ShowCtrl(7, true);
						gui->ShowCtrl(6, false);
						gui->EnableCtrl(7, true);
						gui->EnableCtrl(6, false);
						cBlue=false, cGreen=false, cRed=true, cRandom=false;
						gui->SetFocus(1);
						gui->Enter();
						break;
					}
				case RED:
					{
						gui->ShowCtrl(8, true);
						gui->ShowCtrl(7, false);
						gui->EnableCtrl(8, true);
						gui->EnableCtrl(7, false);
						cBlue=false, cGreen=false, cRed=false, cRandom=true;
						gui->SetFocus(1);
						gui->Enter();
						break;
					}
				case RAND:
					{
						gui->ShowCtrl(5, true);
						gui->ShowCtrl(8, false);
						gui->EnableCtrl(5, true);
						gui->EnableCtrl(8, false);
						cBlue=true, cGreen=false, cRed=false, cRandom=false;
						gui->SetFocus(1);
						gui->Enter();
						break;
					}
				case BACK:
					{
						//show the menu
						gui->ShowCtrl(1, true);
						gui->ShowCtrl(2, true);
						gui->ShowCtrl(3, true);
						gui->ShowCtrl(4, true);

						gui->EnableCtrl(1, true);
						gui->EnableCtrl(2, true);
						gui->EnableCtrl(3, true);
						gui->EnableCtrl(4, true);
						//hide settings
						gui->ShowCtrl(5, false);
						gui->ShowCtrl(6, false);
						gui->ShowCtrl(7, false);
						gui->ShowCtrl(8, false);
						gui->ShowCtrl(9, false);

						gui->EnableCtrl(5, false);
						gui->EnableCtrl(6, false);
						gui->EnableCtrl(7, false);
						gui->EnableCtrl(8, false);
						gui->EnableCtrl(9, false);
						gui->SetFocus(1);
						gui->Enter();
						break;
					}
			}
		}
		else if(id) 
		{
			lastid=id; gui->Leave();
		}

		// Here we update our background animation
		if (mMenu) {
			t+=dt;
			tx=50*cosf(t/60);
			ty=50*sinf(t/60);

			quad.v[0].tx=tx;        quad.v[0].ty=ty;
			quad.v[1].tx=tx+800/64; quad.v[1].ty=ty;
			quad.v[2].tx=tx+800/64; quad.v[2].ty=ty+600/64;
			quad.v[3].tx=tx;        quad.v[3].ty=ty+600/64;
		}
	return false;
}

bool RenderFunc()
{
	// Render graphics
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	hge->Gfx_RenderQuad(&quad);
	gui->Render();
	if (!mMenu) {
		for(int i=0;i<nObjects;i++)
		{
			spr->SetColor(pObjects[i].color);
			spr->RenderEx(pObjects[i].x, pObjects[i].y, pObjects[i].rot, pObjects[i].scale);		
		}
	}

	if (!mMenu) {
		fnt->SetColor(0xFFFFE080);
		fnt->printf(5, 5, HGETEXT_LEFT, "Level: %.0f\nScore: %d", level-2, score);
		if (gameover) {
			fnt->SetColor(0xFF000000);
			fnt->printf(320, 300, HGETEXT_LEFT, "Game Over");
		}
	}
	hge->Gfx_EndScene();

	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ifstream lvl, faf;
	int i, min=1, max=9;
	float vary=80.0f;
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_LOGFILE, "tetris.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Tetris");
	hge->System_SetState(HGE_FPS, 100);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_USESOUND, false);
	hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
	hge->System_SetState(HGE_SCREENBPP, 32);

	if(hge->System_Initiate())
	{
		// Load textures
		quad.tex=hge->Texture_Load("bg.png");
		texc=hge->Texture_Load("cursor.png");
		tex=hge->Texture_Load("cube.png");
		lvl.open("level.txt", ios::in);
		faf.open("figures_and_field.txt", ios::in);

		if(!quad.tex || !texc || !tex || !lvl.is_open() || !faf.is_open())
		{
			// If one of the data files is not found, display
			// an error message and shutdown.
			MessageBox(NULL, "Can't load BG.PNG, CURSOR.PNG, CUBE.PNG, LEVEL.TXT, FIGURES_AND_FIELD.txt", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			hge->System_Shutdown();
			hge->Release();
			return 0;
		} else {
			for (int n=0;n<9;n++)
			{
				lvl >> levelgame[n];
			}
			for (int q=0;q<29;q++)
			{
				faf >> figures[q];
			}
		}

		MAX_OBJECTS=figures[28];
		figure.f1=MAX_OBJECTS, figure.f2=MAX_OBJECTS, figure.f3=MAX_OBJECTS, figure.f4=MAX_OBJECTS;

		// Set up the quad we will use for background animation
		quad.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

		for(int i=0;i<4;i++)
		{
			// Set up z-coordinate of vertices
			quad.v[i].z=0.5f;
			// Set up color. The format of DWORD col is 0xAARRGGBB
			quad.v[i].col=0xFFFFFFFF;
		}

		quad.v[0].x=0; quad.v[0].y=0; 
		quad.v[1].x=800; quad.v[1].y=0; 
		quad.v[2].x=800; quad.v[2].y=600; 
		quad.v[3].x=0; quad.v[3].y=600; 

		// Create and set up a sprite
		spr=new hgeSprite(tex, 0,0,64,64);
		spr->SetColor(0xFF000000);
		spr->SetHotSpot(32,32);

		// Load the font, create the cursor sprite
		fnt=new hgeFont("font1.fnt");
		sprc=new hgeSprite(texc,0,0,32,32);

		// Initialize objects list
		pObjects=new sprObject[MAX_OBJECTS];
		nObjects=200;

		//Create field
		for(i=1;i<MAX_OBJECTS;i++)
		{
			if (i>=min && i<=max) {
				pObjects[min-1].x=260.0f;
				pObjects[min-1].y=vary;
				pObjects[min-1].dx=0.0f;
				pObjects[min-1].dy=0.0f;
				
				pObjects[min-1].scale=0.4f; 
				pObjects[min-1].dscale=0.4f; 			
				pObjects[min-1].rot= 0.0f; 
				pObjects[min-1].drot= 430.0f;

				pObjects[i].x=pObjects[i-1].x+26.0f;
				pObjects[i].y=vary;
				pObjects[i].dx=0.0f;
				pObjects[i].dy=0.0f;

			} else 
			{
				min+=10; max+=10; vary+=26.0f;
			}
			pObjects[i].scale=0.4f; 
			pObjects[i].dscale=0.4f; 			
			pObjects[i].rot= 0.0f; 
			pObjects[i].drot= 430.0f;
		}

		// Create and initialize the GUI
		gui=new hgeGUI();
		gui->AddCtrl(new hgeGUIMenuItem(1,fnt,400,200,0.0f,"Continue"));
		gui->AddCtrl(new hgeGUIMenuItem(2,fnt,400,240,0.1f,"New game"));
		gui->AddCtrl(new hgeGUIMenuItem(3,fnt,400,280,0.2f,"Settings"));
		gui->AddCtrl(new hgeGUIMenuItem(4,fnt,400,320,0.3f,"Exit"));

		//Settings
		gui->AddCtrl(new hgeGUIMenuItem(5,fnt,400,200,0.3f,"Color: blue"));
		gui->AddCtrl(new hgeGUIMenuItem(6,fnt,400,200,0.3f,"Color: green"));
		gui->AddCtrl(new hgeGUIMenuItem(7,fnt,400,200,0.3f,"Color: red"));
		gui->AddCtrl(new hgeGUIMenuItem(8,fnt,400,200,0.3f,"Color: random"));
		gui->AddCtrl(new hgeGUIMenuItem(9,fnt,400,240,0.3f,"Back"));

		//hide settings
		gui->ShowCtrl(5, false);
		gui->ShowCtrl(6, false);
		gui->ShowCtrl(7, false);
		gui->ShowCtrl(8, false);
		gui->ShowCtrl(9, false);

		gui->EnableCtrl(5, false);
		gui->EnableCtrl(6, false);
		gui->EnableCtrl(7, false);
		gui->EnableCtrl(8, false);
		gui->EnableCtrl(9, false);

		gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		gui->SetCursor(sprc);
		gui->SetFocus(1);
		gui->Enter();

		// Let's rock now!
		hge->System_Start();

		// Delete created objects and free loaded resources
		delete[] pObjects;
		delete gui;
		delete fnt;
		delete spr;
		delete sprc;
		lvl.close();
		faf.close();
		hge->Texture_Free(tex);
		hge->Texture_Free(texc);
		hge->Texture_Free(quad.tex);
	}
	else
    {
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error",
				   MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
    }

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}