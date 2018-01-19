#include "input.h"

//建立一个APP 即窗口
MyAPP myapp;

//  初始化 创建窗口  然后陷入循环
BOOL MyAPP::InitInstance()
{    //创建一个窗开类
	m_pMainWnd = new CMainWindow;        //根据初始化函数进行初始化
	m_pMainWnd->ShowWindow(m_nCmdShow);  //将窗口加入预留画布
	m_pMainWnd->UpdateWindow();          //将画布更新到桌面
	return TRUE; 
}

//添加指定消息
BEGIN_MESSAGE_MAP(CMainWindow, CWnd)
	ON_WM_PAINT()  //绘画消息
	ON_WM_LBUTTONDOWN()   //左鼠标
	ON_WM_LBUTTONDBLCLK() //右鼠标
	ON_WM_RBUTTONDOWN()   //左鼠标双击
END_MESSAGE_MAP()

//用来描述矩形的边  九个框
const CRect CMainWindow::m_rcSquared[9] = {
	CRect(16,16 ,112,112),  CRect(128,16 ,224,112),  CRect(240,16 ,336,112),
	CRect(16,128,112,224),  CRect(128,128,224,224),  CRect(240,128,336,224),
	CRect(16,240,112,336),  CRect(128,240,224,336),  CRect(240,240,336,336)
};

// 窗口的初始化  
CMainWindow::CMainWindow()
{   // EX 先走
	m_nNextChar = EX;
	//对存储点进行初始化
	::ZeroMemory(m_nGameGrid, 9 * sizeof(int));
	//注册窗口类型
	CString strWndClass = AfxRegisterWndClass(
		CS_DBLCLKS, //类样式 窗口操作
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), //指定窗口识别类光标
		(HBRUSH)(COLOR_3DFACE+1),  //背景颜色
		AfxGetApp()->LoadStandardIcon(IDI_WINLOGO)); //桌面图标的句柄
	  //创建窗口
	    CreateEx(0, strWndClass, _T("MGame"),  //窗口名称
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL);
   //设置窗口大小
	CRect rect(0, 0, 352, 352);
	CalcWindowRect(&rect);

	//设置窗口属性
	SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);


}

//删除类的对象
void CMainWindow::PostNcDestroy()
{
	delete this;

}

//绘画函数
void CMainWindow::OnPaint()
{
	CPaintDC dc(this);
	//画出四条边界线
	DrawBoard(&dc);
}

//点击左键函数
void CMainWindow::OnLButtonDown(UINT nFlags, CPoint point)
{   // 不论到自己走
	if (m_nNextChar != EX)
		return;
	//获取那个一格子
	int nPos = GetRectID(point);	
	if ((nPos == -1) || m_nGameGrid[nPos] != 0)
		return;
	//设为EX
	m_nGameGrid[nPos] = EX;
	//下一次轮到OH
	m_nNextChar = OH;

	//这个窗口的dc   建立在站上  程序结束是 dc被回收
	CClientDC dc(this);
	//画X
	DrawX(&dc, nPos);
   //检查游戏是否结束
	CheckForGameOver();
}

//点击右键函数  同左键
void CMainWindow::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_nNextChar != OH)
		return;

	int nPos = GetRectID(point);

	if ((nPos == -1) || m_nGameGrid[nPos] != 0)
		return;

	m_nGameGrid[nPos] = OH;
	m_nNextChar = EX;

	CClientDC dc(this);
	DrawY(&dc, nPos);
	CheckForGameOver();
}

//双击函数
void CMainWindow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	if (dc.GetPixel(point) == RGB(0, 0, 0)) //获取的坐标点的的RGB颜色位黑色  重新开始游戏
		ResetGame();
}


//获得那一个矩形函数
int CMainWindow::GetRectID(CPoint point)
{
	for (int i = 0; i < 9; i++)
	{
		if (m_rcSquared[i].PtInRect(point))  //Rect 类 可以之间判断 point 是否在对应的矩形中
			return i; //返回对应的格子
	}
	return -1;
}

//描边函数
void CMainWindow::DrawBoard(CDC* pDC)
{
	CPen pen(PS_SOLID, 16, RGB(0, 0, 0));

	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(120,16);
	pDC->LineTo(120, 336);

	pDC->MoveTo(232, 16);
	pDC->LineTo(232, 336);

	pDC->MoveTo(16, 120);
	pDC->LineTo(336, 120);

	pDC->MoveTo(16, 232);
	pDC->LineTo(336, 232);

	for (int i = 0; i < 9; i++)
	{
		if (m_nGameGrid[i] == EX)
			DrawX(pDC, i);
		else if (m_nGameGrid[i] == OH)
			DrawY(pDC, i);

	}
	pDC->SelectObject(pOldPen);
}

//画X的函数
void CMainWindow::DrawX(CDC* pDC, int nPos)
{   //设置画笔的属性
	CPen pen(PS_SOLID, 5, RGB(255, 0, 0));
	
	//
	CPen* pOldPen = pDC->SelectObject(&pen);

	CRect rect = m_rcSquared[nPos];

	rect.DeflateRect(16, 16);

	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.bottom);

	pDC->MoveTo(rect.left, rect.bottom);
	pDC->LineTo(rect.right, rect.top);

	pDC->SelectObject(pOldPen);
}

//画O的函数
void CMainWindow::DrawY(CDC* pDC, int nPos)
{   //创建一个新的画笔
	CPen pen(PS_SOLID, 5, RGB(100,100,100));
	//写入新画笔属性并将老画笔返回
	CPen* pOldPen = pDC->SelectObject(&pen);
	 //不使用系统填充刷
	pDC->SelectStockObject(NULL_BRUSH);

	CRect rect = m_rcSquared[nPos];
	rect.DeflateRect(16, 16);

	pDC->Ellipse(rect);
	//将老画笔属性写入
	pDC->SelectObject(pOldPen);
}


//检查游戏是否结束
void CMainWindow::CheckForGameOver()
{

	int nWinner;
	
	if (nWinner = IsWinner())
	{
		CString string = (nWinner == EX) ?
			_T("X wins!") : _T("O wins!");
		MessageBox(string, _T("Game OVer"), MB_ICONEXCLAMATION | MB_OK);
		ResetGame();
	}
	else if (IsDraw()) {
		MessageBox(_T("It's a draw"), _T("Game OVer"), MB_ICONEXCLAMATION | MB_OK);
		ResetGame();
	}
}

//谁赢了
int CMainWindow::IsWinner()
{   //这里使用枚举  连成一条执行
	static int nPattern[8][3] = {
		0,1,2,
		3,4,5,
		6,7,8,
		0,3,6,
		1,4,7,
		2,5,8,
		0,4,8,
		2,4,6
	};
	for (int i = 0; i < 8; i++)
	{
		if ((m_nGameGrid[nPattern[i][0]] == EX) &&
			(m_nGameGrid[nPattern[i][1]] == EX) &&
			(m_nGameGrid[nPattern[i][2]] == EX))
			return EX;
		if ((m_nGameGrid[nPattern[i][0]] == OH) &&
			(m_nGameGrid[nPattern[i][1]] == OH) &&
			(m_nGameGrid[nPattern[i][2]] == OH))
			return OH;
	}
	return 0;
}

BOOL CMainWindow::IsDraw()
{
	for (int i = 0; i < 9; i++)
	{
		if (m_nGameGrid[i] == 0)
			return FALSE;
	}
	return TRUE;
}
void CMainWindow::ResetGame()
{
	m_nNextChar = EX;
	// ：： 表示windows的API
	::ZeroMemory(m_nGameGrid, 9 * sizeof(int));
	Invalidate();
}