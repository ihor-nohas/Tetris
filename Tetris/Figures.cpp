#include "Figures.h"

Figures::Figures(void)
{
	I=1, J=1, L=1, S=1, T=1, Z=1;
	createNewFigure = true;
}

void Figures::rotateFigure(int *_LS, int *_RS) {
	if ((f1!=0 && f2!=0 && f3!=0 && f4!=0 && f1!=9 && f2!=9 && f3!=9 && f4!=9) && //fixed bugs
		(f1!=0+1 && f2!=0+1 && f3!=0+1 && f4!=0+1) && (f1!=9-1 && f2!=9-1 && f3!=9-1 && f4!=9-1) &&
		(f1!=0+2 && f2!=0+2 && f3!=0+2 && f4!=0+2) && (f1!=9-2 && f2!=9-2 && f3!=9-2 && f4!=9-2) &&
		(f1!=0+3 && f2!=0+3 && f3!=0+3 && f4!=0+3) && (f1!=9-3 && f2!=9-3 && f3!=9-3 && f4!=9-3) &&
		(f1!=0+4 && f2!=0+4 && f3!=0+4 && f4!=0+4) && (f1!=9-4 && f2!=9-4 && f3!=9-4 && f4!=9-4)) {
		if (f1<190 && f2<190 && f3<190 && f4<190)
		{
			switch(randFigure)
			{
				case I_figure:
				{
					if ((f1>=20 && f2>=20 && f3>=20 && f4>=20) || (f1>=29 && f2>=29 && f3>=29 && f4>=29)) {
						if (I==1) {
							f1-=18;
							f2-=9;
							//f3=f3;
							f4+=9;
							I++;
							break;
						}
						if (I==2) {
							if (f1==(*_RS) || f2==(*_RS) || f3==(*_RS) || f4==(*_RS)) { //Right border
								f1+=10;
								f2-=1;
								f3-=12;
								f4-=23;	
							} else if (f1==(*_RS)-1 || f2==(*_RS)-1 || f3==(*_RS)-1 || f4==(*_RS)-1) {
								f1+=21;
								f2+=10;
								f3-=1;
								f4-=12;
							} else if (f1==(*_LS) || f2==(*_LS) || f3==(*_LS) || f4==(*_LS)) { //Left border
								f1+=23; 
								f2+=12; 
								f3+=1; 
								f4-=10; 
							} else if (f1==(*_LS)+1 || f2==(*_LS)+1 || f3==(*_LS)+1 || f4==(*_LS)+1) {
								f1+=12; 
								f2+=1; 
								f3-=10; 
								f4-=21;
							} else {
								f1+=22; 
								f2+=11; 
								//f3=f3; 
								f4-=11;
							}
							//Fixed some bugs
							if ((*_RS)>f3) 
							{
								(*_RS)-=10; (*_LS)-=10;
							} 
							if ((*_RS)<f3) 
							{
								(*_RS)+=10; (*_LS)+=10;
							}
							I++;
							break;
						}
						if (I==3) {
							f1+=18;
							f2+=9;
							//f3=f3;
							f4-=9;
							I++;
							break;
						}
						if (I==4) {
							if (f1==(*_RS) || f2==(*_RS) || f3==(*_RS) || f4==(*_RS)) { //Right border
								f1-=13;
								f2-=2;
								f3+=9;
								f4+=20;
							} else if (f1==(*_LS) || f2==(*_LS) || f3==(*_LS) || f4==(*_LS)) { //Left border
								f1-=20;
								f2-=9;
								f3+=2;
								f4+=13;
							} else if (f1==(*_LS)+1 || f2==(*_LS)+1 || f3==(*_LS)+1 || f4==(*_LS)+1) {
								f1-=1;
								f2+=10;
								f3+=21;
								f4+=32;
							} else {
								f1-=22;
								f2-=11;
								//f3=f3;
								f4+=11;
							}
							//Fixed some bugs
							if ((*_RS)>f3) 
							{
								(*_RS)-=10; (*_LS)-=10;
							} 
							if ((*_RS)<f3) 
							{
								(*_RS)+=10; (*_LS)+=10;
							}
							I=1;
							break;
						}
					}
					break;
				}
				case J_figure:
				{
					if (J==1) {
						f1+=2;
						f2-=9;
						//f3=f3; 
						f4+=9;
						J++;
						break;
				}
					if (J==2) {
						if (f1==(*_LS) || f2==(*_LS) || f3==(*_LS) || f4==(*_LS)) { //fixed bug
							f1+=21;
							f2+=12;
							f3+=1; 
							f4-=10;
						} else {
							f1+=20; 
							f2+=11; 
							//f3=f3; 
							f4-=11; 
						}
						J++;
						break;
					}
					if (J==3) {
						f1-=2; 
						f2+=9;
						//f3=f3; 
						f4-=9; 
						J++;
						break;
					}
					if (J==4) {
						if (f1==(*_RS) || f2==(*_RS) || f3==(*_RS) || f4==(*_RS)) { //fixed bug
							f1-=21;
							f2-=12; 
							f3-=1; 
							f4+=10; 
						} else { 
							f1-=20;
							f2-=11;
							//f3=f3; 
							f4+=11;
						}
						J=1;
						break;
					}
				}
				case L_figure:
				{
					if (L==1) {
						f1-=9;
						//f2=f2; 
						f3+=9;
						f4+=20;
						L++;
						break;
					}
					if (L==2) {
						if (f1==(*_LS) || f2==(*_LS) || f3==(*_LS) || f4==(*_LS)) { //fixed bug
							f1+=12;
							f2+=1; 
							f3-=10;
							f4-=1;
						} else {
							f1+=11;
							//figure.f2=f2; 
							f3-=11;
							f4-=2;
						}
						L++;
						break;
					}
					if (L==3) {
						f1+=9;
						//figure.f2=f2; 
						f3-=9;
						f4-=20;
						L++;
						break;
					}
					if (L==4) {
						if (f1==(*_RS) || f2==(*_RS) || f3==(*_RS) || f4==(*_RS)) { //fixed bug
							f1-=12;
							f2-=1; 
							f3+=10;
							f4+=1;
						} else {
							f1-=11;
							//figure.f2=f2; 
							f3+=11;
							f4+=2;
						}
						L=1;
						break;
					}
				}
				case O_figure:
				{
					//not rotate
					break;
				}
				case S_figure:
				{
					if (S==1) {
						f1+=9;
						//figure.f2=f2; 
						f3-=11;
						f4-=20;
						S++;
						break;
					}
					if (S==2) {
						if (f1==(*_RS) || f2==(*_RS) || f3==(*_RS) || f4==(*_RS)) { //fixed bug
							f1-=12;
							f2-=1; 
							f3-=10;
							f4+=1;
						} else {
							f1-=11;
							//figure.f2=f2; 
							f3-=9;
							f4+=2;
						}
					S++;
					break;
					}
					if (S==3) {
						f1-=9;
						//f2=f2; 
						f3+=11; 
						f4+=20;
						S++;
						break;
					}
					if (S==4) {
						if (f1==(*_LS) || f2==(*_LS) || f3==(*_LS) || f4==(*_LS)) { //fixed bug
							f1+=12;
							f2+=1; 
							f3+=10;
							f4-=1;
						} else {
							f1+=11;
							//figure.f2=f2; 
							f3+=9; 
							f4-=2;
						}
						S=1;
						break;
					}
				}
				case T_figure:
				{
					if (T==1) {
						f1+=11;
						f2-=9;
						//figure.f3=f3; 
						f4+=9;
						T++;
						break;
					}
					if (T==2) {
						if (f1==(*_LS) || f2==(*_LS) || f3==(*_LS) || f4==(*_LS)) { //fixed bug
							f1+=10;
							f2+=12;
							f3+=1; 
							f4-=10;
						} else {
							f1+=9;
							f2+=11;
							//f3=f3; 
							f4-=11;
						}
						T++;
						break;
					}
					if (T==3) {
						f1-=11;
						f2+=9;
						//f3=f3; 
						f4-=9;
						T++;
						break;
					}
					if (T==4) {
						if (f1==(*_RS) || f2==(*_RS) || f3==(*_RS) || f4==(*_RS)) { //fixed bug
							f1-=10;
							f2-=12; 
							f3-=1; 
							f4+=10;
						} else {
							f1-=9;
							f2-=11; 
							//f3=f3; 
							f4+=11;
						}
						T=1;
						break;
					}
				}
				case Z_figure:
				{
					if (Z==1) {
						f1-=9;
						//f2=f2; 
						f3-=11;
						f4-=2;
						Z++;
						break;
					}
					if (Z==2) {
						if (f1==(*_RS) || f2==(*_RS) || f3==(*_RS) || f4==(*_RS)) { //fixed bug
							f1+=10;
							f2-=1; 
							f3-=10;
							f4-=21;
						} else {
							f1+=11;
							//figure.f2=f2; 
							f3-=9;
							f4-=20;
						}
						Z++;
						break;
					}
					if (Z==3) {
						f1+=9;
						//f2=f2; 
						f3+=11;
						f4+=2;
						Z++;
						break;
					}
					if (Z==4) {
						if (f1==(*_LS) || f2==(*_LS) || f3==(*_LS) || f4==(*_LS)) { //fixed bug
							f1-=10;
							f2+=1; 
							f3+=10;
							f4+=21;
						} else {
							f1-=11;
							//f2=f2; 
							f3+=9;
							f4+=20;
						}
						Z=1;
						break;
					}
				}
			}
		}
	}
}

void Figures::goLeft(int *_LS) {
	if ((f1!=(*_LS)-10 && f2!=(*_LS)-10 && f3!=(*_LS)-10 && f4!=(*_LS)-10) && 
		(f1!=(*_LS) && f2!=(*_LS) && f3!=(*_LS) && f4!=(*_LS)) && 
		(f1!=(*_LS)+10 && f2!=(*_LS)+10 && f3!=(*_LS)+10 && f4!=(*_LS)+10) && 
		(f1!=(*_LS)+20 && f2!=(*_LS)+20 && f3!=(*_LS)+20 && f4!=(*_LS)+20) && 
		(f1!=(*_LS)+30 && f2!=(*_LS)+30 && f3!=(*_LS)+30 && f4!=(*_LS)+30)) 
		{
			if (f1<190 && f2<190 && f3<190 && f4<190) 
			{
					f1--; f2--; f3--; f4--;
			}
		}
}

void Figures::goRight(int *_RS) {
	if ((f1!=(*_RS)-10 && f2!=(*_RS)-10 && f3!=(*_RS)-10 && f4!=(*_RS)-10) && 
		(f1!=(*_RS) && f2!=(*_RS) && f3!=(*_RS) && f4!=(*_RS)) && 
		(f1!=(*_RS)+10 && f2!=(*_RS)+10 && f3!=(*_RS)+10 && f4!=(*_RS)+10) && 
		(f1!=(*_RS)+20 && f2!=(*_RS)+20 && f3!=(*_RS)+20 && f4!=(*_RS)+20) && 
		(f1!=(*_RS)+30 && f2!=(*_RS)+30 && f3!=(*_RS)+30 && f4!=(*_RS)+30)) 
		{
			if (f1<190 && f2<190 && f3<190 && f4<190) 
			{
				f1++; f2++; f3++; f4++;
			}
		}
}

void Figures::goDown(int *_LS, int *_RS) {
	if (f1<190 && f2<190 && f3<190 && f4<190) 
	{
		f1+=10; f2+=10; f3+=10; f4+=10;
		(*_LS)+=10;	(*_RS)+=10;
	}
}

Figures::~Figures(void)
{
}
