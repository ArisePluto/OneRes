#pragma once
#include <afxwin.h>
#define EX 1
#define OH 2

// class ok
class MyAPP : public CWinApp
{
public:
	virtual BOOL InitInstance();

};
// class ok
class CMainWindow : public CWnd
{  // ok
protected:
	static const CRect m_rcSquared[9]; // ok
	int m_nGameGrid[9];
	int m_nNextChar;
	int GetRectID(CPoint point);
	void DrawBoard(CDC* pDC);
	void DrawX(CDC* pDC, int nPos);
	void DrawY(CDC* pDC, int nPos);
	void ResetGame();
	void CheckForGameOver();
	int IsWinner();
    BOOL IsDraw();

public:
	CMainWindow();
protected:
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg void  OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void  OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void  OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};