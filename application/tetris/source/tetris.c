#include "rexy.h"
#include "rexy_haldevice.h"

#include "rexy_ctl_popup.h"
//#include "registry.h"
#include "../../../application/tetris/header/tetris.h"
#include "application.h"

#define SCRW trscreenWidth
#define SCRH trscreenHeight

#define MAX_LOW 20 
#define MAX_COL 10
#define RECTW 15
#define RECTH 15

#define LEFT_SPACE 15//20
#define RIGHT_SPACE 15//20
#define BOTTOM_SPACE 20

#define WIN_GAME_COUNT 10

#define OPTION_RECT_X LEFT_SPACE + 10 //40
#define OPTION_RECT_Y (LEFT_SPACE + (MAX_COL * RECTW) - 10 ) //(trscreenWidth-OPTION_RECT_X)
#define OPTION_RECT_TOP 80
#define OPTION_RECT_BOTTOM (SCRH-50)

#define SCORE_RECT_X LEFT_SPACE + RIGHT_SPACE + (MAX_COL * RECTW) + 5
#define SCORE_RECT_Y SCORE_RECT_X + 50
#define SCORE_RECT_TOP 80
#define SCORE_RECT_BOTTOM (SCORE_RECT_TOP + 80 )

#define NEXT_RECT_X LEFT_SPACE + RIGHT_SPACE + (MAX_COL * RECTW) + 5
#define NEXT_RECT_Y NEXT_RECT_X + 50
#define NEXT_RECT_TOP 180
#define NEXT_RECT_BOTTOM (NEXT_RECT_TOP + 90 )

typedef enum
{
	TLSTART,
	TLGAME_MILESTONE_NEXT_GAME,
	TLGAME_FAIL_MESSAGE,
	GAMING
}GAMESTATE;

typedef enum
{
	SPIN1,
	SPIN2,
	SPIN3,
	SPIN4
}SHAPESTATE;

static rint32 trtypeShape;//현재 도형모양을 나타낸다.
static SHAPESTATE trShapeState;//회전 상태를 나타낸다.
static TetrisStateID trgameState;
static rint32 trtimeInterVal;//timer가 발생하는 시간 간격..
static ruint8 trmaxShape;
static ruint8 trlowColData[MAX_LOW][MAX_COL];

static int trscreenWidth;
static int trscreenHeight;

static rint32 trlevel;
static rint32 trfocus;
static rect trshapeRect[4];
static rect trshapeNextRect[4];

static rint32 g_trScore;
static rint32 g_trLineScore;
static rint32 g_trHiScore;
static rint32 g_trtypeNextShape;
static rect g_trGameScreen;

static void PrvTetInit(void);
static void PrvTetDraw(WND wnd);
static void PrvTetInitDraw(WND wnd);

/***no data modify**/
static void PrvTetDrawShape(WND wnd);
static void PrvTetDrawWholeShape(WND wnd);

/**data modify**/
static void PrvTetChangePosition(void);//key에 의한 position값을 바군다.
static void PrvTetFillLowWithZero(rint32 low);
static void PrvTetLineDeleteEffect(WND wnd, rint32 line);
static int PrvTetCheckFilledLow(rint32 low);// full된 low가 없다면 -1을 반환.
static void PrvTetOneStepDown(rint32 low);
static void PrvTetDrawBackGroundGaming(WND wnd);
static bool PrvTetCheckMobiltyForDown(void);//움직일수 잇는지 여부 검사..
static bool PrvTetCheckMobiltyForLeft(void);
static bool PrvTetCheckMobiltyForRight(void);

static void PrvTetIncreaseY(void);
static void PrvTetIncreaseX(void);
static void PrvTetDecreaseX(void);
static rint32 PrvTetCheckGameEnd(void);
static void PrvTetMakeNewRect(void);
static void PrvTetMakeNewNextRect(void);
static void PrvTetBlockDown(WND wnd);

static void PrvTetInitHandler(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);
static void PrvTetDrawHandler(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);
static void PrvTetKeyDownHandler(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);
static void PrvTetTimerHandler(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);

st_TetrisStateHandle g_TetHandlerList[] = 
{
	AWM_CREATE, PrvTetInitHandler,
	AWM_PAINT, PrvTetDrawHandler,
	AWM_KEYDOWN, PrvTetKeyDownHandler,
	AWM_TIMER,	PrvTetTimerHandler
};

static void PrvTetDrawShape(WND wnd)
{
	rint32 i;
	for (i=0; i<4; i++)
		if (trshapeRect[i].top >= 0)
			GdiRectangle(wnd, &trshapeRect[i]);
}

static void PrvTetDrawNextShape(WND wnd)
{
	rint32 i;
	for (i=0; i<4; i++)
		GdiRectangle(wnd, &trshapeNextRect[i]);
}

static void PrvTetDrawWholeShape(WND wnd)
{
	rint32 i,j;
	COLORVAL tmp;
	rect rect1;
	i=0;
	j=0;
	
	while(i<MAX_LOW)
	{
		j=0;
		while(j<MAX_COL)
		{
			if(trlowColData[i][j]==1)
			{
				//i*RECTW=left를 나타낸다. //j*RECTH=top를 나타낸다.
				rect1.top=i*RECTH;
				rect1.bottom=rect1.top + RECTH;
				rect1.left=j*RECTW + LEFT_SPACE;
				rect1.right=rect1.left+RECTW;
				tmp = GdiSetBkColor(wnd,COLOR_GREEN);
				GdiRectangle(wnd,&rect1);
				GdiSetBkColor(wnd,tmp);
			}
			j++;
		}
		i++;
	}
}

static void PrvTetChangePosition()
{
	rint32 i;
	rect tmpRect[4];
	SHAPESTATE trShapeState1;
	
	trShapeState1=trShapeState;
	MemSet(&tmpRect[0],0,sizeof(rect)*4);
	MemCopy(&tmpRect[0],&trshapeRect[0], sizeof(rect)*4);
	switch(trtypeShape)
	{
		case 0:
			switch(trShapeState)
			{
				case SPIN1:
					trshapeRect[0].top =trshapeRect[0].top + RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left + RECTW;
					trshapeRect[0].right=trshapeRect[0].left+ RECTW;
					
					trshapeRect[2].top =trshapeRect[2].top - RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left - RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top - RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top+ RECTH;
					trshapeRect[3].left=trshapeRect[3].left + RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN2;
				break;
				case SPIN2:
					trshapeRect[0].top =trshapeRect[0].top - RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left + RECTW;
					trshapeRect[0].right=trshapeRect[0].left+ RECTW;

					trshapeRect[2].top =trshapeRect[2].top + RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left - RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top - RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left - RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN3;
				break;
				case SPIN3:
					trshapeRect[0].top =trshapeRect[0].top - RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left - RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;
					
					trshapeRect[2].top =trshapeRect[2].top + RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left + RECTW;
					trshapeRect[2].right=trshapeRect[2].left+ RECTW;

					trshapeRect[3].top =trshapeRect[3].top + RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left - RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN4;
				break;
				case SPIN4:
					trshapeRect[0].top =trshapeRect[0].top + RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left - RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top - RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left + RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top + RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left + RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN1;
				break;
			}
		break;
		case 1:
			switch(trShapeState)
			{
				case SPIN1:
					trshapeRect[0].top =trshapeRect[0].top + RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left - RECTW;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top - RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top - 2*RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left - RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN2;
				break;
				case SPIN2:
					trshapeRect[0].top =trshapeRect[0].top ;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left + 2*RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top + RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left + RECTW;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top + RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left - RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN3;
				break;
				case SPIN3:
					trshapeRect[0].top =trshapeRect[0].top - 2*RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left - RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top - RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left- RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top + RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN4;
				break;
				case SPIN4:
					trshapeRect[0].top =trshapeRect[0].top + RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left - RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top + RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left + RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left + 2*RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN1;
				break;
			}
		break;
		case 2:
			switch(trShapeState)
			{
				case SPIN1:
					trshapeRect[0].top =trshapeRect[0].top;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left+RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top -RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top ;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left - RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top - RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left - 2*RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN2;
				break;
				case SPIN2:
					trshapeRect[0].top =trshapeRect[0].top -RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left+RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top + RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left + RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top + 2*RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN3;
				break;
				case SPIN3:
					trshapeRect[0].top =trshapeRect[0].top - RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left - RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top - RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left + RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left + 2*RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN4;
				break;
				case SPIN4:
					trshapeRect[0].top =trshapeRect[0].top + 2*RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left - RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top +RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top ;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left - RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top - RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN1;
				break;
			}
		break;
		case 3:
			switch(trShapeState)
			{
				case SPIN1:
					trshapeRect[0].top =trshapeRect[0].top+ 2*RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left-RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top +RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top ;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left + RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top - RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left + 2*RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN2;
				break;
				case SPIN2:
					trshapeRect[0].top =trshapeRect[0].top - 2*RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left+RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top -RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top ;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left - RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top + RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left - 2*RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN1;
				break;
				/*case SPIN3:
				break;
				case SPIN4:
				break;*/
			}
		break;
		case 4:
			switch(trShapeState)
			{
				case SPIN1:
					trshapeRect[0].top =trshapeRect[0].top+ 2*RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top +RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left - RECTW;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top - RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left + RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN2;
				break;
				case SPIN2:
					trshapeRect[0].top =trshapeRect[0].top;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left+RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top +RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top ;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left - RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top - RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left - 2*RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN3;
				break;
				case SPIN3:
					trshapeRect[0].top =trshapeRect[0].top- RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left+RECTW;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top + RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top + 2*RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left - RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN4;
				break;
				case SPIN4:
					trshapeRect[0].top =trshapeRect[0].top - RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left - RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top - 2*RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left ;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top - RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left + RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left + 2*RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN1;
				break;
			}
		break;
		case 5:
			switch(trShapeState)
			{
				case SPIN1:
					trshapeRect[0].top =trshapeRect[0].top+ RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left - RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top +2*RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top +RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left + RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left + 2*RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN2;
				break;
				case SPIN2:
					trshapeRect[0].top =trshapeRect[0].top+ RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left + RECTW;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top -RECTH;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top - 2*RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left - RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN3;
				break;
				case SPIN3:
					trshapeRect[0].top =trshapeRect[0].top;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left+RECTW;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top -RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;

					trshapeRect[2].top =trshapeRect[2].top ;
					trshapeRect[2].bottom=trshapeRect[2].top + RECTH;
					trshapeRect[2].left=trshapeRect[2].left - RECTW;
					trshapeRect[2].right=trshapeRect[2].left + RECTW;

					trshapeRect[3].top =trshapeRect[3].top + RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left - 2*RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN4;
				break;
				case SPIN4:
					trshapeRect[0].top =trshapeRect[0].top - 2*RECTH;
					trshapeRect[0].bottom=trshapeRect[0].top + RECTH;
					trshapeRect[0].left=trshapeRect[0].left;
					trshapeRect[0].right=trshapeRect[0].left + RECTW;

					trshapeRect[1].top =trshapeRect[1].top -RECTH;
					trshapeRect[1].bottom=trshapeRect[1].top + RECTH;
					trshapeRect[1].left=trshapeRect[1].left - RECTW;
					trshapeRect[1].right=trshapeRect[1].left + RECTW;
					
					trshapeRect[3].top =trshapeRect[3].top + RECTH;
					trshapeRect[3].bottom=trshapeRect[3].top + RECTH;
					trshapeRect[3].left=trshapeRect[3].left + RECTW;
					trshapeRect[3].right=trshapeRect[3].left + RECTW;
					trShapeState=SPIN1;
				break;
			}
		break;
		case 6:
			switch(trShapeState)
			{
				case SPIN1:
				break;
				case SPIN2:
				break;
				case SPIN3:
				break;
				case SPIN4:
				break;
			}
		break;
	}
	
	if(trlowColData[trshapeRect[0].top/RECTH][(trshapeRect[0].left -LEFT_SPACE)/RECTW] ==1 || trlowColData[trshapeRect[1].top/RECTH][(trshapeRect[1].left -LEFT_SPACE)/RECTW]==1 || trlowColData[trshapeRect[2].top/RECTH][(trshapeRect[2].left -LEFT_SPACE)/RECTW]==1 || trlowColData[trshapeRect[3].top/RECTH][(trshapeRect[3].left -LEFT_SPACE)/RECTW]==1)//회전 가능한지 여부 check
	{
		trShapeState=trShapeState1;
		MemCopy(&trshapeRect[0],&tmpRect[0], sizeof(rect)*4);
	}
	
	i=0;
	while(i<4)//left 를 넘엇는지 check
	{
		if(trshapeRect[i].left<LEFT_SPACE)
		{
			int j;
			for(j=0;j<4;j++)
			{
				trshapeRect[j].left = trshapeRect[j].left + RECTW;
				trshapeRect[j].right= trshapeRect[j].left + RECTW;
			}
		}
		i++;
	}
	i=0;
	while(i<4)//right를 넘엇는지 check
	{
//		if(trshapeRect[i].right>(trscreenWidth-LEFT_SPACE))
		if(trshapeRect[i].right>(LEFT_SPACE + (MAX_COL * RECTW)))
		{
			int j;
			for(j=0;j<4;j++)
			{
				trshapeRect[j].left = trshapeRect[j].left - RECTW;
				trshapeRect[j].right= trshapeRect[j].left + RECTW;
			}
		}
		i++;
	}
	
}

static rint32 PrvTetCheckFilledLow(rint32 low)
{
	int i=0;
	while(i<MAX_COL)
	{
		if(trlowColData[low][i] ==0)
		{
			return 0;
		}
		i++;
	}
	return 1;
}

static void PrvTetFillLowWithZero(rint32 low)
{
	MemSet(&trlowColData[low],0, MAX_COL);
}


static void PrvTetOneStepDown(rint32 low)
{
	PrvTetFillLowWithZero(low);
	while(low>=1)
	{
		MemCopy(&trlowColData[low], &trlowColData[low-1],MAX_COL);
		low--;
	}
}

static void PrvTetLineDeleteEffect(WND wnd, rint32 line)
{
	COLORVAL tmp;
	rect rect1;

	rect1.top=line*RECTH;
	rect1.bottom=rect1.top + RECTH;
	rect1.left= LEFT_SPACE;
	rect1.right=rect1.left + (RECTW*MAX_COL);
	tmp = GdiSetBkColor(wnd,COLOR_YELLOW);
	GdiRectangle(wnd,&rect1);
	GdiSetBkColor(wnd,tmp);


}

static void PrvTetDrawBackGroundGaming(WND wnd)
{
	rect rect1;
	rint8 buf_score[6], buf_line[6];
	COLORVAL tmp;

	// trscreenHeight 값이 어디선가 변형됨 -_-
	//if (trscreenHeight != 320) {
	//	trscreenHeight = 320;
	//}

	rect1.bottom=SCRH;
	rect1.left=0;
	rect1.right=LEFT_SPACE;
	rect1.top=0;
	tmp = GdiSetBkColor(wnd,COLOR_RED);
	GdiRectangle(wnd, &rect1);
	
	rect1.bottom=SCRH;
	rect1.left=rect1.right+(RECTW*MAX_COL);
	rect1.right=RIGHT_SPACE+rect1.left;
	rect1.top=0;

	GdiRectangle(wnd, &rect1);

	rect1.bottom=SCRH;
	rect1.left=0;
//	rect1.right=SCRW;
	rect1.right=LEFT_SPACE + RIGHT_SPACE + ( MAX_COL * RECTW );
	rect1.top=SCRH - BOTTOM_SPACE;

	GdiRectangle(wnd, &rect1);

	GdiSetBkColor(wnd,tmp);

	rect1.bottom=SCORE_RECT_BOTTOM;
	rect1.left=SCORE_RECT_X;
	rect1.right=SCORE_RECT_Y;
	rect1.top=SCORE_RECT_TOP;
	GdiRectangle(wnd, &rect1);
	MemSet(buf_score, 0, 6);
	MemSet(buf_line, 0, 6);
	StrIToA(g_trScore, (char*)buf_score);
	StrIToA(g_trLineScore, (char*)buf_line);
	GdiTextOut(wnd, "Score", StrLen("Score"), (ruint16) (SCORE_RECT_X + 5), (ruint16) (SCORE_RECT_TOP + 5) );
	GdiTextOut(wnd, (char*)buf_score, StrLen((char*)buf_score), (ruint16) (SCORE_RECT_X + 5), (ruint16) (SCORE_RECT_TOP + 20) );
	GdiTextOut(wnd, "Line", StrLen("Line"), (ruint16) (SCORE_RECT_X + 5), (ruint16) (SCORE_RECT_TOP + 40) );
	GdiTextOut(wnd, (char*)buf_line, StrLen((char*)buf_line), (ruint16) (SCORE_RECT_X + 5), (ruint16) (SCORE_RECT_TOP + 55) );

	rect1.bottom=NEXT_RECT_BOTTOM;
	rect1.left=NEXT_RECT_X;
	rect1.right=NEXT_RECT_Y;
	rect1.top=NEXT_RECT_TOP;
	GdiRectangle(wnd, &rect1);
	GdiTextOut(wnd, "Next", StrLen("Next"), (ruint16) (NEXT_RECT_X + 5), (ruint16) (NEXT_RECT_TOP + 5) );

}

static bool PrvTetCheckMobiltyForDown()//1이면 움직일수 잇다.
{
	rint32 i=0;
	
	while(i<4) 
	{
		if(trlowColData[(trshapeRect[i].bottom/RECTH)][((trshapeRect[i].left-LEFT_SPACE)/RECTW)] == 1 || trshapeRect[i].bottom >= (SCRH - BOTTOM_SPACE))
		{
			return 0;// not mobility..
		}
		i++;
	}

	return 1;
}

static bool PrvTetCheckMobiltyForLeft()
{
	rint32 i=0;
	while(i<4)
	{
		if(trshapeRect[i].left <= LEFT_SPACE || trlowColData[trshapeRect[i].top/RECTH][((trshapeRect[i].left-LEFT_SPACE)/RECTW)-1] ==1)
		{
			return 0;// not mobility..
		}
		i++;
	}
	return 1;
}

static bool PrvTetCheckMobiltyForRight()
{
	rint32 i=0;
	while(i<4)
	{
		if(trshapeRect[i].right >= LEFT_SPACE + (MAX_COL * RECTW) || trlowColData[trshapeRect[i].top/RECTH][(trshapeRect[i].right-LEFT_SPACE)/RECTW] ==1)
		{
			return 0;// not mobility..
		}
		i++;
	}
	return 1;
}

static void PrvTetIncreaseY()
{
	rint32 i=0;
	while(i<4)
	{
		trshapeRect[i].bottom=trshapeRect[i].bottom+RECTH;
		trshapeRect[i].top=trshapeRect[i].top+RECTH;
		i++;
	}
}

static void PrvTetIncreaseX()
{
	rint32 i=0;
	while(i<4)
	{
		trshapeRect[i].left=trshapeRect[i].left+RECTW;
		trshapeRect[i].right=trshapeRect[i].right+RECTW;
		i++;
	}
}

static void PrvTetDecreaseX()
{
	rint32 i=0;
	while(i<4)
	{
		trshapeRect[i].left=trshapeRect[i].left-RECTW;
		trshapeRect[i].right=trshapeRect[i].right-RECTW;
		i++;
	}
}

static int PrvTetCheckGameEnd()
{
	rint32 i=0;
	while(i<4)
	{
		if(trshapeRect[i].top <= -RECTH && trlowColData[trshapeRect[i].bottom/RECTH][(trshapeRect[i].left-LEFT_SPACE)/RECTW] ==1)
		{
			return 1;//fail game
		}
		i++;
	}
	return 0;
}

static void PrvTetMakeNewRect()
{
	trShapeState=SPIN1;
	if(trtypeShape==0)
	{
		
		trshapeRect[0].top=-RECTH;
		trshapeRect[0].bottom=trshapeRect[0].top+RECTH;
		trshapeRect[0].left=60;
		trshapeRect[0].right=trshapeRect[0].left + RECTW;

		trshapeRect[1].top=-RECTH;
		trshapeRect[1].bottom=trshapeRect[1].top+RECTH;
		trshapeRect[1].left=trshapeRect[0].right;
		trshapeRect[1].right=trshapeRect[1].left + RECTW;

		trshapeRect[2].top=-RECTH;
		trshapeRect[2].bottom=trshapeRect[2].top+RECTH;
		trshapeRect[2].left=trshapeRect[1].right;
		trshapeRect[2].right=trshapeRect[2].left + RECTW;

		trshapeRect[3].top=0;
		trshapeRect[3].bottom=trshapeRect[3].top+RECTH;
		trshapeRect[3].left=trshapeRect[2].left-RECTW;
		trshapeRect[3].right=trshapeRect[3].left + RECTW;	
	}
	else if(trtypeShape==1)
	{
		trshapeRect[0].top=-RECTH;
		trshapeRect[0].bottom=trshapeRect[0].top+RECTH;
		trshapeRect[0].left=60;
		trshapeRect[0].right=trshapeRect[0].left + RECTW;

		trshapeRect[1].top=trshapeRect[0].top;
		trshapeRect[1].bottom=trshapeRect[1].top+RECTH;
		trshapeRect[1].left=trshapeRect[0].left+RECTW;
		trshapeRect[1].right=trshapeRect[1].left + RECTW;

		trshapeRect[2].top=trshapeRect[1].top + RECTH;
		trshapeRect[2].bottom=trshapeRect[2].top+RECTH;
		trshapeRect[2].left=trshapeRect[1].left;
		trshapeRect[2].right=trshapeRect[2].left + RECTW;

		trshapeRect[3].top=trshapeRect[2].top;
		trshapeRect[3].bottom=trshapeRect[3].top+RECTH;
		trshapeRect[3].left=trshapeRect[2].left+RECTW;
		trshapeRect[3].right=trshapeRect[3].left + RECTW;
	}
	else if(trtypeShape==2)
	{
		trshapeRect[0].top=0;
		trshapeRect[0].bottom=trshapeRect[0].top+RECTH;
		trshapeRect[0].left=60;
		trshapeRect[0].right=trshapeRect[0].left + RECTW;

		trshapeRect[1].top=trshapeRect[0].top;
		trshapeRect[1].bottom=trshapeRect[1].top+RECTH;
		trshapeRect[1].left=trshapeRect[0].left+RECTW;
		trshapeRect[1].right=trshapeRect[1].left + RECTW;

		trshapeRect[2].top=trshapeRect[1].top - RECTH;
		trshapeRect[2].bottom=trshapeRect[2].top+RECTH;
		trshapeRect[2].left=trshapeRect[1].left;
		trshapeRect[2].right=trshapeRect[2].left + RECTW;

		trshapeRect[3].top=trshapeRect[2].top;
		trshapeRect[3].bottom=trshapeRect[3].top+RECTH;
		trshapeRect[3].left=trshapeRect[2].left+RECTW;
		trshapeRect[3].right=trshapeRect[3].left + RECTW;
	}
	else if(trtypeShape==3)
	{
		trshapeRect[0].top=-RECTH;
		trshapeRect[0].bottom=trshapeRect[0].top+RECTH;
		trshapeRect[0].left=60;
		trshapeRect[0].right=trshapeRect[0].left + RECTW;

		trshapeRect[1].top=trshapeRect[0].top + RECTH;
		trshapeRect[1].bottom=trshapeRect[1].top+RECTH;
		trshapeRect[1].left=trshapeRect[0].left;
		trshapeRect[1].right=trshapeRect[1].left + RECTW;

		trshapeRect[2].top=trshapeRect[1].top + RECTH;
		trshapeRect[2].bottom=trshapeRect[2].top+RECTH;
		trshapeRect[2].left=trshapeRect[1].left;
		trshapeRect[2].right=trshapeRect[2].left + RECTW;

		trshapeRect[3].top=trshapeRect[2].top + RECTH;
		trshapeRect[3].bottom=trshapeRect[3].top+RECTH;
		trshapeRect[3].left=trshapeRect[2].left;
		trshapeRect[3].right=trshapeRect[3].left + RECTW;
	}
	else if(trtypeShape==4)
	{
		trshapeRect[0].top=-RECTH;
		trshapeRect[0].bottom=trshapeRect[0].top+RECTH;
		trshapeRect[0].left=60;
		trshapeRect[0].right=trshapeRect[0].left + RECTW;

		trshapeRect[1].top=trshapeRect[0].top;
		trshapeRect[1].bottom=trshapeRect[1].top+RECTH;
		trshapeRect[1].left=trshapeRect[0].left+RECTW;
		trshapeRect[1].right=trshapeRect[1].left + RECTW;

		trshapeRect[2].top=trshapeRect[1].top + RECTH;
		trshapeRect[2].bottom=trshapeRect[2].top+RECTH;
		trshapeRect[2].left=trshapeRect[1].left;
		trshapeRect[2].right=trshapeRect[2].left + RECTW;

		trshapeRect[3].top=trshapeRect[2].top + RECTH;
		trshapeRect[3].bottom=trshapeRect[3].top+RECTH;
		trshapeRect[3].left=trshapeRect[2].left;
		trshapeRect[3].right=trshapeRect[3].left + RECTW;
	}
	else if(trtypeShape==5)
	{
		trshapeRect[0].top=-RECTH;
		trshapeRect[0].bottom=trshapeRect[0].top+RECTH;
		trshapeRect[0].left=60;
		trshapeRect[0].right=trshapeRect[0].left + RECTW;

		trshapeRect[1].top=trshapeRect[0].top;
		trshapeRect[1].bottom=trshapeRect[1].top+RECTH;
		trshapeRect[1].left=trshapeRect[0].left-RECTW;
		trshapeRect[1].right=trshapeRect[1].left + RECTW;

		trshapeRect[2].top=trshapeRect[1].top + RECTH;
		trshapeRect[2].bottom=trshapeRect[2].top+RECTH;
		trshapeRect[2].left=trshapeRect[1].left;
		trshapeRect[2].right=trshapeRect[2].left + RECTW;

		trshapeRect[3].top=trshapeRect[2].top + RECTH;
		trshapeRect[3].bottom=trshapeRect[3].top+RECTH;
		trshapeRect[3].left=trshapeRect[2].left;
		trshapeRect[3].right=trshapeRect[3].left + RECTW;
	}
	else if(trtypeShape==6)
	{
		trshapeRect[0].top=-RECTH;
		trshapeRect[0].bottom=trshapeRect[0].top+RECTH;
		trshapeRect[0].left=60;
		trshapeRect[0].right=trshapeRect[0].left + RECTW;

		trshapeRect[1].top=trshapeRect[0].top;
		trshapeRect[1].bottom=trshapeRect[1].top+RECTH;
		trshapeRect[1].left=trshapeRect[0].left+RECTW;
		trshapeRect[1].right=trshapeRect[1].left + RECTW;

		trshapeRect[2].top=trshapeRect[0].top + RECTH;
		trshapeRect[2].bottom=trshapeRect[2].top+RECTH;
		trshapeRect[2].left=trshapeRect[0].left;
		trshapeRect[2].right=trshapeRect[2].left + RECTW;

		trshapeRect[3].top=trshapeRect[1].top + RECTH;
		trshapeRect[3].bottom=trshapeRect[3].top+RECTH;
		trshapeRect[3].left=trshapeRect[1].left;
		trshapeRect[3].right=trshapeRect[3].left + RECTW;
	}
}

static void PrvTetMakeNewNextRect()
{
	if(g_trtypeNextShape==0)
	{
		
		trshapeNextRect[0].top=NEXT_RECT_TOP + 25;
		trshapeNextRect[0].bottom=trshapeNextRect[0].top+RECTH;
		trshapeNextRect[0].left=NEXT_RECT_X + 2;
		trshapeNextRect[0].right=trshapeNextRect[0].left + RECTW;

		trshapeNextRect[1].top=NEXT_RECT_TOP + 25;
		trshapeNextRect[1].bottom=trshapeNextRect[1].top+RECTH;
		trshapeNextRect[1].left=trshapeNextRect[0].right;
		trshapeNextRect[1].right=trshapeNextRect[1].left + RECTW;

		trshapeNextRect[2].top=NEXT_RECT_TOP + 25;
		trshapeNextRect[2].bottom=trshapeNextRect[2].top+RECTH;
		trshapeNextRect[2].left=trshapeNextRect[1].right;
		trshapeNextRect[2].right=trshapeNextRect[2].left + RECTW;

		trshapeNextRect[3].top=NEXT_RECT_TOP + 25 + RECTH;
		trshapeNextRect[3].bottom=trshapeNextRect[3].top+RECTH;
		trshapeNextRect[3].left=trshapeNextRect[2].left-RECTW;
		trshapeNextRect[3].right=trshapeNextRect[3].left + RECTW;	
	}
	else if(g_trtypeNextShape==1)
	{
		trshapeNextRect[0].top=NEXT_RECT_TOP + 25;
		trshapeNextRect[0].bottom=trshapeNextRect[0].top+RECTH;
		trshapeNextRect[0].left=NEXT_RECT_X + 2;
		trshapeNextRect[0].right=trshapeNextRect[0].left + RECTW;

		trshapeNextRect[1].top=trshapeNextRect[0].top;
		trshapeNextRect[1].bottom=trshapeNextRect[1].top+RECTH;
		trshapeNextRect[1].left=trshapeNextRect[0].left+RECTW;
		trshapeNextRect[1].right=trshapeNextRect[1].left + RECTW;

		trshapeNextRect[2].top=trshapeNextRect[1].top + RECTH;
		trshapeNextRect[2].bottom=trshapeNextRect[2].top+RECTH;
		trshapeNextRect[2].left=trshapeNextRect[1].left;
		trshapeNextRect[2].right=trshapeNextRect[2].left + RECTW;

		trshapeNextRect[3].top=trshapeNextRect[2].top;
		trshapeNextRect[3].bottom=trshapeNextRect[3].top+RECTH;
		trshapeNextRect[3].left=trshapeNextRect[2].left+RECTW;
		trshapeNextRect[3].right=trshapeNextRect[3].left + RECTW;
	}
	else if(g_trtypeNextShape==2)
	{
		trshapeNextRect[0].top=NEXT_RECT_TOP + 25 + RECTH;
		trshapeNextRect[0].bottom=trshapeNextRect[0].top+RECTH;
		trshapeNextRect[0].left=NEXT_RECT_X + 2;
		trshapeNextRect[0].right=trshapeNextRect[0].left + RECTW;

		trshapeNextRect[1].top=trshapeNextRect[0].top;
		trshapeNextRect[1].bottom=trshapeNextRect[1].top+RECTH;
		trshapeNextRect[1].left=trshapeNextRect[0].left+RECTW;
		trshapeNextRect[1].right=trshapeNextRect[1].left + RECTW;

		trshapeNextRect[2].top=trshapeNextRect[1].top - RECTH;
		trshapeNextRect[2].bottom=trshapeNextRect[2].top+RECTH;
		trshapeNextRect[2].left=trshapeNextRect[1].left;
		trshapeNextRect[2].right=trshapeNextRect[2].left + RECTW;

		trshapeNextRect[3].top=trshapeNextRect[2].top;
		trshapeNextRect[3].bottom=trshapeNextRect[3].top+RECTH;
		trshapeNextRect[3].left=trshapeNextRect[2].left+RECTW;
		trshapeNextRect[3].right=trshapeNextRect[3].left + RECTW;
	}
	else if(g_trtypeNextShape==3)
	{
		trshapeNextRect[0].top=NEXT_RECT_TOP + 25;
		trshapeNextRect[0].bottom=trshapeNextRect[0].top+RECTH;
		trshapeNextRect[0].left=NEXT_RECT_X + 2;
		trshapeNextRect[0].right=trshapeNextRect[0].left + RECTW;

		trshapeNextRect[1].top=trshapeNextRect[0].top + RECTH;
		trshapeNextRect[1].bottom=trshapeNextRect[1].top+RECTH;
		trshapeNextRect[1].left=trshapeNextRect[0].left;
		trshapeNextRect[1].right=trshapeNextRect[1].left + RECTW;

		trshapeNextRect[2].top=trshapeNextRect[1].top + RECTH;
		trshapeNextRect[2].bottom=trshapeNextRect[2].top+RECTH;
		trshapeNextRect[2].left=trshapeNextRect[1].left;
		trshapeNextRect[2].right=trshapeNextRect[2].left + RECTW;

		trshapeNextRect[3].top=trshapeNextRect[2].top + RECTH;
		trshapeNextRect[3].bottom=trshapeNextRect[3].top+RECTH;
		trshapeNextRect[3].left=trshapeNextRect[2].left;
		trshapeNextRect[3].right=trshapeNextRect[3].left + RECTW;
	}
	else if(g_trtypeNextShape==4)
	{
		trshapeNextRect[0].top=NEXT_RECT_TOP + 25;
		trshapeNextRect[0].bottom=trshapeNextRect[0].top+RECTH;
		trshapeNextRect[0].left=NEXT_RECT_X + 2;
		trshapeNextRect[0].right=trshapeNextRect[0].left + RECTW;

		trshapeNextRect[1].top=trshapeNextRect[0].top;
		trshapeNextRect[1].bottom=trshapeNextRect[1].top+RECTH;
		trshapeNextRect[1].left=trshapeNextRect[0].left+RECTW;
		trshapeNextRect[1].right=trshapeNextRect[1].left + RECTW;

		trshapeNextRect[2].top=trshapeNextRect[1].top + RECTH;
		trshapeNextRect[2].bottom=trshapeNextRect[2].top+RECTH;
		trshapeNextRect[2].left=trshapeNextRect[1].left;
		trshapeNextRect[2].right=trshapeNextRect[2].left + RECTW;

		trshapeNextRect[3].top=trshapeNextRect[2].top + RECTH;
		trshapeNextRect[3].bottom=trshapeNextRect[3].top+RECTH;
		trshapeNextRect[3].left=trshapeNextRect[2].left;
		trshapeNextRect[3].right=trshapeNextRect[3].left + RECTW;
	}
	else if(g_trtypeNextShape==5)
	{
		trshapeNextRect[0].top=NEXT_RECT_TOP + 25;
		trshapeNextRect[0].bottom=trshapeNextRect[0].top+RECTH;
		trshapeNextRect[0].left=NEXT_RECT_X + 2 + RECTW;
		trshapeNextRect[0].right=trshapeNextRect[0].left + RECTW;

		trshapeNextRect[1].top=trshapeNextRect[0].top;
		trshapeNextRect[1].bottom=trshapeNextRect[1].top+RECTH;
		trshapeNextRect[1].left=trshapeNextRect[0].left-RECTW;
		trshapeNextRect[1].right=trshapeNextRect[1].left + RECTW;

		trshapeNextRect[2].top=trshapeNextRect[1].top + RECTH;
		trshapeNextRect[2].bottom=trshapeNextRect[2].top+RECTH;
		trshapeNextRect[2].left=trshapeNextRect[1].left;
		trshapeNextRect[2].right=trshapeNextRect[2].left + RECTW;

		trshapeNextRect[3].top=trshapeNextRect[2].top + RECTH;
		trshapeNextRect[3].bottom=trshapeNextRect[3].top+RECTH;
		trshapeNextRect[3].left=trshapeNextRect[2].left;
		trshapeNextRect[3].right=trshapeNextRect[3].left + RECTW;
	}
	else if(g_trtypeNextShape==6)
	{
		trshapeNextRect[0].top=NEXT_RECT_TOP + 25;
		trshapeNextRect[0].bottom=trshapeNextRect[0].top+RECTH;
		trshapeNextRect[0].left=NEXT_RECT_X + 2;
		trshapeNextRect[0].right=trshapeNextRect[0].left + RECTW;

		trshapeNextRect[1].top=trshapeNextRect[0].top;
		trshapeNextRect[1].bottom=trshapeNextRect[1].top+RECTH;
		trshapeNextRect[1].left=trshapeNextRect[0].left+RECTW;
		trshapeNextRect[1].right=trshapeNextRect[1].left + RECTW;

		trshapeNextRect[2].top=trshapeNextRect[0].top + RECTH;
		trshapeNextRect[2].bottom=trshapeNextRect[2].top+RECTH;
		trshapeNextRect[2].left=trshapeNextRect[0].left;
		trshapeNextRect[2].right=trshapeNextRect[2].left + RECTW;

		trshapeNextRect[3].top=trshapeNextRect[1].top + RECTH;
		trshapeNextRect[3].bottom=trshapeNextRect[3].top+RECTH;
		trshapeNextRect[3].left=trshapeNextRect[1].left;
		trshapeNextRect[3].right=trshapeNextRect[3].left + RECTW;
	}
}

static void PrvTetInit()
{
	//srand(time(0));
	GdiGetScreenInfo(&trscreenWidth,&trscreenHeight);
	MemSet(trshapeRect,0,sizeof(int)*4*4);
	g_trGameScreen.top = 0;
	g_trGameScreen.bottom = SCRH - BOTTOM_SPACE;
	g_trGameScreen.left = LEFT_SPACE;
	g_trGameScreen.right = g_trGameScreen.left + (MAX_COL * RECTW);
	trtypeShape=UsrRand();
	trtypeShape=trtypeShape%7;
	g_trtypeNextShape = UsrRand() % 7;
	PrvTetMakeNewRect();
	PrvTetMakeNewNextRect();
	trgameState= TETRIS_START;
	trtimeInterVal=1000;//timer가 발생하는 시간 간격..
	trmaxShape=0;
	trlevel =1;
	trfocus=0;
	g_trScore = 0;
	g_trLineScore = 0;
	MemSet(trlowColData,0,MAX_COL*MAX_LOW);

}

static void PrvTetInitDraw(WND wnd)
{
	rect rect1;
	COLORVAL tmp;
	rint8 buf[5];
	MemSet(buf,0,5);
	StrIToA(trlevel, (char*)buf);
	PrvTetDrawBackGroundGaming(wnd);
			
	rect1.bottom=OPTION_RECT_BOTTOM;
	rect1.left=OPTION_RECT_X;
	rect1.right=OPTION_RECT_Y;
	rect1.top=OPTION_RECT_TOP;
	tmp = GdiSetBkColor(wnd,COLOR_LIGHTGREEN|COLOR_TRANSPARENT);
	GdiRectangle(wnd, &rect1);
	GdiSetBkColor(wnd,tmp);
	GdiTextOut(wnd, "Level",StrLen("Level"),(ruint16)(OPTION_RECT_X+30),(ruint16)OPTION_RECT_TOP+30);
	GdiTextOut(wnd, (char*)buf,StrLen((char*)buf),(ruint16)(OPTION_RECT_X+80),(ruint16)(OPTION_RECT_TOP+30));
	GdiTextOut(wnd, "Start",StrLen("Start"),(ruint16)(OPTION_RECT_X+30),(ruint16)(OPTION_RECT_TOP+60));
	GdiTextOut(wnd, ">",StrLen(">"),(ruint16)(OPTION_RECT_X+15),(ruint16)(OPTION_RECT_TOP+30+(trfocus*30)));

}
static void PrvTetDraw(WND wnd)
{
	GdiLockScreen(wnd);
//	GdiClrScreen(wnd, COLOR_WHITE);
	GdiFillRect(wnd, &g_trGameScreen, COLOR_WHITE);	
	if(trgameState == TETRIS_START)
	{
		PrvTetInitDraw(wnd);
	}
	else if(trgameState == TETRIS_MILESTONE_NEXT_GAME)
	{
		rect rect1;
		COLORVAL tmp;
			
		PrvTetDrawBackGroundGaming(wnd);
			
		rect1.bottom=OPTION_RECT_BOTTOM;
		rect1.left=OPTION_RECT_X;
		rect1.right=OPTION_RECT_Y;
		rect1.top=OPTION_RECT_TOP;
		tmp = GdiSetBkColor(wnd,COLOR_LIGHTGREEN|COLOR_TRANSPARENT);
		GdiRectangle(wnd, &rect1);
		GdiSetBkColor(wnd,tmp);

		if(trtimeInterVal>0)
		{
			GdiTextOut(wnd, "continue",StrLen("continue"),(ruint16)(OPTION_RECT_X+30),(ruint16)(OPTION_RECT_TOP+30));
			GdiTextOut(wnd, "Quit",StrLen("quit"),(ruint16)(OPTION_RECT_X+30),(ruint16)(OPTION_RECT_TOP+60));
			GdiTextOut(wnd, ">",StrLen(">"),(ruint16)(OPTION_RECT_X+15),(ruint16)(OPTION_RECT_TOP+30+(trfocus*30)));
		}
		else
		{
			trfocus =1;
			GdiTextOut(wnd, "you win all the game",StrLen("you win all the game"),(ruint16)(OPTION_RECT_X+30),(ruint16)(OPTION_RECT_TOP+30));
			GdiTextOut(wnd, "Quit",StrLen("Quit"),(ruint16)(OPTION_RECT_X+30),(ruint16)(OPTION_RECT_TOP+60));
			GdiTextOut(wnd, ">",StrLen(">"),(ruint16)(OPTION_RECT_X+15),(ruint16)(OPTION_RECT_TOP+30+(trfocus*30)));
		}
	}
	else if(trgameState == TETRIS_FAIL_MESSAGE)
	{
		rect rect1;
		COLORVAL tmp;
		PrvTetDrawBackGroundGaming(wnd);
		rect1.bottom=OPTION_RECT_BOTTOM;
		rect1.left=OPTION_RECT_X;
		rect1.right=OPTION_RECT_Y;
		rect1.top=OPTION_RECT_TOP;
		tmp = GdiSetBkColor(wnd,COLOR_LIGHTGREEN|COLOR_TRANSPARENT);
		GdiRectangle(wnd, &rect1);
		GdiSetBkColor(wnd,tmp);
		GdiTextOut(wnd, "try again",StrLen("try again"),(ruint16)(OPTION_RECT_X+30),(ruint16)(OPTION_RECT_TOP+30));
		GdiTextOut(wnd, "Quit",StrLen("quit"),(ruint16)(OPTION_RECT_X+30),(ruint16)(OPTION_RECT_TOP+60));
		GdiTextOut(wnd, ">",StrLen(">"),(ruint16)(OPTION_RECT_X+15),(ruint16)(OPTION_RECT_TOP+30+(trfocus*30)));
	}
	else if(trgameState == TETRIS_GAMING)
	{
		PrvTetDrawBackGroundGaming(wnd);
		PrvTetDrawWholeShape(wnd);
	    PrvTetDrawShape(wnd);
		PrvTetDrawNextShape(wnd);
	}
	GdiUnlockScreen(wnd);
}

static void PrvTetBlockDown(WND wnd)
{
	rint32 i=0; 
	rint32 line_del = 0;
	//top and left를 이용..
	while(i<4)
	{
		if (trshapeRect[i].top/RECTH >=0 && (trshapeRect[i].left-LEFT_SPACE)/RECTW >=0 )
			trlowColData[(trshapeRect[i].top/RECTH)][((trshapeRect[i].left-LEFT_SPACE)/RECTW)]=1;
		i++;
	}
	//여기서 full인지 판단하고 처리해야 한다.
	i=0;
	while(i<MAX_LOW)
	{
		if(PrvTetCheckFilledLow(i)!=0)
		{
			PrvTetLineDeleteEffect(wnd, i);
			PrvTetOneStepDown(i);
			trmaxShape++;
			line_del++;
		}
		i++;
	}
	//if (i>0) Sleep(100);
	g_trLineScore += line_del;
	switch(line_del)
	{
	case 0:
		break;
	case 1:
		g_trScore += 100;
		break;
	case 2:
		g_trScore += 200;
		break;
	case 3:
		g_trScore += 300;
		break;
	case 4:
		g_trScore += 500;
		break;
	}
	//여기서는 새로운 모양 setting
	g_trScore += 10;

	trtypeShape = g_trtypeNextShape;
	g_trtypeNextShape = UsrRand() % 7;
						

	PrvTetMakeNewRect();
	PrvTetMakeNewNextRect();
}

static void PrvTetInitHandler(WND wnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	GdiClrScreen(wnd, COLOR_WHITE);
	PrvTetInit();
	PrvTetInitDraw(wnd);
}

static void PrvTetDrawHandler(WND wnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	PrvTetDraw(wnd);
}

static void PrvTetTimerHandler(WND wnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	if(wParam == TETRIS_TIMER)
	{
		if(trgameState == TETRIS_START)
		{
		}
		else if(trgameState == TETRIS_MILESTONE_NEXT_GAME)
		{
		}
		else if(trgameState == TETRIS_FAIL_MESSAGE)
		{
		}
		else if(trgameState == TETRIS_GAMING)
		{
			if(PrvTetCheckGameEnd())//if fail game
			{
				trfocus =0;
				trmaxShape=0;
				trgameState = TETRIS_FAIL_MESSAGE;
				UsrKillTimer(wnd, TETRIS_TIMER);
				PrvTetDraw(wnd);
			}
			else if(trmaxShape>=WIN_GAME_COUNT)//if win game
			{
				trgameState = TETRIS_MILESTONE_NEXT_GAME;
				if (trtimeInterVal >= 200) trtimeInterVal=trtimeInterVal-100;
				trfocus=0;
				trmaxShape=0;
				UsrKillTimer(wnd, TETRIS_TIMER);
				PrvTetDraw(wnd);
			}
			else
			{
				if(PrvTetCheckMobiltyForDown()==1)
				{
					PrvTetIncreaseY();
				}
				else
				{
					PrvTetBlockDown(wnd);
				}
			}
		}
		PrvTetDraw(wnd);
	}
}

static void PrvTetKeyDownHandler(WND wnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	switch(wParam) 
	{
		case VK_PAD_0:
			if(trgameState == TETRIS_START)
			{
			}
			else if(trgameState == TETRIS_MILESTONE_NEXT_GAME)
			{
			}
			else if(trgameState == TETRIS_FAIL_MESSAGE)
			{
			}
			else if(trgameState == TETRIS_GAMING)
			{
			}
			break;
		case VK_USER_OK:
			if(trgameState ==TETRIS_START)
			{
				if(trfocus ==1)
				{
					trtimeInterVal = trtimeInterVal - 200*(trlevel-1);
					trgameState = TETRIS_GAMING;
					UsrSetTimer(wnd, TETRIS_TIMER, (rint32)trtimeInterVal);
					PrvTetDraw(wnd);
				}
			}
			else if(trgameState ==TETRIS_MILESTONE_NEXT_GAME)
			{
				if(trfocus == 0)
				{
					//MemSet(trlowColData,0,MAX_COL*MAX_LOW);
					trgameState = TETRIS_GAMING;
					UsrSetTimer(wnd, TETRIS_TIMER, (rint32)trtimeInterVal);
					PrvTetDraw(wnd);
				}
				else if(trfocus==1)
				{
					StaGotoState(wnd, STATE_ID_ENTRY, MSG_DESTROY, MSG_CREATE );
				}
			}
			else if(trgameState == TETRIS_FAIL_MESSAGE)
			{
				if(trfocus == 0)
				{
					MemSet(trlowColData,0,MAX_COL*MAX_LOW);
					trgameState = TETRIS_GAMING;
					trmaxShape=0;
					UsrSetTimer(wnd, TETRIS_TIMER, (rint32)trtimeInterVal);
					PrvTetDraw(wnd);
					g_trScore = 0;
					g_trLineScore = 0;

				}
				else if(trfocus==1)
				{
					//UsrKillTimer(wnd, TETRIS_TIMER);
					StaGotoState(wnd, STATE_ID_ENTRY, MSG_DESTROY, MSG_CREATE );
				}
			}
			else if(trgameState == TETRIS_GAMING)
			{

			}
			break;
			
		case VK_USER_CLR:
			break;
		case VK_USER_DOWN:
			if(trgameState == TETRIS_START)
			{
				if(trfocus ==0)
				{
					trfocus = 1;
				}
				else
				{
					trfocus =0;
				}
			}
			else if(trgameState == TETRIS_MILESTONE_NEXT_GAME  && trtimeInterVal>0)
			{
				if(trfocus ==0)
				{
					trfocus = 1;
				}
				else
				{
					trfocus =0;
				}
			}
			else if(trgameState == TETRIS_FAIL_MESSAGE)
			{
				if(trfocus ==0)
				{
					trfocus = 1;
				}
				else
				{
					trfocus =0;
				}
			}
			else if(trgameState == TETRIS_GAMING)
			{
				if(PrvTetCheckMobiltyForDown()==1)
				{
					PrvTetIncreaseY();
				}
				// Add
				else 
				{
					PrvTetBlockDown(wnd);
					if(trmaxShape>=WIN_GAME_COUNT)//if win game
					{
						trgameState = TETRIS_MILESTONE_NEXT_GAME;
						if (trtimeInterVal >= 200) trtimeInterVal=trtimeInterVal-100;
						trfocus=0;
						trmaxShape=0;
						UsrKillTimer(wnd, TETRIS_TIMER);
						//PrvTetDraw(wnd);
					}

				}
			}
			PrvTetDraw(wnd);
			break;

		case VK_USER_UP://회전 용 key
			if(trgameState == TETRIS_START)
			{
				if(trfocus ==0)
				{
					trfocus = 1;
				}
				else
				{
					trfocus =0;
				}
			}
			else if(trgameState == TETRIS_MILESTONE_NEXT_GAME && trtimeInterVal>0)
			{
				if(trfocus ==0)
				{
					trfocus = 1;
				}
				else
				{
					trfocus =0;
				}
			}
			else if(trgameState == TETRIS_FAIL_MESSAGE)
			{
				if(trfocus ==0)
				{
					trfocus = 1;
				}
				else
				{
					trfocus =0;
				}
			}
			else if(trgameState == TETRIS_GAMING)
			{
				PrvTetChangePosition();
			}
			PrvTetDraw(wnd);
			break;
		case VK_USER_LEFT:
			if(trgameState == TETRIS_START)
			{
				if(trfocus ==0)
				{
					if(trlevel >1)
					{
						trlevel--;
					}
				}
			}
			else if(trgameState == TETRIS_MILESTONE_NEXT_GAME)
			{
			}
			else if(trgameState == TETRIS_FAIL_MESSAGE)
			{
			}
			else if(trgameState == TETRIS_GAMING)
			{
				if(PrvTetCheckMobiltyForLeft())
				{
					PrvTetDecreaseX();
				}
				else
				{
					//do nothing
				}
			}
			PrvTetDraw(wnd);
			break;
		case VK_USER_RIGHT:
		    if(trgameState == TETRIS_START)
			{
				if(trfocus ==0)
				{
					if(trlevel <4)
					{
						trlevel++;
					}
				}
			}
			else if(trgameState == TETRIS_MILESTONE_NEXT_GAME)
			{
			}
			else if(trgameState == TETRIS_FAIL_MESSAGE)
			{
			}
			else if(trgameState == TETRIS_GAMING)
			{
				if(PrvTetCheckMobiltyForRight())
				{
					PrvTetIncreaseX();
				}
				else
				{
					//do nothing
				}
			}
			PrvTetDraw(wnd);
			break;
	}
}


rint32 PrvTetProc(WND wnd, rint32 msg, rint32 wparam, rint32 lparam)
{
	rint32 i;
	for (i=0; i<4; i++)
		if (msg == g_TetHandlerList[i].eStateId) g_TetHandlerList[i].pfnStateHandler(wnd, msg, wparam, lparam);

	return 0;
}

int	TetrisProc( WND wnd, int msg, int wparam, int lparam )
{
	static WND TetrisMainWindow;
	int	ScreenWidth, ScreenHeight;
	int bHandled = FALSE;

	switch ( msg )
	{
		case AWM_CREATE:

			// Create Window for this application
			GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); // We are going to use whole screen
			TetrisMainWindow= WinCreateWindow( PrvTetProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL );

			// Do something for your application's initialization

			bHandled = TRUE;
			break;
			
		case AWM_DESTROY:
			
			// Do something to close this app
			
			// Close all window that are created by this application
			WinDestroyWindow(TetrisMainWindow);
			bHandled = TRUE;	// End this application
			break;
	}

	return bHandled;
}
