// ProcessingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Processing.h"
#include "ProcessingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
		// Save Current Windows
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessingDlg dialog

CProcessingDlg::CProcessingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProcessingDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessingDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessingDlg message handlers

BOOL CProcessingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	int k,t=0;
	char name[50]={0,};

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// Initialize a program
	HSI_PreProcessing hsi_pre;
	HSI_Processing hsi;
	HSI_PostProcessing hsi_post; 

	hsi_pre.LoadCubeImage();	

	Width = Nx, Height = Ny;
	
	m_pImage=new CBaseImage;
	SIZE size;
	size.cx=Width,size.cy=Height;
	SetWindowPos(NULL,0,0,size.cx,size.cy+30,SWP_NOMOVE|SWP_SHOWWINDOW); 

	//	to save current windows
	CClientDC dc(this);	
	dcBuf.CreateCompatibleDC(&dc);

	// Read a plane
	// Allocate a plane memory
/*	plane = new unsigned int *[Nx];
	if(plane==NULL) exit(-1);

	for(int i=0;i<Nx;i++) {
		plane[i] = new unsigned int [Ny];
		if(plane[i]==NULL) exit(-1);
	}
	for(int k=0;k<Nz;k++) {
		// Copy a plane in a cube
		for(int i=0;i<Nx;i++)
			for(int j=0;j<Ny;j++)				
				plane[i][j] = hsi_pre.cube[i][j][k];  

		sprintf(name,"scene1_%02d.bmp",k+1);
		SavePlaneImage(name,plane);
	} */

	FILE *fp;
	fp = fopen("bands.txt","wt");

	// Load Basic Library
	hsi.LoadBasicLibrary();

	for(t=0;t<Nt;t++) {	
		// Save bands
		for(k=0;k<Ne;k++)	fprintf(fp,"%d ",hsi.bs[k]);
		fprintf(fp,"\n");	

		////////////////////////////////////////////////////
		//                    Processing                  //
		////////////////////////////////////////////////////	
		hsi.Processing(hsi_pre.cube);

		////////////////////////////////////////////////////
		//                  Post-Processing               //
		////////////////////////////////////////////////////	
		// Save Target
		hsi_post.get_target(hsi.endmember);

		char name_target[50];
		sprintf(name_target,"target_%02d.bmp",t);
		SavePlaneImage(name_target,hsi_post.target);  
		// show the image
		dc.BitBlt(0,0,Width,Height,&dcBuf,0,0,SRCCOPY);  
	}

	fclose(fp);  

	count=0;
	SetTimer(0,200,NULL);
		
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProcessingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CProcessingDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CClientDC dc(this);
		dc.BitBlt(0,0,Nx,Ny,&dcBuf,0,0,SRCCOPY);

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CProcessingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CProcessingDlg::SavePlaneImage(char *name,unsigned int **plane)
{
	int i,m,n;

	CClientDC dc(this);	
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,Width,Height);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bitmap);

	m_pImage=new CBaseImage;

	// Allocate Buffer
	m_pImage->m_pDib = new char[(Width*Height)*3];
	memset(m_pImage->m_pDib, 0, (Width*Height)*3); 

	// if current windwos is saved
//	CDC dcBuf;
//	dcBuf.CreateCompatibleDC(&dc);
	CBitmap bitmap_buffer;

	bitmap_buffer.CreateCompatibleBitmap(&dc,Width,Height);
	dcBuf.SelectObject(&bitmap_buffer);

	//	BITMAPINFOHEADER 
	SIZE size;
	size.cx=Width,size.cy=Height;

	int Colornum=0;  // no Palette
	m_pImage->AllocBmpInfo(Colornum);
	m_pImage->SetBmpInfo(size,Colornum);  // no palette in True Color

	// BITMAPFILEHEADER 
	m_pImage->SetBmpHeader(size,Colornum); 

	// Copy Image
	i=0;
	for(m=0;m<Height;m++)
		for(n=0;n<Width;n++) {
			m_pImage->m_pDib[i++]=plane[n][m];
			m_pImage->m_pDib[i++]=plane[n][m];
			m_pImage->m_pDib[i++]=plane[n][m];
		}

	SetStretchBltMode(dcBuf, COLORONCOLOR);
	SetDIBitsToDevice(
			dcBuf,          // hDC
			0,				// DestX 출력할 DC 의 시작 X 좌표
			0,				// DestY 출력할 DC 의 시작 Y 좌표
			Width,			// nDestWidth DIB 의 가로 크기
			Height,			// nDestHeight DIB 의 세로 크기
			0,				// SrcX  DIB 의 출력 시작 X 좌표
			0,				// SrcY  DIB 의 출력 시작 Y 좌표
			0,               // nStartScan 배열에서 처음 스캔할 위치
			Height,			// nNumScans
			m_pImage->m_pDib,          // lpBits 
			m_pImage->pBitmapInfo,     // lpBitsInfo
			DIB_RGB_COLORS); // wUsage

//	dc.BitBlt(0,0,Width,Height,&dcBuf,0,0,SRCCOPY);

	char fn[50];
	sprintf(fn,"%s",name);
	m_pImage->SaveImage(fn); 

	delete m_pImage->m_pDib;  
	dcMem.SelectObject(pOldBitmap);
	ReleaseDC(&dcMem);
}

void CProcessingDlg::OnTimer(UINT nIDEvent) 
{
	char str[50]={0,};

    if(nIDEvent==0) {
			m_pImage=new CBaseImage;
			sprintf(str,"target_%02d.bmp",count++);  //PDVD_000.BMP
			if(count==Nt) count = 0;

			m_pImage->LoadBmp(str);

			CClientDC dc(this);	
			m_pImage->PaintImage(0,0,dc);
	}
	CDialog::OnTimer(nIDEvent);
}

void CProcessingDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	KillTimer(0);	
}