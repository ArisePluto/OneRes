#include "input.h"

//����һ��APP ������
MyAPP myapp;

//  ��ʼ�� ��������  Ȼ������ѭ��
BOOL MyAPP::InitInstance()
{    //����һ��������
	m_pMainWnd = new CMainWindow;        //���ݳ�ʼ���������г�ʼ��
	m_pMainWnd->ShowWindow(m_nCmdShow);  //�����ڼ���Ԥ������
	m_pMainWnd->UpdateWindow();          //���������µ�����
	return TRUE; 
}

//���ָ����Ϣ
BEGIN_MESSAGE_MAP(CMainWindow, CWnd)
	ON_WM_PAINT()  //�滭��Ϣ
	ON_WM_LBUTTONDOWN()   //�����
	ON_WM_LBUTTONDBLCLK() //�����
	ON_WM_RBUTTONDOWN()   //�����˫��
END_MESSAGE_MAP()

//�����������εı�  �Ÿ���
const CRect CMainWindow::m_rcSquared[9] = {
	CRect(16,16 ,112,112),  CRect(128,16 ,224,112),  CRect(240,16 ,336,112),
	CRect(16,128,112,224),  CRect(128,128,224,224),  CRect(240,128,336,224),
	CRect(16,240,112,336),  CRect(128,240,224,336),  CRect(240,240,336,336)
};

// ���ڵĳ�ʼ��  
CMainWindow::CMainWindow()
{   // EX ����
	m_nNextChar = EX;
	//�Դ洢����г�ʼ��
	::ZeroMemory(m_nGameGrid, 9 * sizeof(int));
	//ע�ᴰ������
	CString strWndClass = AfxRegisterWndClass(
		CS_DBLCLKS, //����ʽ ���ڲ���
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), //ָ������ʶ������
		(HBRUSH)(COLOR_3DFACE+1),  //������ɫ
		AfxGetApp()->LoadStandardIcon(IDI_WINLOGO)); //����ͼ��ľ��
	  //��������
	    CreateEx(0, strWndClass, _T("MGame"),  //��������
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL);
   //���ô��ڴ�С
	CRect rect(0, 0, 352, 352);
	CalcWindowRect(&rect);

	//���ô�������
	SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);


}

//ɾ����Ķ���
void CMainWindow::PostNcDestroy()
{
	delete this;

}

//�滭����
void CMainWindow::OnPaint()
{
	CPaintDC dc(this);
	//���������߽���
	DrawBoard(&dc);
}

//����������
void CMainWindow::OnLButtonDown(UINT nFlags, CPoint point)
{   // ���۵��Լ���
	if (m_nNextChar != EX)
		return;
	//��ȡ�Ǹ�һ����
	int nPos = GetRectID(point);	
	if ((nPos == -1) || m_nGameGrid[nPos] != 0)
		return;
	//��ΪEX
	m_nGameGrid[nPos] = EX;
	//��һ���ֵ�OH
	m_nNextChar = OH;

	//������ڵ�dc   ������վ��  ��������� dc������
	CClientDC dc(this);
	//��X
	DrawX(&dc, nPos);
   //�����Ϸ�Ƿ����
	CheckForGameOver();
}

//����Ҽ�����  ͬ���
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

//˫������
void CMainWindow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	if (dc.GetPixel(point) == RGB(0, 0, 0)) //��ȡ�������ĵ�RGB��ɫλ��ɫ  ���¿�ʼ��Ϸ
		ResetGame();
}


//�����һ�����κ���
int CMainWindow::GetRectID(CPoint point)
{
	for (int i = 0; i < 9; i++)
	{
		if (m_rcSquared[i].PtInRect(point))  //Rect �� ����֮���ж� point �Ƿ��ڶ�Ӧ�ľ�����
			return i; //���ض�Ӧ�ĸ���
	}
	return -1;
}

//��ߺ���
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

//��X�ĺ���
void CMainWindow::DrawX(CDC* pDC, int nPos)
{   //���û��ʵ�����
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

//��O�ĺ���
void CMainWindow::DrawY(CDC* pDC, int nPos)
{   //����һ���µĻ���
	CPen pen(PS_SOLID, 5, RGB(100,100,100));
	//д���»������Բ����ϻ��ʷ���
	CPen* pOldPen = pDC->SelectObject(&pen);
	 //��ʹ��ϵͳ���ˢ
	pDC->SelectStockObject(NULL_BRUSH);

	CRect rect = m_rcSquared[nPos];
	rect.DeflateRect(16, 16);

	pDC->Ellipse(rect);
	//���ϻ�������д��
	pDC->SelectObject(pOldPen);
}


//�����Ϸ�Ƿ����
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

//˭Ӯ��
int CMainWindow::IsWinner()
{   //����ʹ��ö��  ����һ��ִ��
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
	// ���� ��ʾwindows��API
	::ZeroMemory(m_nGameGrid, 9 * sizeof(int));
	Invalidate();
}