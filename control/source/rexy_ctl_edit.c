/* 
* $Id: REXY_CTL_EDIT.c 491 2006-01-11 00:26:34Z madholic $
* Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy Embedded
*   
* This file defines the functions and logics of Edit Control.
* For function naming rules follow below.
* Exported : "Ctl" + "(ControlName)" + "(Verb)" {+ "(Object)" : Optional}
* Private : "PrvCtlEdit" + "(Verb)" + "(Object)"
*/

/** @file */

/**
 * @addtogroup EditCtlGroup Ctrl Edit Control
 * Edit Control
 * @{
 */
#include "rexy_ctl_edit.h"

//#define TRACESELECT						TR_RexyControl
//#define TRACELEVEL						TR_LEVEL4

//
//
//
typedef struct _REXY_CTL_EDIT_CARET
{
	WND			hWnd;				///< 캐럿이 나타날 윈도의 핸들
	int			nX, nY;				///< 캐럿의 x,y 좌표
	int			nWidth, nHeight;	///< 캐럿의 폭과 높이
	int			nElapse;			///< 캐럿의 깜박임 속도
	bool		bVisible;			///< 캐럿의 표시 여부
	bool		bDrawed;			///< 캐럿의 화면 표시 상태
	bool		bCreated;			///< 캐럿의 생성 상태
} REXY_CTL_EDIT_CARET;

// CtlEditCaret Related Functions
int CtlEditCaretCreate(REXY_CTL_EDIT_CARET *pCaret, WND hWnd, int nWidth, int nHeight, int nElapse);
int CtlEditCaretDraw(REXY_CTL_EDIT_CARET *pCaret);
int CtlEditCaretShow(REXY_CTL_EDIT_CARET *pCaret);
int CtlEditCaretHide(REXY_CTL_EDIT_CARET *pCaret);
int CtlEditCaretSetPos(REXY_CTL_EDIT_CARET *pCaret, int nX, int nY);
int CtlEditCaretDestroy(REXY_CTL_EDIT_CARET *pCaret);

/**
 * @brief		캐럿을 생성한다.
 * @param[in]	pCaret	캐럿 정보 구조체에 대한 포인터
 * @param[in]	hWnd	캐럿이 표시될 윈도 핸들
 * @param[in]	nWidth	
 * @param[in]	nHeight	
 * @param[in]	nElapse	
 * @retval		0		오류 발생
 *				1		작업 성공
 * @if HISTORY
 *				13-Jun-2006	yich Initiated
 * @endif
 */ 
int CtlEditCaretCreate(REXY_CTL_EDIT_CARET *pCaret, WND hWnd, int nWidth, int nHeight, int nElapse)
{
	TRACE(TR_RexyControl, TR_LEVEL3, 
		("CtlEditCaretCreate(pCaret=0x%x, hWnd=0x%x, nWidth=%d, nHeight=%d, nElapse=%d) Called\n", pCaret, hWnd, nWidth, nHeight, nElapse));

	if (!pCaret)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretCreate() Return %d\n", 0));
		return 0;
	}

	if (pCaret->bCreated)
	{
		CtlEditCaretDestroy(pCaret);
	}

	pCaret->hWnd     = hWnd;
	pCaret->bVisible = FALSE;
	pCaret->nX = 0;
	pCaret->nY = 0;
	pCaret->nWidth = nWidth;
	pCaret->nHeight = nHeight;
	pCaret->nElapse = nElapse;
	pCaret->bDrawed = FALSE;
	pCaret->bCreated = TRUE;

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretCreate() Return %d\n", 1));
	return 1;
}

/**
 * @brief		캐럿을 화면에 그린다.
 * @param[in]	pCaret	캐럿 정보 구조체에 대한 포인터
 * @retval		0		오류 발생
 *				1		작업 성공
 * @if HISTORY
 *				13-Jun-2006	yich Initiated
 * @endif
 */ 
int CtlEditCaretDraw(REXY_CTL_EDIT_CARET *pCaret)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretDraw(pCaret=0x%x) Called\n", pCaret));

	if (!pCaret)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretDraw() Return %d\n", 0));
		return 0;
	}

	TRACE(TR_RexyControl, TR_LEVEL2, ("\tBefore Draw - (X=%3d, Y=%3d, D=%02x)\n", pCaret->nX, pCaret->nY, pCaret->bDrawed));

	if (pCaret->bVisible == FALSE)
	{
		TRACE(TR_RexyControl, TR_LEVEL2, ("\tNot Visible\n"));
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretDraw() Return %d\n", 0));
		return 0;
	}

	{
		rect	r;

		r.left	 = pCaret->nX;
		r.right  = pCaret->nX + pCaret->nWidth - 1;
		r.top	 = pCaret->nY;
		r.bottom = pCaret->nY + pCaret->nHeight - 1;
		GdiLockScreen(pCaret->hWnd);
		GdiInvertRect(pCaret->hWnd, &r);
		GdiUnlockScreen(pCaret->hWnd);

		pCaret->bDrawed = ~pCaret->bDrawed;
	}

	TRACE(TR_RexyControl, TR_LEVEL2, ("\tAfter Draw - (X=%3d, Y=%3d, D=%02x)\n", pCaret->nX, pCaret->nY, pCaret->bDrawed));
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretDraw() Return %d\n", 1));
	return 1;
}

/**
 * @brief		캐럿을 화면에 표시한다.
 * @param[in]	pCaret	캐럿 정보 구조체에 대한 포인터
 * @retval		0		오류 발생
 *				1		작업 성공
 * @if HISTORY
 *				13-Jun-2006	yich Initiated
 * @endif
 */ 
int CtlEditCaretShow(REXY_CTL_EDIT_CARET *pCaret)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretShow(pCaret=0x%x) Called\n", pCaret));

	if (!pCaret)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretShow() Return %d\n", 0));
		return 0;
	}

	pCaret->bVisible = TRUE;
	CtlEditCaretDraw(pCaret);
	UsrSetTimer(pCaret->hWnd, REXY_CTL_EDIT_CARET_TIMER, pCaret->nElapse);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretShow() Return %d\n", 1));
	return 1;
}

/**
 * @brief		캐럿을 화면에 지운다.
 * @param[in]	pCaret	캐럿 정보 구조체에 대한 포인터
 * @retval		0		오류 발생
 *				1		작업 성공
 * @if HISTORY
 *				13-Jun-2006	yich Initiated
 * @endif
 */ 
int CtlEditCaretHide(REXY_CTL_EDIT_CARET *pCaret)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretHide(pCaret=0x%x) Called\n", pCaret));

	if (!pCaret)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretHide() Return %d\n", 0));
		return 0;
	}

	if (pCaret->bDrawed)
	{
		CtlEditCaretDraw(pCaret);
	}
	pCaret->bVisible = FALSE;
	pCaret->bDrawed = FALSE;
	UsrKillTimer(pCaret->hWnd, REXY_CTL_EDIT_CARET_TIMER);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretHide() Return %d\n", 1));
	return 1;
}

/**
 * @brief		캐럿을 위치를 변경한다.
 * @param[in]	pCaret	캐럿 정보 구조체에 대한 포인터
 * @param[in]	nX		캐럿의 새 X 좌표
 * @param[in]	nY		캐럿의 새 Y 좌표
 * @retval		0		오류 발생
 *				1		작업 성공
 * @if HISTORY
 *				13-Jun-2006	yich Initiated
 * @endif
 */ 
int CtlEditCaretSetPos(REXY_CTL_EDIT_CARET *pCaret, int nX, int nY)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretSetPos(pCaret=0x%x, nX=%d, nY=%d) Called\n", pCaret, nX, nY));

	if (!pCaret)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretSetPos() Return %d\n", 0));
		return 0;
	}

	if (pCaret->bDrawed)
	{
		CtlEditCaretDraw(pCaret);
	}

	pCaret->nX = nX;
	pCaret->nY = nY;
	CtlEditCaretDraw(pCaret);
	UsrSetTimer(pCaret->hWnd, REXY_CTL_EDIT_CARET_TIMER, pCaret->nElapse);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretSetPos() Return %d\n", 0));
	return 0;
}

/**
 * @brief		캐럿을 제거한다.
 * @param[in]	pCaret	캐럿 정보 구조체에 대한 포인터
 * @retval		0		오류 발생
 *				1		작업 성공
 * @if HISTORY
 *				13-Jun-2006	yich Initiated
 * @endif
 */ 
int CtlEditCaretDestroy(REXY_CTL_EDIT_CARET	*pCaret)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretDestroy(pCaret=0x%x) Called\n", pCaret));

	if (!pCaret)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretSetPos() Return %d\n", 0));
		return 0;
	}

	CtlEditCaretHide(pCaret);
	UsrKillTimer(pCaret->hWnd, REXY_CTL_EDIT_CARET_TIMER);
	
	pCaret->bVisible = FALSE;

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCaretSetPos() Return %d\n", 1));
	return 1;
}

//
//
//
//
//
#define CTL_EDIT_BUFFER_MAXSIZE			1024
#define CTL_EDIT_DELAY_CLEAR			3000

#define CTL_EDIT_GETMEMSIZE(n)			((n) + (8 - (n) % 8))

#define CTL_EDIT_STATUS					0x0F
#define CTL_EDIT_STATUS_NORMAL			0x00
#define CTL_EDIT_STATUS_FOCUS			0x01
#define CTL_EDIT_STATUS_HILITE			0x02

#define CTL_EDIT_FLAG					0xF0
#define CTL_EDIT_FLAG_NORMAL			0x00
#define CTL_EDIT_FLAG_RETURN			0x10

#define CTL_EDIT_COLOR_BORDER			MAKERGB(29, 79, 132)
#define CTL_EDIT_COLOR_BORDER_HILITE	MAKERGB(212, 96, 1)
#define CTL_EDIT_COLOR_BORDER_FOCUS		MAKERGB(73, 6, 0)

#define CTL_EDIT_COLOR_BORDER_IN		MAKERGB(120, 181, 168)
#define CTL_EDIT_COLOR_BORDER_IN_HILITE	MAKERGB(254, 190,17)
#define CTL_EDIT_COLOR_BORDER_IN_FOCUS	MAKERGB(177, 17, 1)

#define CTL_EDIT_COLOR_WINDOW			MAKERGB(199, 246, 236)

#define CTL_EDIT_COLOR_TEXT				MAKERGB(0, 0, 0)

#define CTL_EDIT_MARGIN					3

#define CTL_EDIT_POS_CARET				1
#define CTL_EDIT_POS_TEXT				2
#define CTL_EDIT_POS_HEIGHT				3

#define CTL_EDIT_TIMER_CLEAR			1

#define CTL_EDIT_STRING_PASSWORD		"***********************"

#define CTL_EDIT_CARET_TIMER_E			1000

/**
 * @struct Edit Control을 위한 내부 자료 구조
 */
typedef struct _REXY_CTL_EDIT
{
	bool					bEnabled;							///< 컨트롤 활성화 여부 (미사용)
	bool					bVisible;							///< 컨트롤 표시 여부(미사용)

	rect					sWndRect;							///< 컨트롤 윈도의 크기
	int						nRectWidth, nRectHeight;			///<

	char					pString[CTL_EDIT_BUFFER_MAXSIZE];	///< 문자열 저장 공간
	int						nLength;							///< 현재 저장된 문자열 길이
	int						nMaxChars;							///< 최대 문자열 길이

	bool					bMultiLine;							///< Single/Multi Line 여부
	bool					bPassword;							///< Password 입력 모드
	bool					bReadOnly;							///< 읽기 전용 모드 여부

	REXY_CTL_EDIT_CARET		sCaret;								///< 캐럿 정보에 대한 구조체
	int						nCaretX, nCaretY;					///< 캐럿의 X, Y 좌표
	int						nPos;								///< 캐럿의 문자열 내의 위치
	int						nStartLine, nStartX;				///<

	int						nMaxLines;							///< 최대 줄수
	int						nPreTotalLines;
	int						nPrvKeyCode;						///<
	
	CtlEditCBDraw			pfnCBDraw;							///< 그리기 콜백 함수 포인터
	CtlEditCBKeyDown		pfnCBKeyDown;						///< 키 입력 처리를 위한 콜백 함수 포인터

    void*					pData;								///< Edit 컨트롤와 연관된 자료 저장을 위한 포인터, 추후 확장 용도로 사용
} REXY_CTL_EDIT;

////////////////////////////////////////////////////////////
// Internal Functions
//
static REXY_CTL_EDIT* PrvCtlEditGetDataP(WND hCtlWnd, int *pErr);
static void PrvCtlEditSetWnd(WND hCtlWnd, int *pErr);

static int PrvCtlEditInitCaret(WND hCtlWnd);
static int PrvCtlEditGetCaretPos(WND hCtlWnd, int nPos, int *pX, int *pY, int *pX2);
static int PrvCtlEditUpdateCaret(WND hCtlWnd);

static int PrvCtlEditMovePos(WND hCtlWnd, int nKeyCode);
static int PrvCtlEditGetTextRect(WND hCtlWnd, rect *pTextRect);
static int PrvCtlEditGetLineHeight(void);
static int PrvCtlEditGetTextPos(WND hCtlWnd, int nX, int nY, int *pPos);
static int PrvCtlEditUpdate(WND hCtlWnd);
static int PrvCtlEditDraw(WND hCtlWnd, rect *pClientRect);
static int PrvCtlEditInsertChars(WND hCtlWnd, char *pChars, int nCharsLen);
static int PrvCtlEditPostProcess(WND hCtlWnd);
static int PrvCtlEditDeleteChars(WND hCtlWnd, int nBackDelete);
static int PrvCtlEditCheckLeadByte(char *pszLine, int nPos);
static int PrvCtlEditGetPosition(WND hCtlWnd, int *pPos, int *pX, int *pY, int *pX2, int nWhat);
static int PrvCtlEditDrawScrollBar(WND hCtlWnd, int nTotalWidth );	//added by wichang

/** 
 * @brief        Get data pointer.
 * @param[in]    hCtlWnd         Pointer of control to retrieve data pointer
 * @param[out]   pErr            Pointer to error value
 * @retval       REXY_CTL_EDIT*  edit control data pointer.
 * @remarks      Control structure has void data pointer member.\n
 *               This function returns speicified control's data pointer.
 * @if HISTORY
 *         13-Jun-2006 yich Initiated
 * @endif
 */
static REXY_CTL_EDIT* PrvCtlEditGetDataP(WND hCtlWnd, int *pErr)
{
    //Will be checked by control core.
    //CHECK_NULL_PTR(hWnd, NULL, *pErr);

	TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlEditGetDataP(hCtlWnd=0x%x, *pErr=%d) Called\n", hCtlWnd, *pErr));
    CHECK_ERR_IF_NOT_EQUAL(hCtlWnd->window_type,  CTL_TYPE_EDIT, NULL);
    
    //Will be checked after PrvCtlEditGetDataP() function call.
    //CHECK_NULLPTR(hWnd->pData, NULL, *pErr);

	TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlEditGetDataP(*pErr=%d) Return 0x%x\n", *pErr, hCtlWnd->pData));
    return (REXY_CTL_EDIT*)hCtlWnd->pData;
}

/** 
* @brief        Set edit control window area.
* @param[in]    hWnd    Pointer to control structure
* @param[out]   pErr    Pointer to error value
* @retval       void
* @remarks      This function calculates window area of edit control by its margin.
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
static void PrvCtlEditSetWnd(WND hCtlWnd, int *pErr)
{
    int nWndWidth;
    int nWndHeight;

    REXY_CTL_EDIT *pCtlEdit;

	TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlEditSetWnd(hCtlWnd=0x%x, *pErr=%d) Called\n", hCtlWnd, *pErr));
    pCtlEdit = PrvCtlEditGetDataP(hCtlWnd, pErr);
    CHECK_NULLPTR_VOID(pCtlEdit, *pErr);

    pCtlEdit->nRectWidth  = nWndWidth = pCtlEdit->sWndRect.right - pCtlEdit->sWndRect.left + 1;
    pCtlEdit->nRectHeight = nWndHeight = pCtlEdit->sWndRect.bottom - pCtlEdit->sWndRect.top + 1;

	CtlSetWNDRect(hCtlWnd, pCtlEdit->sWndRect.left, pCtlEdit->sWndRect.top, nWndWidth, nWndHeight, pErr);

	//pCtlEdit->nMaxLines = nWndHeight / CtlEditGetLineHeight();
	TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlEditSetWnd() return\n"));
}

static int PrvCtlEditMovePos(WND hCtlWnd, int nKeyCode)
{
	rect rcText;
	int x, y;
	int nWidth, nHeight, nLineHeight;

	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	char *pText = (char *)pCtlEdit->pString;
	
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMovePos(hCtlWnd=0x%x, nKeyCode=%d) Called\n", hCtlWnd, nKeyCode));
	if (!pCtlEdit)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMovePos() Return %d\n", 0));
		return 0;
	}

	PrvCtlEditGetTextRect(hCtlWnd, &rcText);
	nWidth = rcText.right - rcText.left + 1;
	nHeight = rcText.bottom - rcText.top + 1;

	nLineHeight = PrvCtlEditGetLineHeight();

	switch (nKeyCode)
	{
		case VK_USER_LEFT:
			if (pCtlEdit->nPos > 0)
			{
				pCtlEdit->nPos--;

				if (pText[pCtlEdit->nPos] & 0x80)
					pCtlEdit->nPos--;
			}

			PrvCtlEditGetCaretPos(hCtlWnd, pCtlEdit->nPos, &pCtlEdit->nCaretX, &pCtlEdit->nCaretY, NULL);
			break;

		case VK_USER_RIGHT:
			if (pCtlEdit->nPos < pCtlEdit->nLength)
			{
				if (pText[pCtlEdit->nPos] & 0x80)
					pCtlEdit->nPos++;
				pCtlEdit->nPos++;

				PrvCtlEditGetCaretPos(hCtlWnd, pCtlEdit->nPos, &pCtlEdit->nCaretX, &pCtlEdit->nCaretY, NULL);
			}
			break;

		case VK_USER_UP:
			if (pCtlEdit->bMultiLine && pCtlEdit->nPos > 0 && pCtlEdit->nCaretY > pCtlEdit->nStartLine)
			{
				x = pCtlEdit->nCaretX;
				y = pCtlEdit->nCaretY - PrvCtlEditGetLineHeight();

				PrvCtlEditGetTextPos(hCtlWnd, x, y, &pCtlEdit->nPos);
				PrvCtlEditGetCaretPos(hCtlWnd, pCtlEdit->nPos, &pCtlEdit->nCaretX, &pCtlEdit->nCaretY, NULL);
			}
			break;

		case VK_USER_DOWN:
			if (pCtlEdit->bMultiLine && pCtlEdit->nPos < pCtlEdit->nLength)
			{
				x = pCtlEdit->nCaretX;
				y = pCtlEdit->nCaretY + PrvCtlEditGetLineHeight();

				PrvCtlEditGetTextPos(hCtlWnd, x, y, &pCtlEdit->nPos);
				PrvCtlEditGetCaretPos(hCtlWnd, pCtlEdit->nPos, &pCtlEdit->nCaretX, &pCtlEdit->nCaretY, NULL);
			}
			break;

		default:
			TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMovePos() Return %d\n", 0));
			return 0;
	}	

	if (pCtlEdit->bMultiLine)
	{
		if ((nKeyCode == VK_USER_LEFT || nKeyCode == VK_USER_UP) &&
			(pCtlEdit->nCaretY < pCtlEdit->nStartLine * nLineHeight))
		{
			pCtlEdit->nStartLine--;
			PrvCtlEditUpdate(hCtlWnd);
		}
		else if ((nKeyCode == VK_USER_RIGHT || nKeyCode == VK_USER_DOWN) &&
			(pCtlEdit->nCaretY + nLineHeight > pCtlEdit->nStartLine * nLineHeight + nHeight))
		{
			pCtlEdit->nStartLine++;
			PrvCtlEditUpdate(hCtlWnd);

			PrvCtlEditGetCaretPos(hCtlWnd, pCtlEdit->nPos, &pCtlEdit->nCaretX, &pCtlEdit->nCaretY, NULL);
		}
	}
	else
	{
		if (nKeyCode == VK_USER_RIGHT && pCtlEdit->nCaretX >= pCtlEdit->nStartX + nWidth)
		{
			pCtlEdit->nStartX += 40;
			PrvCtlEditUpdate(hCtlWnd);

			PrvCtlEditGetCaretPos(hCtlWnd, pCtlEdit->nPos, &pCtlEdit->nCaretX, &pCtlEdit->nCaretY, NULL);
		}
		else if (nKeyCode == VK_USER_LEFT && pCtlEdit->nCaretX < pCtlEdit->nStartX)
		{
			pCtlEdit->nStartX = max(0, pCtlEdit->nStartX - 40);
			PrvCtlEditUpdate(hCtlWnd);

			PrvCtlEditGetCaretPos(hCtlWnd, pCtlEdit->nPos, &pCtlEdit->nCaretX, &pCtlEdit->nCaretY, NULL);
		}
	}

	PrvCtlEditUpdateCaret(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMovePos() Return %d\n", 1));
	return 1;	
}

static int PrvCtlEditMsgChar(WND hCtlWnd, int nCharCode)
{
	char pBuff[2] = {0, };
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgChar(hCtlWnd=0x%x, nCharCode=%d) Called\n", hCtlWnd, nCharCode));

	if (!pCtlEdit)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgChar() Return %d\n", 0));
		return 0;
	}

	if (pCtlEdit->bReadOnly)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgChar() Return %d\n", 0));
		return 0;
	}

	if (nCharCode & 0x80)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgChar() Return %d\n", 0));
		return 0;
	}

	if (nCharCode != 0x0D && nCharCode < 32)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgChar() Return %d\n", 0));
		return 0;
	}

	if (pCtlEdit->nMaxChars > 0 && pCtlEdit->nLength >= pCtlEdit->nMaxChars)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgChar() Return %d\n", 0));
		return 0;
	}

	if (nCharCode == 0x0D)
		nCharCode = 0x0A;

	pBuff[0] = (char) nCharCode;
	PrvCtlEditInsertChars(hCtlWnd, pBuff, 1);

	PrvCtlEditPostProcess(hCtlWnd);
	PrvCtlEditUpdate(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgChar() Return %d\n", 1));
	return 1;
}

//static int PrvCtlEditMsgGetText(WND hCtlWnd, int nMaxCnt, char *pText)
//{
//	int nLength = 0;
//	char *pCurrText;
//	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);
//
//	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgGetText(hCtlWnd=0x%x, nMaxCnt=%d, pText=0x%x) Called\n", hCtlWnd, nMaxCnt, pText));
//	if (!pCtlEdit)
//	{
//		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgGetText() Return %d\n", -1));
//		return -1;
//	}
//
//	if (!pText || nMaxCnt <= 0)
//	{
//		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgGetText() Return %d\n", -1));
//		return -1;
//	}
//
//	pCurrText = pCtlEdit->pString;
//
//	if (pCurrText)
//		nLength = StrLen(pCurrText);
//
//	MemSet(pText, 0, nMaxCnt);
//	if (nLength > 0)
//	{
//		if (nLength >= nMaxCnt)
//			nLength = nMaxCnt - 1;
//
//		nLength = min(nLength, nMaxCnt);
//		MemCopy(pText, pCurrText, nLength);
//
//		pText[nLength] = '\0';
//	}
//
//	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgGetText() Return %d\n", nLength));
//	return nLength;
//}

static int PrvCtlEditMsgSetText(WND hCtlWnd, char *pText)
{
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgSetText(hCtlWnd=0x%x, pText=0x%x) Called\n", hCtlWnd, pText));
	if (!pCtlEdit || !pText)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgSetText() Return %d\n", 0));
		return 0;
	}

	pCtlEdit->nLength = StrLen(pText);

	if (pCtlEdit->nLength > 0)
		StrCpy(pCtlEdit->pString, pText);
	
	pCtlEdit->nPos = 0;

	PrvCtlEditInitCaret(hCtlWnd);

	PrvCtlEditUpdate(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgSetText() Return %d\n", pCtlEdit->nLength));
	return pCtlEdit->nLength;
}

//static int PrvCtlEditMsgLimitText(WND hCtlWnd, int nMax)
//{
//	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);
//
//	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgLimitText(hCtlWnd=0x%x, nMax=%d) Called\n", hCtlWnd, nMax));
//	if (!pCtlEdit)
//	{
//		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgLimitText() Return %d\n", 0));
//		return 0;
//	}
//
//	pCtlEdit->nMaxChars = max(nMax, pCtlEdit->nLength);
//	
//	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgLimitText() Return %d\n", 0));
//	return 1;
//}

static int PrvCtlEditPostProcess(WND hCtlWnd)
{
	rect rcText;
	int x, y, x2;
	int nWidth, nHeight, nNewHeight, nLine;
	int nReturnFlag, nLineHeight, nStartHeight;
	char *pCurrText;
	int nErr;

	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditPostProcess(hCtlWnd=0x%x) Called\n", hCtlWnd));
	
	CHECK_NULLPTR(pCtlEdit, 0, nErr);

	pCurrText = pCtlEdit->pString;

	PrvCtlEditGetTextRect(hCtlWnd, &rcText);

	x = 0;
	y = 0;

	nWidth = rcText.right - rcText.left + 1;
	nHeight = rcText.bottom - rcText.top + 1;

	PrvCtlEditGetCaretPos(hCtlWnd, pCtlEdit->nPos, &x, &y, &x2);

	nNewHeight = nHeight;
	nLineHeight = PrvCtlEditGetLineHeight();

	if (pCtlEdit->bMultiLine)
	{
		PrvCtlEditGetPosition(hCtlWnd, NULL, NULL, &nNewHeight, NULL, CTL_EDIT_POS_HEIGHT);
		nLine = nNewHeight / nLineHeight;

		{
			nReturnFlag = 0;
			if (x == 0 && pCtlEdit->nPos > 0)
			{
				nReturnFlag = pCurrText[pCtlEdit->nPos - 1] == 0x0A;
				if (!nReturnFlag)
				{
					y -= nLineHeight;
					x = x2;
				}
			}

			nStartHeight = pCtlEdit->nStartLine * nLineHeight;
			if (y + nLineHeight > nStartHeight + nHeight)
			{
				if (nReturnFlag || x > 0)
				{
					pCtlEdit->nStartLine++;
				}
			}
			else if (y < nStartHeight)
			{
				pCtlEdit->nStartLine--;
			}		
		}

		pCtlEdit->nCaretX = x;
		pCtlEdit->nCaretY = y;
	}
	else
	{
		pCtlEdit->nCaretX = x;
		pCtlEdit->nCaretY = y;

		if (pCtlEdit->nCaretX >= pCtlEdit->nStartX + nWidth)
		{
			pCtlEdit->nStartX += 40;
			PrvCtlEditUpdate(hCtlWnd);

			PrvCtlEditGetCaretPos(hCtlWnd, pCtlEdit->nPos, &pCtlEdit->nCaretX, &pCtlEdit->nCaretY, NULL);
		}
		else if (pCtlEdit->nCaretX <= pCtlEdit->nStartX)
		{
			pCtlEdit->nStartX = max(0, pCtlEdit->nStartX - 40);
			PrvCtlEditUpdate(hCtlWnd);

			PrvCtlEditGetCaretPos(hCtlWnd, pCtlEdit->nPos, &pCtlEdit->nCaretX, &pCtlEdit->nCaretY, NULL);
		}

	}

	PrvCtlEditUpdateCaret(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditPostProcess() Return %d\n", 1));
	return 1;
}

static int PrvCtlEditInsertChars(WND hCtlWnd, char* pChars, int nCharsLen)
{
	int nNewPos, nNewLen, nNewSize;
	char *pPrevText, *pCurrText;
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditInsertChars(hCtlWnd=0x%x, pChars=0x%x, nCharsLen=%d) Called\n", hCtlWnd, pChars, nCharsLen));
	if (!pCtlEdit)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditInsertChars() Return %d\n", 0));
		return 0;
	}

	nNewLen = pCtlEdit->nLength + nCharsLen;
	nNewSize = CTL_EDIT_GETMEMSIZE(nNewLen);

//	if (nNewSize >= CtlEditBUFFER_MAXSIZE)
//		return 0;

	pCurrText = pCtlEdit->pString; 
	pPrevText = pCtlEdit->pString; // hWnd->text;

	nNewPos = pCtlEdit->nPos + nCharsLen;

	if (pCtlEdit->nPos < pCtlEdit->nLength)
		MemCopy(pCurrText + nNewPos, pPrevText + pCtlEdit->nPos, pCtlEdit->nLength - pCtlEdit->nPos);

	if (nCharsLen > 0)
		MemCopy(pCurrText + pCtlEdit->nPos, pChars, nCharsLen);

	pCtlEdit->nPos = nNewPos;
	pCtlEdit->nLength = nNewLen;

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditInsertChars() Return %d\n", 1));
	return 1;
}

static int PrvCtlEditDeleteChars(WND hCtlWnd, int nBackDelete)
{
	char *pTextPtr;
	char *pNewPtr = NULL;
	int nNewPos, nNewLen;
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDeleteChars(hCtlWnd=0x%x, nBackDelete=%d) Called\n", hCtlWnd, nBackDelete));
	if (!pCtlEdit)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDeleteChars() Return %d\n", 0));
		return 0;
	}

	if ((nBackDelete && pCtlEdit->nPos <= 0) ||
		(!nBackDelete && pCtlEdit->nPos >= pCtlEdit->nLength))
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDeleteChars() Return %d\n", 0));
		return 0;
	}

	nNewPos = pCtlEdit->nPos;
	pTextPtr = pCtlEdit->pString;

	if (nBackDelete)
		nNewPos = (pCtlEdit->nPos > 1 && PrvCtlEditCheckLeadByte(pTextPtr, pCtlEdit->nPos - 2)) ? pCtlEdit->nPos - 2 : pCtlEdit->nPos - 1;
	else
		nNewPos = (pCtlEdit->nPos < pCtlEdit->nLength - 1 && pTextPtr[pCtlEdit->nPos] & 0x80) ? pCtlEdit->nPos + 2 : pCtlEdit->nPos + 1;

	if (nNewPos == pCtlEdit->nPos)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDeleteChars() Return %d\n", 0));
		return 0;
	}

	nNewLen = pCtlEdit->nLength - abs(nNewPos - pCtlEdit->nPos);
	pNewPtr = pTextPtr;

	if (nBackDelete)
		MemCopy(pNewPtr + nNewPos, pTextPtr + pCtlEdit->nPos, pCtlEdit->nLength - pCtlEdit->nPos);
	else
		MemCopy(pNewPtr + pCtlEdit->nPos, pTextPtr + nNewPos, pCtlEdit->nLength - nNewPos);

	pCtlEdit->nLength = nNewLen;
	if (pCtlEdit->nLength < CTL_EDIT_BUFFER_MAXSIZE - 1)
		pTextPtr[pCtlEdit->nLength] = '\0';

	if (nBackDelete)
		pCtlEdit->nPos = nNewPos;

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDeleteChars() Return %d\n", 1));
	return 1;
}

static int PrvCtlEditUpdateCaret(WND hCtlWnd)
{
	int x, y;
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditUpdateCaret(hCtlWnd=0x%x) Called\n", hCtlWnd));
	if (!pCtlEdit)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditUpdateCaret() Return %d\n", 0));
		return 0;
	}

//	if ((pCtlEdit->nStatus & CTL_EDIT_STATUS) == CTL_EDIT_STATUS_FOCUS)
	{
		CtlEditCaretCreate(&pCtlEdit->sCaret, hCtlWnd, 2, 11, CTL_EDIT_CARET_TIMER_E);

		x = pCtlEdit->nCaretX + CTL_EDIT_MARGIN - pCtlEdit->nStartX;
		y = pCtlEdit->nCaretY + CTL_EDIT_MARGIN - pCtlEdit->nStartLine * PrvCtlEditGetLineHeight();

		CtlEditCaretSetPos(&pCtlEdit->sCaret, x, y); 
		CtlEditCaretShow(&pCtlEdit->sCaret);
	}
//	else
//		CtlEditCaretHide(hWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditUpdateCaret() Return %d\n", 1));
	return 1;
}

static int PrvCtlEditUpdate(WND hCtlWnd)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditUpdate(hCtlWnd=0x%x) Called\n", hCtlWnd));
	WinUpdateControl(hCtlWnd, 1);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditUpdate() Return %d\n", 1));
	return 1;
}

static int PrvCtlEditDrawText(WND hCtlWnd, rect *pTextRect)
{
	int nCode, nCodeWidth;
	int i, x, y;
	int nLineLen, nLineWidth, nLineHeight;
	int nWidth, nHeight, nStartHeight;
	char *pTextPtr;
	char *pLinePtr, *pPrevPtr;
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDrawText(hCtlWnd=0x%x, pTextRect=0x%x) Called\n", hCtlWnd, pTextRect));
	if (!pCtlEdit || pCtlEdit->nLength <= 0)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDrawText() Return %d\n", 0));
		return 0;
	}

	pTextPtr = pCtlEdit->pString;

	x = 0;
	y = 0;

	nLineLen = 0;
	nLineWidth = 0;
	nLineHeight = PrvCtlEditGetLineHeight();

	nWidth = pTextRect->right - pTextRect->left + 1;
	nHeight = pTextRect->bottom - pTextRect->top + 1;
	nStartHeight = nLineHeight * pCtlEdit->nStartLine;

	pLinePtr = pTextPtr;
	
	for (i = 0; i < pCtlEdit->nLength; i++)
	{
		pPrevPtr = pTextPtr;

		nCode = (char)*pTextPtr++;

		if (pCtlEdit->bPassword)
			nCodeWidth = FntGetFontWidth(&FntGlobalContext, '*');
		else
			nCodeWidth = FntGetFontWidth(&FntGlobalContext, (WORD)nCode);

		if (pCtlEdit->bMultiLine)
		{
			if (nLineWidth + nCodeWidth >= nWidth || nCode == 0x0A)
			{
				if (y >= nStartHeight)
				{
					GdiLockScreen(hCtlWnd);
					GdiTextOut(hCtlWnd, pLinePtr, nLineLen, (rint16)(pTextRect->left + x), (rint16)(pTextRect->top + y - nStartHeight));
					GdiUnlockScreen(hCtlWnd);
				}

				if (nCode == 0x0A)
				{
					nLineLen = 0;
					nLineWidth = 0;
					pLinePtr = pTextPtr;
				}
				else
				{
					nLineLen = (*pPrevPtr & 0x80) ? 2 : 1;
					nLineWidth = nCodeWidth;
					pLinePtr = pPrevPtr;
				}

				y += PrvCtlEditGetLineHeight();
				/*
				if (y + FntGetTextHeight(&FntGlobalContext) >= nStartHeight + nHeight)
				{
					//y += CtlEditGetLineHeight();
					CtlEditSetScrollPos( hWnd, y);
					return 1;
				
				}
				*/
			}
			else
			{
				nLineLen += 1;			
				nLineWidth += nCodeWidth;
			}
		}
		else	// single-line
		{
			if (nLineWidth >= pCtlEdit->nStartX && pLinePtr == pCtlEdit->pString && pCtlEdit->nStartX > 0)
			{
				nLineLen = 0;
				pLinePtr = pPrevPtr;

				pCtlEdit->nStartX = nLineWidth;
			}

			if (nLineWidth + nCodeWidth >= pCtlEdit->nStartX + nWidth)
			{
				GdiLockScreen(hCtlWnd);

				if (pCtlEdit->bPassword)
					GdiTextOut(hCtlWnd, CTL_EDIT_STRING_PASSWORD, nLineLen, (rint16)(pTextRect->left + x), (rint16)(pTextRect->top + y - nStartHeight));
				else
					GdiTextOut(hCtlWnd, pLinePtr, nLineLen, (rint16)(pTextRect->left + x), (rint16)(pTextRect->top + y - nStartHeight));

				GdiUnlockScreen(hCtlWnd);

				TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDrawText() Return %d\n", 1));
				return 1;
			}
			else
			{
				nLineLen++;
				nLineWidth += nCodeWidth;
			}
		}
	}

	if (y >= nStartHeight)
	{
		GdiLockScreen(hCtlWnd);

		if (pCtlEdit->bPassword)
			GdiTextOut(hCtlWnd, CTL_EDIT_STRING_PASSWORD, nLineLen, (rint16)(pTextRect->left + x), (rint16)(pTextRect->top + y - nStartHeight));	
		else
			GdiTextOut(hCtlWnd, pLinePtr, nLineLen, (rint16)(pTextRect->left + x), (rint16)(pTextRect->top + y - nStartHeight));	

		GdiUnlockScreen(hCtlWnd);
	}

	PrvCtlEditDrawScrollBar(hCtlWnd, y);
	
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDrawText() Return %d\n", 1));
	return 1;
}

static int PrvCtlEditDrawScrollBar(WND hCtlWnd, int nTotalWidth )
{
	int nTotalLines, nMaxLines, nCaretLine;
	int nPreLine, nNextLine;
	int nPrePos, nNextPos, nCaretPos;
	int nRectHeight;
	int nScrollLine, nScrollPos;
	
	rect rtClient;

	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDrawScrollBar(hCtlWnd=0x%x, nTotalWidth=%d) Called\n", hCtlWnd, nTotalWidth));
	nRectHeight = pCtlEdit->nRectHeight + 1;
	
	nTotalLines = (nTotalWidth / PrvCtlEditGetLineHeight()) + 1;
	nMaxLines   = pCtlEdit->nMaxLines;

	if ( pCtlEdit->nCaretY == 0)
		nCaretLine	= 1;
	else
		nCaretLine = (pCtlEdit->nCaretY / PrvCtlEditGetLineHeight()) + 1;

	nNextLine = nTotalLines - nCaretLine; 
	nPreLine  = nTotalLines - nNextLine -1;

	nScrollLine = nTotalLines - nMaxLines;

	if ( pCtlEdit->nPrvKeyCode  == VK_USER_RIGHT ||
		pCtlEdit->nPrvKeyCode == VK_USER_UP	     ||
		pCtlEdit->nPrvKeyCode == VK_USER_BACKSPACE	||
			pCtlEdit->nPrvKeyCode == VK_USER_DELETE )
		nCaretLine	-= 1;

	else
		nCaretLine	-= nMaxLines;

	pCtlEdit->nPreTotalLines = nTotalLines;

	if ( nScrollLine <= 0)
		nScrollPos = 0;
	else
		nScrollPos =  ((nRectHeight) * (10000 / (nTotalLines * 100 / nScrollLine))) / 100;

	if ( nCaretLine <= 0 )
		nCaretPos = 0;
	else
		nCaretPos = ((nRectHeight) * (10000 / (nTotalLines * 100 / nCaretLine))) / 100;

	if ( nPreLine > 0 )
		nPrePos	= (((nRectHeight) ) * (nPreLine))  / 100;
	else
		nPrePos	= 0;
	
	if ( nNextLine > 0 )
		nNextPos = (((nRectHeight)) * (nNextLine)) / 100;
	else
		nNextPos = nRectHeight;

	if ( nPreLine == 0 && nNextLine >= 0 && nNextLine <= (nMaxLines - 1))
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDrawScrollBar() Return\n"));
		return 0;			
	}

	if ( nPreLine >=0 && nPreLine <= (nMaxLines - 1) )
	{
		if ( (nNextLine > 0 && (nNextLine+nPreLine) > (nMaxLines -1)) )
		{
			if ( pCtlEdit->nPrvKeyCode == VK_USER_UP		||
				pCtlEdit->nPrvKeyCode == VK_USER_LEFT		||
				pCtlEdit->nPrvKeyCode == VK_USER_BACKSPACE	||
				pCtlEdit->nPrvKeyCode == VK_USER_DELETE)
			{
				rtClient.top	= nCaretPos;
				rtClient.bottom = nRectHeight - (nScrollPos - nCaretPos);
			}
			else
			{
				rtClient.top	= 0;
				rtClient.bottom = nRectHeight - nScrollPos;
			}
		}
		else if ( nNextLine == 0 && \
				  pCtlEdit->nPrvKeyCode == VK_USER_BACKSPACE)
		{
			rtClient.top	= nPrePos;
			rtClient.bottom = nRectHeight;
		}

	}

	if ( nPreLine > (nMaxLines - 1) && nNextLine >= 0 )
	{
		if ( nNextLine > 0 )
		{
			if ( nCaretLine > nScrollLine)
			{
				rtClient.top	= nScrollPos;
				rtClient.bottom = nRectHeight;
			}
			else
			{
				rtClient.top	= nCaretPos;
				rtClient.bottom = nRectHeight - (nScrollPos - nCaretPos);
			}


		}	
		else
		{
			rtClient.top	= nScrollPos;
			rtClient.bottom = nRectHeight;
		}
	}

	rtClient.left	= pCtlEdit->sWndRect.right - 25;
	rtClient.right	= pCtlEdit->sWndRect.right;

	GdiLockScreen(hCtlWnd);
	GdiRectangle (hCtlWnd, &rtClient);
	GdiUnlockScreen(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDrawScrollBar() Return\n"));
	return 1;
}

static int PrvCtlEditGetCaretPos(WND hCtlWnd, int pos, int *px, int *py, int *px2)
{
	int nRet;
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetCaretPos(hCtlWnd=0x%x, pos=%d, px=0x%x, py=0x%x, px2=0x%x) Called\n", hCtlWnd, pos, px, py, px2));

	nRet = PrvCtlEditGetPosition(hCtlWnd, &pos, px, py, px2, CTL_EDIT_POS_CARET);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetCaretPos() Return %d\n", nRet));
	return nRet;
}


static int PrvCtlEditGetTextPos(WND hCtlWnd, int x, int y, int *pos)
{
	int nRet;
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetTextPos(hCtlWnd=0x%x, x=%d, y=%d, *pos=%d) Called\n", hCtlWnd, x, y, *pos));

	nRet = PrvCtlEditGetPosition(hCtlWnd, pos, &x, &y, NULL, CTL_EDIT_POS_TEXT);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetTextPos(*pos=%d) Return %d\n", *pos, nRet));
	return nRet;
}

static int PrvCtlEditGetPosition(WND hCtlWnd, int *pos, int *px, int *py, int *px2, int what)
{
	int i, x, y, x2, nCode;
	int nCodeWidth, nWidth;
	rect rcText;
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);
	char *pPrevPtr, *pTextPtr = pCtlEdit->pString; 

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetPosition(hCtlWnd=0x%x, pos=0x%x, px=0x%x, py=0x%x, px2=0x%x, what=%d) Called\n", hCtlWnd, pos, px, py, px2, what));
	if (!pCtlEdit || pCtlEdit->nLength <= 0)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetPosition() Return %d\n", 0));
		return 0;
	}

	if ((what == CTL_EDIT_POS_CARET && !pos) ||
		(what == CTL_EDIT_POS_TEXT && (!py || !px)) ||
		(what == CTL_EDIT_POS_HEIGHT && !py))	
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetPosition() Return %d\n", 0));
		return 0;
	}

	PrvCtlEditGetTextRect(hCtlWnd, &rcText);

	nWidth = rcText.right - rcText.left + 1;
	nCodeWidth = 0;

	x = y = x2 = 0;

	for (i = 0; i < pCtlEdit->nLength; i++)
	{
		pPrevPtr = pTextPtr;

		if (pCtlEdit->bPassword)
			nCode = (char)*pTextPtr++;
		else
		{
			nCode = (char)*pTextPtr++;
		}

		if (nCode == 0x0A)
			nCodeWidth = 0;
		else
		{
			if (pCtlEdit->bPassword)
				nCodeWidth = FntGetFontWidth(&FntGlobalContext, '*');
			else
				nCodeWidth = FntGetFontWidth(&FntGlobalContext, (WORD)nCode);
		}

		if (pCtlEdit->bMultiLine)
		{
			if ((what == CTL_EDIT_POS_CARET && *pos <= i) ||
				(what == CTL_EDIT_POS_TEXT && y >= *py && x + nCodeWidth / 2 >= *px))
				break;

			if (nCode == 0x0A || x + nCodeWidth + FntGetFontWidth(&FntGlobalContext, *pTextPtr) >= nWidth)
			{			
				if (what == CTL_EDIT_POS_TEXT && y >= *py)
					break;

				x2 = x + nCodeWidth;
				x = 0;

				y += PrvCtlEditGetLineHeight();
			}
			else 
			{
				x += nCodeWidth;
				x2 = x;
			}
		}
		else	// single-line
		{
			if ((what == CTL_EDIT_POS_CARET && *pos <= i) ||
				(what == CTL_EDIT_POS_TEXT && x >= *px))
				break;

			x += nCodeWidth;
			x2 = x;
		}

		if (!(pCtlEdit->bPassword))
		{
			if (*pPrevPtr & 0x80)
				i++;
		}
	}

	if (what == CTL_EDIT_POS_CARET)
	{
		if (px2)
			*px2 = x2;

		*px = x;
		*py = y;
	}
	else if (what == CTL_EDIT_POS_TEXT)
		*pos = i;
	else if (what == CTL_EDIT_POS_HEIGHT)
	{
		*py = y;

		if (x > 0)
			*py += PrvCtlEditGetLineHeight();
	}

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetPosition() Return %d\n", 1));
	return 1;
}

static int PrvCtlEditGetLineHeight()
{
	int nRet;
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetLineHeight() Called\n", 0));
	nRet = FntGetTextHeight(&FntGlobalContext) + 1;
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetLineHeight() Return %d\n", nRet));
	return nRet;
}

static int PrvCtlEditGetTextRect(WND hCtlWnd, rect *pTextRect)
{
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	rect rcClient;

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetTextRect(hCtlWnd=0x%x, *pTextRect=0x%x) Called\n", hCtlWnd, pTextRect));
	
	SET_RECT(&rcClient, 0, 0, (hCtlWnd->frame.right - hCtlWnd->frame.left - 1), (hCtlWnd->frame.bottom - hCtlWnd->frame.top - 1));
	
	GdiInflateRect(&rcClient, -CTL_EDIT_MARGIN, -CTL_EDIT_MARGIN);
	
	pCtlEdit->nMaxLines = (rcClient.bottom - rcClient.top + 1) / PrvCtlEditGetLineHeight();

	if (pTextRect)
		*pTextRect = rcClient;
			
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditGetTextRect() Return %d\n", 1));
	return 1;
}

static int PrvCtlEditDraw(WND hCtlWnd, rect *pClientRect)
{
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);
	rect rcRect = *pClientRect;

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDraw(hCtlWnd=0x%x, pClientRect=0x%x) Called\n", hCtlWnd, pClientRect));
	if (!pCtlEdit)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDraw() Return %d\n", 0));
		return 0;
	}

	GdiLockScreen(hCtlWnd);

	GdiSetFgColor(hCtlWnd, hCtlWnd->bg_color);				// Border
	GdiRectangle(hCtlWnd, &rcRect);

	GdiInflateRect(&rcRect, -1, -1);
	GdiSetFgColor(hCtlWnd, CTL_EDIT_COLOR_BORDER_IN);	// Border-in
	GdiRectangle(hCtlWnd, &rcRect);

	GdiInflateRect(&rcRect, -1, -1);
	GdiFillRect(hCtlWnd, &rcRect, CTL_EDIT_COLOR_WINDOW);

	if (pCtlEdit->nLength > 0)
	{
		GdiSetFgColor(hCtlWnd, CTL_EDIT_COLOR_TEXT);
		GdiSetBkColor(hCtlWnd, CTL_EDIT_COLOR_WINDOW);

		PrvCtlEditGetTextRect(hCtlWnd, &rcRect);
		PrvCtlEditDrawText(hCtlWnd, &rcRect);
	}

	GdiUnlockScreen(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDraw() Return %d\n", 0));
	return 0;
}


static int PrvCtlEditCheckLeadByte(char* pszLine, int nPos)
{
	int i, nCheck = 0;

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCheckLeadByte(pszLine=0x%x, nPos=%d) Called\n", pszLine, nPos));
	for (i = nPos; i >= 0; i--)
	{
		if (pszLine[i] & 0x80) nCheck++;
		else break;
	}

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCheckLeadByte() Return %d\n", nCheck % 2));
	return nCheck % 2;
}


static int PrvCtlEditInitCaret(WND hCtlWnd)
{
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditInitCaret(hCtlWnd=0x%x) Called\n", hCtlWnd));
	if (!pCtlEdit)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditInitCaret() Return %d\n", 0));
		return 0;
	}

	pCtlEdit->nCaretX = 0;
	pCtlEdit->nCaretY = 0;

	pCtlEdit->nPos = 0;
	pCtlEdit->nStartX = 0;
	pCtlEdit->nStartLine = 0;

	PrvCtlEditUpdateCaret(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditInitCaret() Return %d\n", 1));
	return 1;
}

////////////////////////////////////////////////////////////
// Message Processing Related Functions
//
/** 
 * @brief       Create a new edit control.
 * @param[in]	hCtlWnd        Windows handle
 * @return      NONE
 * @remarks     This function creates new edit control and initiate its value.
 * @if HISTORY
 *				13-Jun-2006 yich Initiated
 * @endif
 */
void CtlEditMsgCreate(WND hCtlWnd)
{
	REXY_CTL_EDIT	*pCtlEdit;
	int				nErr;
	
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgCreate(hCtlWnd=0x%x) Called\n", hCtlWnd));

	//Allocate memory for control structure.
	hCtlWnd->pData = MemAlloc(sizeof(REXY_CTL_EDIT));

    if(hCtlWnd->pData == NULL)
	{
		WinDestroyControl(hCtlWnd);
		hCtlWnd = NULL;

		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCreate() Return\n"));
		return;
	}

	pCtlEdit = (REXY_CTL_EDIT*)CtlGetStruct(hCtlWnd);

    //Initialize memory allocated - control.
    MemSet(pCtlEdit, 0, sizeof(REXY_CTL_EDIT));
	
    //Set default values to edit control structure.
	MemSet(pCtlEdit->pString, 0, sizeof(pCtlEdit->pString));
	pCtlEdit->nLength			= 0;
	pCtlEdit->nMaxChars			= sizeof(pCtlEdit->pString);

	pCtlEdit->sWndRect.left		= 0;
    pCtlEdit->sWndRect.top		= 0;
    pCtlEdit->sWndRect.right	= 0;
    pCtlEdit->sWndRect.bottom	= 0;

	pCtlEdit->nMaxLines			= 0;

	pCtlEdit->nCaretX			= 0;
	pCtlEdit->nCaretY			= 0;
	pCtlEdit->nPos				= 0;
	pCtlEdit->nStartX			= 0;
	pCtlEdit->nStartLine		= 0;

	pCtlEdit->nPreTotalLines	= 0;
	pCtlEdit->bMultiLine		= FALSE;	// single or multiline

	MemSet(&pCtlEdit->sCaret, 0, sizeof(REXY_CTL_EDIT_CARET));

	PrvCtlEditSetWnd(hCtlWnd, &nErr);
//	CHECK_ERR_IF_NOT_EQUAL(nErr, CTLERR_NONE, 0);

	CtlSetWNDColor(hCtlWnd, COLOR_BLACK, COLOR_WHITE, &nErr);
//	CHECK_ERR_IF_NOT_EQUAL(nErr, CTLERR_NONE, 0);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCreate() Return\n", 0));
    return;
}

/** 
 * @brief       Destroys created edit control.
 * @param[in]   hCtlWnd			Pointer to control structure
 * @param[out]  pErr            Pointer to error value
 * @retval      RxErrNone       Always return RxErrNone
 * @remarks     Destroys created edit control.\n
 *              Free allocated memory for edit control data.
 * @if HISTORY
 *				13-Jun-2006 yich Initiated
 * @endif
 */
int CtlEditMsgDestroy(WND hCtlWnd, int *pErr)
{
    REXY_CTL_EDIT *pCtlEdit;

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDestroy(hCtlWnd=0x%x, *pErr=%d) Called\n", hCtlWnd, *pErr));

    pCtlEdit = PrvCtlEditGetDataP(hCtlWnd, pErr);
    CHECK_NULLPTR(pCtlEdit, 0, *pErr);

	CtlEditCaretDestroy(&pCtlEdit->sCaret);

    MemFree(pCtlEdit);
    hCtlWnd->pData = NULL;

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDestroy() Return %d\n", RxErrNone));
    return RxErrNone;
}

/** 
 * @brief		Edit control에서 필요한 키 처리를 한다.
 * @param[in]	hCtlWnd			Pointer to control structure
 * @param[in]	nKey			처리할 키 값
 * @param[out]	pErr			Pointer to error value
 * @retval		RxErrNone		Always return RxErrNone
 * @if HISTORY
 *				13-Jun-2006 yich Initiated
 * @endif
 */
int CtlEditMsgKeyDown(WND hCtlWnd, int nKey, int *pErr)
{


	REXY_CTL_EDIT *pCtlEdit;
	
	int bHandled = FALSE;
	
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgKeyDown(hCtlWnd=0x%x, nKey=%d, *pErr=%d)\n", hCtlWnd, nKey, *pErr));

	pCtlEdit = PrvCtlEditGetDataP(hCtlWnd, pErr);
	CHECK_NULLPTR(pCtlEdit, 0, *pErr);

	pCtlEdit->nPrvKeyCode = nKey;

    if (pCtlEdit->pfnCBKeyDown != NULL)
	{
		int nLength;
		pCtlEdit->pfnCBKeyDown(hCtlWnd, nKey, pCtlEdit->pString, &nLength);
		
		bHandled = TRUE;
	}
    else
    {
		switch (nKey)
		{
			case VK_USER_LEFT:
			case VK_USER_RIGHT:
			case VK_USER_UP:
			case VK_USER_DOWN:
				PrvCtlEditMovePos(hCtlWnd, nKey);
				bHandled = TRUE;
				break;
			
			case VK_USER_DELETE:
				PrvCtlEditDeleteChars(hCtlWnd, FALSE);
				PrvCtlEditPostProcess(hCtlWnd);
				PrvCtlEditUpdate(hCtlWnd);
				bHandled = TRUE;
				break;

			case VK_USER_BACKSPACE:
				PrvCtlEditDeleteChars(hCtlWnd, TRUE);
				PrvCtlEditPostProcess(hCtlWnd);
				PrvCtlEditUpdate(hCtlWnd);
				bHandled = TRUE;
				break;

			default:
				PrvCtlEditMsgChar(hCtlWnd, nKey);
				bHandled = TRUE;
				break;
		}
    }
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgKeyDown() Return %d\n", 1));
	return 	bHandled = TRUE;		
}

/**
 * @brief		Edit control를 화면에 그린다.
 * @param[in]	hCtlWnd		컨트롤 윈도 핸들
 * @return		
 * @if HISTORY
 *				13-Jun-2006	yich Initiated
 * @endif
 */
int CtlEditMsgPaint(WND hCtlWnd)
{
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgPaint(hCtlWnd=0x%x)\n", hCtlWnd));

    if (pCtlEdit->pfnCBDraw != NULL)
	{
		pCtlEdit->pfnCBDraw(hCtlWnd, pCtlEdit->pString + pCtlEdit->nPos, -1);
	}
	else
	{
		COLORVAL old_fg_color = GdiGetFgColor(hCtlWnd);
		COLORVAL old_bg_color = GdiGetBkColor(hCtlWnd);
		rect sClientRect = {0, 0, };

		//Lock screen to prevent flickering.
		GdiLockScreen(hCtlWnd);

		CtlEditCaretHide(&pCtlEdit->sCaret);

		sClientRect.right = pCtlEdit->sWndRect.right - pCtlEdit->sWndRect.left;
		sClientRect.bottom = pCtlEdit->sWndRect.bottom - pCtlEdit->sWndRect.top;

		PrvCtlEditDraw(hCtlWnd, &sClientRect);

		GdiSetFgColor(hCtlWnd, old_fg_color);
		GdiSetBkColor(hCtlWnd, old_bg_color);

		CtlEditCaretShow(&pCtlEdit->sCaret);

		//Unlock screen.
		GdiUnlockScreen(hCtlWnd);
	}

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgPaint() Return %d\n", 1));
	return 1;
}

/**
 * @brief		Edit 컨트롤에서 타이머를 처리한다.
 * @param[in]	hCtlWnd		컨트롤 윈도 핸들
 * @param[in]	nTimerId	처리할 타이머 ID
 * @return		
 * @if HISTORY
 *				13-Jun-2006	yich Initiated
 * @endif
 */
int CtlEditMsgTimer(WND hCtlWnd, int nTimerId)
{
	REXY_CTL_EDIT *pCtlEdit = (REXY_CTL_EDIT *)CtlGetStruct(hCtlWnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgTimer(hCtlWnd=0x%x, nTimerId=%d) Called\n", hCtlWnd, nTimerId));

	if (!pCtlEdit)
	{
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgTimer() Return %d\n", 0));
		return 0;
	}

	switch (nTimerId)
	{
		case REXY_CTL_EDIT_TIMER:		
			UsrKillTimer(hCtlWnd, nTimerId);
			break;

		case REXY_CTL_EDIT_CARET_TIMER:
			CtlEditCaretDraw(&pCtlEdit->sCaret);
			break;
	}

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditMsgTimer() Return %d\n", 1));
	return 1;
}

/** 
 * @brief        Main message handler of edit control.
 * @param[in]    hCtlWnd     Pointer to control structure
 * @param[in]    nMsg        Message to handle
 * @param[in]    wParam      wparam of message
 * @param[in]    lParam      lparam of message
 * @retval       RxErrNone   If nothing to handle or no error occurred
 * @remarks      This function handles messages for edit control.\n
 *               This function is called by control core if some messages is needed to be handled by edit control.
 * @if HISTORY
 *         13-Jun-2006 yich Initiated
 * @endif
*/
int CtlEditProc(WND hCtlWnd, int nMsg, int wParam, int lParam)
{
	int nErr, nRet;

   	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditProc(hCtlWnd=0x%x, nMsg=%d, wParam=%d, lParam=%d) Called\n", hCtlWnd, nMsg, wParam, lParam));
	switch(nMsg)
    {
    case AWM_CREATE:
		CtlEditMsgCreate(hCtlWnd);
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditProc() Return %d\n", 0));
        return 0;

    case AWM_DESTROY:
        nRet = CtlEditMsgDestroy(hCtlWnd, &nErr);
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditProc() Return %d\n", nRet));
		return nRet;

	case AWM_KEYDOWN:
		nRet = CtlEditMsgKeyDown(hCtlWnd, wParam, &nErr);
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditProc() Return %d\n", nRet));
		return nRet;

	case AWM_PAINT:
		nRet = CtlEditMsgPaint(hCtlWnd);
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditProc() Return %d\n", nRet));
		return nRet;

	case AWM_TIMER:
		nRet = CtlEditMsgTimer(hCtlWnd, wParam);
		TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditProc() Return %d\n", nRet));
		return nRet;
	}

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditProc() Return %d\n", RxErrNone));
    return RxErrNone;
}


////////////////////////////////////////////////////////////
// Callback Related Functions
//
/** 
 * @brief        Call back function called after creating edit control.
 * @param[in]    hCtlWnd            Pointer to control structure
 * @param[out]   pErr            Pointer to error value
 * @retval       RxErrNone       Always return RxErrNone
 * @remarks      Currently does nothing.
 * @if HISTORY
 * @endif
 */
int CtlEditCreatePostCB(WND hCtlWnd, int *pErr)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCreatePostCB(hCtlWnd=0x%x, *pErr=%d)\n", hCtlWnd, *pErr));
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCreatePostCB() Return %d\n", RxErrNone));
    return RxErrNone;
}

/** 
 * @brief        Call back function called before destroying edit control.
 * @param[in]    hCtlWnd            Pointer to control structure
 * @param[out]   pErr            Pointer to error value
 * @retval       RxErrNone       On success
 * @remarks      Currently does nothing.
 * @if HISTORY
 * @endif
 */
int CtlEditDestroyPreCB(WND hCtlWnd, int *pErr)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDestroyPreCB(hCtlWnd=0x%x, *pErr=%d) Called\n", hCtlWnd, *pErr));
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDestroyPreCB() Return %d\n", RxErrNone));
    return RxErrNone;
}

/** 
 * @brief        Call back function called before drawing edit control.
 * @param[in]    hCtlWnd            Pointer to control structure
 * @param[out]   pErr            Pointer to error value
 * @retval       RxErrNone       On success
 * @remarks      Currently does nothing.
 * @if HISTORY
 * @endif
 */
int CtlEditDrawPreCB(WND hCtlWnd, int *pErr)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDrawPreCB(hCtlWnd=0x%x, *pErr=%d) Called\n", hCtlWnd, *pErr));
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDrawPreCB() Return %d\n", RxErrNone));
	return RxErrNone;
}

/** 
 * @brief        Call back function called after drawing edit control.
 * @param[in]    hCtlWnd            Pointer to control structure
 * @param[out]   pErr            Pointer to error value
 * @retval       RxErrNone       On success
 * @remarks      Currently does nothing.
 * @if HISTORY
 * @endif
 */
int CtlEditDrawPostCB(WND hCtlWnd, int *pErr)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDrawPostCB(hCtlWnd=0x%x, *pErr=%d) Called\n", hCtlWnd, *pErr));
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditDrawPostCB() Return %d\n", RxErrNone));
	return RxErrNone;
}

/** 
* @brief        Callback message hanlder of edit control.
* @param[in]    hCtlWnd       Pointer to control structure
* @param[in]    nMsg        Message to handle
* @param[in]    wParam      wparam of message
* @param[in]    lParam      lparam of message
* @param[in]    bIsPre      0 if functions is called for pre-callback\n
*                           Besides 0 if function called for post-callback
* @param[out]   pErr        Pointer to error value
* @retval       RxErrNone   If nothing to handle or no error occurred
* @remarks      This callback function called after handling some messages.
* @if HISTORY
*         13-Jun-2006 yich Initiated
* @endif
*/
int CtlEditCBProc(WND hCtlWnd, int nMsg, int wParam, int lParam, int bIsPre, int *pErr)
{
	int nRet;

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCBProc(hCtlWnd=0x%x, nMsg=%d, wParam=%d, lParam=%d, *pErr=%d) Called\n", hCtlWnd, nMsg, wParam, lParam, *pErr));

    switch(nMsg)
    {
    case AWM_CREATE:
		CtlEditCreatePostCB(hCtlWnd, pErr);
		break;
	case AWM_DESTROY:
		CtlEditDestroyPreCB(hCtlWnd, pErr);
		break;
	case AWM_PAINT:
		if (bIsPre)
		{
			nRet = CtlEditDrawPreCB(hCtlWnd, pErr);
			TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCBProc() Return %d\n", nRet));
			return nRet;
		}
		else
		{
			nRet = CtlEditDrawPostCB(hCtlWnd, pErr);
			TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCBProc() Return %d\n", nRet));
			return nRet;
		}
	
	default:
		break;
    }

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCBProc() Return %d\n", RxErrNone));
    return RxErrNone;
}


////////////////////////////////////////////////////////////
// Command Related Functions
//
/** 
 * @brief        Command handler of edit control.
 * @param[in]    hCtlWnd Pointer to control structure
 * @param[in]    nCmd    Command ID
 * @param[in]    wParam  wparam of command
 * @param[in]    lParam  lparam of command
 * @param[out]   pErr    Pointer to error value
 * @retval       Varies by command ID
 * @remarks      This function commands to edit control to do various functions.\n
 *               By command ID edit control set its inner member value or,
 *               returns its inner member value to whom it called.\n
 *               Also does some functions such as drawing by specified command ID.
 * @if HISTORY
 *         13-Jun-2006 yich Initiated
 * @endif
 */
int CtlEditCmdProc(WND hCtlWnd, int nCmd, int wParam, int lParam, int *pErr)
{
    int nRet = 0;
    REXY_CTL_EDIT *pCtlEdit;

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCmdProc(hCtlWnd=0x%x, nCmd=0x%x, wParam=%d, lParam=%d, *pErr=%d) Called\n", hCtlWnd, nCmd, wParam, lParam, *pErr));
    pCtlEdit = PrvCtlEditGetDataP(hCtlWnd, pErr);
    CHECK_NULLPTR(pCtlEdit, 0, *pErr);

    switch (nCmd)
    {
	// 필드
	case CTL_EDIT_CMD_GET_WND_RECT:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_GET_WND_RECT=0x%x,{%d, %d, %d, %d}\n", &pCtlEdit->sWndRect, pCtlEdit->sWndRect.left, pCtlEdit->sWndRect.top, pCtlEdit->sWndRect.right, pCtlEdit->sWndRect.bottom));
		nRet = (int)&pCtlEdit->sWndRect;
		break;
	case CTL_EDIT_CMD_SET_WND_RECT:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_SET_WND_RECT=0x%x,{%d, %d, %d, %d}\n", (rect*)lParam, ((rect*)lParam)->left, ((rect*)lParam)->top, ((rect*)lParam)->right, ((rect*)lParam)->bottom));
		MemCopy(&pCtlEdit->sWndRect, (rect*)lParam, sizeof(rect));
		PrvCtlEditSetWnd(hCtlWnd, pErr);
		break;

	case CTL_EDIT_CMD_GET_STRING:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_GET_STRING=0x%x\n", pCtlEdit->pString));
		nRet = (int)pCtlEdit->pString;
		break;
	case CTL_EDIT_CMD_SET_STRING:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_SET_STRING=0x%x\n", (rect*)lParam));
		PrvCtlEditMsgSetText(hCtlWnd, (char*)lParam);
		break;

    case CTL_EDIT_CMD_GET_MAX_LEN:			// 최대 길이
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_GET_STRING=0x%x\n", pCtlEdit->nMaxChars));
		nRet = pCtlEdit->nMaxChars;
		break;
    case CTL_EDIT_CMD_SET_MAX_LEN:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_SET_STRING=0x%x\n", wParam));
		pCtlEdit->nMaxChars = wParam;
		break;

	case CTL_EDIT_CMD_GET_PASSWORD:			// Password 입력모드 여부
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_GET_PASSWORD=0x%x\n", pCtlEdit->bPassword));
		nRet = pCtlEdit->bPassword;
		break;
	case CTL_EDIT_CMD_SET_PASSWORD:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_SET_STRING=0x%x\n", wParam));
		pCtlEdit->bPassword = wParam;
		break;

	case CTL_EDIT_CMD_GET_READONLY:			//
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_GET_READONLY=0x%x\n", pCtlEdit->bReadOnly));
		nRet = pCtlEdit->bReadOnly;
		break;
	case CTL_EDIT_CMD_SET_READONLY:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_SET_READONLY=0x%x\n", wParam));
		pCtlEdit->bReadOnly = wParam;
		break;

    case CTL_EDIT_CMD_GET_CURSOR_INDEX:		// 문자열내의 커서의 위치
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_GET_CURSOR_INDEX=0x%x\n", pCtlEdit->nPos));
		nRet = pCtlEdit->nPos;
		break;
    case CTL_EDIT_CMD_SET_CURSOR_INDEX:		
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_SET_CURSOR_INDEX=0x%x\n", wParam));
		pCtlEdit->nPos = wParam;
		break;

	case CTL_EDIT_CMD_GET_MULTILINE:			// 멀티 라인 여부
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_GET_MULTILINE=0x%x\n", pCtlEdit->bMultiLine));
		nRet = pCtlEdit->bMultiLine;
		break;
	case CTL_EDIT_CMD_SET_MULTILINE:			
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_SET_MULTILINE=0x%x\n", wParam));
		pCtlEdit->bMultiLine = wParam;
		break;

	// 명령
    case CTL_EDIT_CMD_DO_DRAW:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_DO_DRAW\n"));
		CtlDraw(hCtlWnd, pErr);
        break;

	// 콜백 함수
	case CTL_EDIT_CMD_GET_CB_DRAW:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_GET_DRAW_CB=0x%x\n", pCtlEdit->pfnCBDraw));
		nRet = (int)pCtlEdit->pfnCBDraw;
		break;
	case CTL_EDIT_CMD_SET_CB_DRAW:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_SET_DRAW_CB=0x%x\n", lParam));
		pCtlEdit->pfnCBDraw = (CtlEditCBDraw)lParam;
		break;

	case CTL_EDIT_CMD_GET_CB_KEYDOWN:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_GET_IME_CB=0x%x\n", pCtlEdit->pfnCBKeyDown));
		nRet = (int)pCtlEdit->pfnCBKeyDown;
		break;
	case CTL_EDIT_CMD_SET_CB_KEYDOWN:
		TRACE(TR_RexyControl, TR_LEVEL4, ("CTL_EDIT_CMD_SET_IME_CB=0x%x\n", lParam));
		pCtlEdit->pfnCBKeyDown = (CtlEditCBKeyDown)lParam;
		break;
    }

	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditCmdProc() Return %d\n", nRet));
    return nRet;
}


////////////////////////////////////////////////////////////
//  Unit Test Module
//
#ifdef UNIT_TEST
#define UNIT_TEST_ERR(err, n, str)  if(err != RxErrNone) \
                                    { TrcPrintF("UNIT TEST #%d : %s : FAILED\n", n, str); return; } \
                                    else \
                                    { TrcPrintF("UNIT TEST #%d : %s : OK\n", n, str); n++; }

#define UNIT_TEST_ITEMNUM       8
#define UNIT_TEST_FOCUS         4
#define UNIT_TEST_TOPINDEX      2
#define UNIT_TEST_ITEM_MARGIN   4

void CtlEditTestUnitDummyCB(WND hCtlWnd, int nLine, rect rtLineRect, void *pData)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditTestUnitDummyCB() Called\n", 0));
    //Do nothing
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditTestUnitDummyCB() Return\n"));
}

void CtlEditTestUnit()
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditTestUnit() Called\n", 0));
	TRACE(TR_RexyControl, TR_LEVEL3, ("CtlEditTestUnit() Return\n"));
}
#endif

/** @} */ // End of Edit Control
