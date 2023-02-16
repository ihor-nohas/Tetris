#pragma once

class Figures
{
public:
	enum AllFigures {
		I_figure=1, 
		J_figure, 
		L_figure, 
		O_figure,
		S_figure, 
		T_figure, 
		Z_figure
	};
	AllFigures randFigure;
	int Color;
	int f1, f2, f3, f4; //position figures
	int I, J, L, S, T, Z; //for rotate figure
	bool createNewFigure;
	void rotateFigure(int *_LS, int *_RS);
	void goLeft(int *_LS);
	void goRight(int *_RS);
	void goDown(int *_LS, int *_RS);
	Figures(void);
	~Figures(void);
};
