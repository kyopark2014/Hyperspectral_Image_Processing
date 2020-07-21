// EdgeDetectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EdgeDetector.h"
#include "EdgeDetectorDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDC dcbuf;

/////////////////////////////////////////////////////////////////////////////
// CEdgeDetectorDlg dialog

CEdgeDetectorDlg::CEdgeDetectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEdgeDetectorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEdgeDetectorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEdgeDetectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdgeDetectorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdgeDetectorDlg, CDialog)
	//{{AFX_MSG_MAP(CEdgeDetectorDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdgeDetectorDlg message handlers

BOOL CEdgeDetectorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	// Image 
	m_pImage=new CBaseImage;
	m_pImage->LoadBmp("d:\\hyper\\fig31.bmp");
//	m_pImage->LoadBmp("d:\\hyper\\edge31.bmp");
//	m_pImage->LoadBmp("d:\\hyper\\btestpattern.bmp");

	// Windows size
	SIZE size;
	size = m_pImage->GetSize();
	Width=size.cx, Height=size.cy; // Picture Window
	SetWindowPos(NULL,0,0,size.cx,size.cy,SWP_NOMOVE|SWP_SHOWWINDOW);

	// Detect Edge
	edge = (int **)malloc(sizeof(int)*Height);
	if(edge==NULL) exit(-1);
	for(int i=0;i<Height;i++) {
		edge[i] = (int *)malloc(sizeof(int)*Width);
		if(edge[i]==NULL) exit(-1);
	}

//	ExtreactRGB();
//	EdgeDetector("d:\\hyper\\fig31.bmp");
//	EdgeDetector("d:\\hyper\\edge31.bmp");
	EdgeDetector("d:\\hyper\\btestpattern.bmp");
//	DetectEdgeInAllOfImage();

	// Save Windows DC
	CClientDC dc(this);	
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,Height,Width);
	dcbuf.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcbuf.SelectObject(&bitmap);
	dcbuf.BitBlt(0,0,Height,Width,&dc,0,0,SRCCOPY);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEdgeDetectorDlg::OnPaint() 
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
		dc.BitBlt(0,0,Height,Width,&dcbuf,0,0,SRCCOPY);

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEdgeDetectorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEdgeDetectorDlg::EdgeDetector(char fname[50])
{
	int i=0,j=0,m=0,n=0,t=0;

	int **image;
	// Read Image data - base image
	image = (int **)malloc(sizeof(int)*Height);
	if(image==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		image[i] = (int *)malloc(sizeof(int)*Width);
		if(image[i]==NULL) exit(-1);
	}

	LoadImage(image,Height,Width,fname);

	// bluring
	double sigma = 1;
	double cof = 0.01;
	double x[3],y[3];
	double mask[3][3];

	// define mask using gausian
	int **bimage;
	bimage = (int **)malloc(sizeof(int)*Height);
	if(bimage==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		bimage[i] = (int *)malloc(sizeof(int)*Width);
		if(bimage[i]==NULL) exit(-1);
	} 

	for(i=0;i<3;i++) {
		x[i] = cof*(i-1);
		y[i] = cof*(i-1);
	}

	for(i=0;i<3;i++) {
		for(j=0;j<3;j++) {
			mask[i][j] = 1/(2*3.141592*sigma*sigma) \
				*exp(-(x[i]*x[i]+y[j]*y[j])/(2*sigma*sigma));
		}
	}
	
	double bval;
	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			bval = 0;
			for(i=-1;i<3-1;i++) 
				for(j=-1;j<3-1;j++) {
					bval += (image[m+i][n+j]*mask[1+i][1+j]);
				}
			bimage[m][n] = (int)bval;
		}
	} 

	for(m=0;m<Height;m++) {
		for(n=0;n<Width;n++) {
			bimage[m][n] = image[m][n];
		}
	} 

	// texture edge detection
#define DNO 255
#define D0  0
#define D45 30
#define D90 60
#define D135 90
#define D180 120
#define D225 150
#define D270 180
#define D315 210
#define D360 240

	int **timage;
	int dif=2;
	
	timage = (int **)malloc(sizeof(int)*Height);
	if(timage==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		timage[i] = (int *)malloc(sizeof(int)*Width);
		if(timage[i]==NULL) exit(-1);
	}

	for(m=0;m<Height;m++)
		for(n=0;n<Width;n++) timage[m][n]=0;  

	int dl,dr;
	i=0,j=0;
	for(m=1;m<Height-1;m++) {
		j=0;
		for(n=1;n<Width-1;n++) {
			if((bimage[m-1][n+1]-bimage[m+1][n-1])>dif) dl=1;
			else if((bimage[m-1][n+1]-bimage[m+1][n-1])<dif) dl=-1;
			else dl=0;

			if((bimage[m+1][n+1]-bimage[m-1][n-1])>dif) dr=1;
			else if((bimage[m+1][n+1]-bimage[m-1][n-1])<dif) dr=-1;
			else dr=0;

			if(dl==0 && dr==0) timage[i][j]=DNO;
			else if(dl== 1 && dr== 1) timage[i][j]=D0;  // deg 0
			else if(dl== 1 && dr== 0) timage[i][j]=D45; // deg 45
			else if(dl== 1 && dr==-1) timage[i][j]=D90; // deg 90
			else if(dl== 0 && dr==-1) timage[i][j]=D135; // deg 135
			else if(dl==-1 && dr==-1) timage[i][j]=D180; // deg 180
			else if(dl==-1 && dr== 0) timage[i][j]=D225; // deg 225
			else if(dl==-1 && dr== 1) timage[i][j]=D270; // deg 270
			else if(dl== 0 && dr== 1) timage[i][j]=D315; // deg 315
			j++;
		}
		i++;
	} 

	int **cimage; // Compensated image
	
	cimage = (int **)malloc(sizeof(int)*Height);
	if(cimage==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		cimage[i] = (int *)malloc(sizeof(int)*Width);
		if(cimage[i]==NULL) exit(-1);
	}

	for(m=0;m<Height;m++)
		for(n=0;n<Width;n++) cimage[m][n]=0;

	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			if(timage[m][n]==D0) {
				if((timage[m-1][n]==D0 || timage[m-1][n]==D45 || timage[m-1][n]==D315) && 
					(timage[m-1][n-1]==D0 || timage[m-1][n-1]==D45 || timage[m-1][n-1]==D315) && 
					(timage[m-1][n+1]==D0 || timage[m-1][n+1]==D45 || timage[m-1][n+1]==D315))
					cimage[m][n]=0;  
				else cimage[m][n]=timage[m][n];
			}
			else if(timage[m][n]==D45) {
				if((timage[m][n-1]==D0 || timage[m][n-1]==D45 || timage[m][n-1]==D90) && 
					(timage[m+1][n-1]==D0 || timage[m+1][n-1]==D45 || timage[m+1][n-1]==D90) && 
					(timage[m+1][n]==D0 || timage[m+1][n]==D45 || timage[m+1][n]==D90))
					cimage[m][n]=0;  
				else cimage[m][n]=timage[m][n];
			}
			else if(timage[m][n]==D90) {
				if((timage[m+1][n-1]==D45 || timage[m+1][n-1]==D90 || timage[m+1][n-1]==D135) && 
					(timage[m][n]==D45 || timage[m][n]==D90 || timage[m][n]==D135) && 
					(timage[m-1][n+1]==D45 || timage[m-1][n+1]==D90 || timage[m-1][n+1]==D135))
					cimage[m][n]=0;  
				else cimage[m][n]=timage[m][n];
			}
			else if(timage[m][n]==D135) {
				if((timage[m+1][n]==D90 || timage[m+1][n]==D135 || timage[m+1][n]==D180) && 
					(timage[m+1][n+1]==D90 || timage[m+1][n+1]==D135 || timage[m+1][n+1]==D180) && 
					(timage[m][n+1]==D90 || timage[m][n+1]==D135 || timage[m][n+1]==D180))
					cimage[m][n]=0;  
				else cimage[m][n]=timage[m][n];
			}
			else if(timage[m][n]==D180) {
				if((timage[m-1][n+1]==D135 || timage[m-1][n+1]==D180 || timage[m-1][n+1]==D225) && 
					(timage[m][n+1]==D135 || timage[m][n+1]==D180 || timage[m][n+1]==D225) && 
					(timage[m+1][n+1]==D135 || timage[m+1][n+1]==D180 || timage[m+1][n+1]==D225))
					cimage[m][n]=0;  
				else cimage[m][n]=timage[m][n];
			}
			else if(timage[m][n]==D225) {
				if((timage[m-1][n]==D180 || timage[m-1][n]==D225 || timage[m-1][n]==D270) && 
					(timage[m-1][n+1]==D180 || timage[m-1][n+1]==D225 || timage[m-1][n+1]==D270) && 
					(timage[m][n+1]==D180 || timage[m][n+1]==D225 || timage[m][n+1]==D270))
					cimage[m][n]=0;  
				else cimage[m][n]=timage[m][n];
			}
			else if(timage[m][n]==D270) {
				if((timage[m-1][n-1]==D225 || timage[m-1][n-1]==D270 || timage[m-1][n-1]==D315) && 
					(timage[m-1][n]==D225 || timage[m-1][n]==D270 || timage[m-1][n]==D315) && 
					(timage[m-1][n+1]==D225 || timage[m-1][n+1]==D270 || timage[m-1][n+1]==D315))
					cimage[m][n]=0;  
				else cimage[m][n]=timage[m][n];
			}
			else if(timage[m][n]==D315) {
				if((timage[m][n-1]==D270 || timage[m][n-1]==D315 || timage[m][n-1]==D0) && 
					(timage[m-1][n-1]==D270 || timage[m-1][n-1]==D315 || timage[m-1][n-1]==D0) && 
					(timage[m-1][n]==D270 || timage[m-1][n]==D315 || timage[m-1][n]==D0))
					cimage[m][n]=0;  
				else cimage[m][n]=timage[m][n];
			}
			else cimage[m][n]=0;
		}
	} 

	RGBSaveImage(cimage,Height,Width,"timage.bmp");

// Display edge detected image
/*	LoadColorImage();

	int **Rd,**Gd,**Bd;

	Rd = (int **)malloc(sizeof(int)*Height);
	if(Rd==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		Rd[i] = (int *)malloc(sizeof(int)*Width);
		if(Rd[i]==NULL) exit(-1);
	}
	Gd = (int **)malloc(sizeof(int)*Height);
	if(Gd==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		Gd[i] = (int *)malloc(sizeof(int)*Width);
		if(Gd[i]==NULL) exit(-1);
	}
	Bd = (int **)malloc(sizeof(int)*Height);
	if(Bd==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		Bd[i] = (int *)malloc(sizeof(int)*Width);
		if(Bd[i]==NULL) exit(-1);
	}
	for(m=0;m<Height;m++) 
		for(n=0;n<Width;n++) {
			if(edge[m][n]==255) {
				Bd[m][n]=255;
				Gd[m][n]=255;
				Rd[m][n]=255;
			}
			else {
				Bd[m][n]=Bc[m][n];
				Gd[m][n]=Gc[m][n];
				Rd[m][n]=Rc[m][n];
			}
		}
	
	RGBSaveImage(Rd,Gd,Bd,"d:\\hyper\\edgedetectedimage.bmp");

	for(i=0;i<Height;i++) 	free(Rd[i]),free(Gd[i]),free(Bd[i]);
	free(Rd),free(Gd),free(Bd);

	for(i=0;i<Height;i++) 	free(Rc[i]),free(Gc[i]),free(Bc[i]);
	free(Rc),free(Gc),free(Bc);

	for(i=0;i<Height;i++) 
		free(dedge[i]),free(dedgex[i]),free(dedgey[i]),free(imdata[i]),free(bimage[i]);
	free(dedge),free(dedgex),free(dedgey),free(imdata),free(bimage); */

	for(i=0;i<Height;i++) free(image[i]), free(bimage[i]), free(timage[i]), free(cimage[i]);
	free(image), free(bimage), free(timage), free(cimage);
}

void CEdgeDetectorDlg::ExtreactRGB()
{
	int i=0,m=0,n=0,t=0;
	char FigurePathB[50] = "d:\\hyper\\fig05.BMP";
	char FigurePathG[50] = "d:\\hyper\\fig15.BMP";
	char FigurePathR[50] = "d:\\hyper\\fig25.BMP";

	CClientDC dc(this);	
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,Width,Height);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bitmap);

	m_pImage=new CBaseImage;

	// B
	m_pImage->LoadBmp(FigurePathB);
	m_pImage->PaintImage(0,0,dcMem);
	dc.BitBlt(0,0,Width,Height,&dcMem,0,0,SRCCOPY);

	B = (int **)malloc(sizeof(int)*Height);
	if(B==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		B[i] = (int *)malloc(sizeof(int)*Width);
		if(B[i]==NULL) exit(-1);
	}
	
	COLORREF pcolor;
	i=0;
	for(m=0;m<Height;m++) {
		for(n=0;n<Width;n++) {
			pcolor = dcMem.GetPixel(n,Height-1-m);

			B[m][n]=GetBValue(pcolor);
		}
	} 

	// G
	m_pImage->LoadBmp(FigurePathG);
	m_pImage->PaintImage(0,0,dcMem);
	dc.BitBlt(0,0,Width,Height,&dcMem,0,0,SRCCOPY);

	G = (int **)malloc(sizeof(int)*Height);
	if(G==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		G[i] = (int *)malloc(sizeof(int)*Width);
		if(G[i]==NULL) exit(-1);
	}
	
//	COLORREF pcolor;
	i=0;
	for(m=0;m<Height;m++) {
		for(n=0;n<Width;n++) {
			pcolor = dcMem.GetPixel(n,Height-1-m);

			G[m][n]=GetGValue(pcolor);
		}
	} 

	// R
	m_pImage->LoadBmp(FigurePathR);
	m_pImage->PaintImage(0,0,dcMem);
	dc.BitBlt(0,0,Width,Height,&dcMem,0,0,SRCCOPY);

	R = (int **)malloc(sizeof(int)*Height);
	if(R==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		R[i] = (int *)malloc(sizeof(int)*Width);
		if(R[i]==NULL) exit(-1);
	}
	
//	COLORREF pcolor;
	i=0;
	for(m=0;m<Height;m++) {
		for(n=0;n<Width;n++) {
			pcolor = dcMem.GetPixel(n,Height-1-m);

			R[m][n]=GetRValue(pcolor);
		}
	} 

	//////////////////////////////////////////////
	dcMem.SelectObject(pOldBitmap);
	ReleaseDC(&dcMem);

	m_pImage->m_pDib = new char[(Width*Height)*3];
	memset(m_pImage->m_pDib, 0, (Width*Height)*3); 

	CDC dcBuffer;
	dcBuffer.CreateCompatibleDC(&dc);

	CBitmap bitmap_buffer;

	i=0;
	for(m=0;m<Height;m++) 
		for(n=0;n<Width;n++) {
			m_pImage->m_pDib[i++]=B[m][n];
			m_pImage->m_pDib[i++]=G[m][n];
			m_pImage->m_pDib[i++]=R[m][n]; 
		}

	bitmap_buffer.CreateCompatibleBitmap(&dc,Width,Height);
	dcBuffer.SelectObject(&bitmap_buffer);

	//	BITMAPINFOHEADER 
	SIZE size;
	size.cx=Width,size.cy=Height;

	int Colornum=0;  // no Palette
	m_pImage->AllocBmpInfo(Colornum);
	m_pImage->SetBmpInfo(size,Colornum);  // no palette in True Color

	// BITMAPFILEHEADER 
	m_pImage->SetBmpHeader(size,Colornum); 

	SetStretchBltMode(dcBuffer, COLORONCOLOR);
	SetDIBitsToDevice(
			dcBuffer,                 // hDC
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

	dc.BitBlt(0,0,Width,Height,&dcBuffer,0,0,SRCCOPY);

////////////////////////////////////////////////////////////////////////////////
// Save Image
////////////////////////////////////////////////////////////////////////////////
	//	BITMAPINFOHEADER 
/*	int Colornum=0;  // no Palette
	m_pImage->AllocBmpInfo(Colornum);
	m_pImage->SetBmpInfo(size,Colornum);  // no palette in True Color

	// BITMAPFILEHEADER 
	m_pImage->SetBmpHeader(size,Colornum); 

	// Get RGB data 
	m_pImage->m_pDib= new char [m_pImage->pBitmapInfo->bmiHeader.biSizeImage];
	GetDIBits(
		(HDC)dcMem,				// Handle to DC
		(HBITMAP)bitmap,		// Handle to bitmap
		0,                      // first Scan line to set
		Height,					// number of scan lines to copy
		m_pImage->m_pDib,       // array for bitmap bits
		m_pImage->pBitmapInfo,	// bitmap data buffer
		DIB_RGB_COLORS			// RGB index
	);	*/

	// Write RGB image to BMP
	m_pImage->SaveImage("d:\\hyper\\fullcolor.bmp"); 

	delete m_pImage->m_pDib;  
	dcMem.SelectObject(pOldBitmap);
	ReleaseDC(&dcMem);
}

void CEdgeDetectorDlg::LoadColorImage()
{
	int i,m,n;

	CClientDC dc(this);	
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,Width,Height);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bitmap);

	// load color image
	m_pImage=new CBaseImage;
	m_pImage->LoadBmp("d:\\hyper\\bfullcolor.bmp");

	m_pImage->PaintImage(0,0,dcMem);
	dc.BitBlt(0,0,Width,Height,&dcMem,0,0,SRCCOPY);

	// R G R
	Rc = (int **)malloc(sizeof(int)*Height);
	if(Rc==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		Rc[i] = (int *)malloc(sizeof(int)*Width);
		if(Rc[i]==NULL) exit(-1);
	}
	Gc = (int **)malloc(sizeof(int)*Height);
	if(Gc==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		Gc[i] = (int *)malloc(sizeof(int)*Width);
		if(Gc[i]==NULL) exit(-1);
	}
	Bc = (int **)malloc(sizeof(int)*Height);
	if(Bc==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		Bc[i] = (int *)malloc(sizeof(int)*Width);
		if(Bc[i]==NULL) exit(-1);
	}

	COLORREF pcolor;
	i=0;
	for(m=0;m<Height;m++) {
		for(n=0;n<Width;n++) {
			pcolor = dcMem.GetPixel(n,Height-1-m);

			Rc[m][n]=GetRValue(pcolor);
			Gc[m][n]=GetGValue(pcolor);
			Bc[m][n]=GetBValue(pcolor);
		}
	} 

	dcMem.SelectObject(pOldBitmap);
	ReleaseDC(&dcMem);
}

void CEdgeDetectorDlg::LoadImage(int **data,int Hegiht,int Width,char fname[50])
{
	int i,m,n;

	CClientDC dc(this);	
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,Width,Height);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bitmap);

	COLORREF pcolor;
	// load image
	m_pImage=new CBaseImage;

	m_pImage->LoadBmp(fname);

	// show image
	m_pImage->PaintImage(0,0,dcMem);
	dc.BitBlt(0,0,Width,Height,&dcMem,0,0,SRCCOPY);

	i=0;
	for(m=0;m<Height;m++) {
		for(n=0;n<Width;n++) {
			pcolor = dcMem.GetPixel(n,Height-1-m);

			data[m][n]=GetBValue(pcolor);
		}
	} 

	dcMem.SelectObject(pOldBitmap);
	ReleaseDC(&dcMem);
}

/*
void CEdgeDetectorDlg::FindEdge(int fnum)
{
	int i=0,j=0,m=0,n=0,t=0;

	// Read Image data
	int **data;
	data = (int **)malloc(sizeof(int)*Height);
	if(data==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		data[i] = (int *)malloc(sizeof(int)*Width);
		if(data[i]==NULL) exit(-1);
	}

	char fname[50];
	sprintf(fname,"d:\\hyper\\fig%02d.bmp",fnum);
	LoadImage(data,fname);

	// Bluring for noise elumination
	// 3by3 mask
	double mask[3][3];
	double x[3],y[3];

	// define mask using gausian
	for(i=0;i<3;i++) {
		x[i] = cof*(i-1);
		y[i] = cof*(i-1);
	}

	for(i=0;i<3;i++) {
		for(j=0;j<3;j++) {
			mask[i][j] = 1/(2*3.141592*sigma*sigma) \
				*exp(-(x[i]*x[i]+y[j]*y[j])/(2*sigma*sigma));
		}
	}

	// bluring
	bimage = (int **)malloc(sizeof(int)*Height);
	if(bimage==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		bimage[i] = (int *)malloc(sizeof(int)*Width);
		if(bimage[i]==NULL) exit(-1);
	} 
	
	double bval;
	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			bval = 0;
			for(i=-1;i<3-1;i++) 
				for(j=-1;j<3-1;j++) {
					bval += (data[m+i][n+j]*mask[1+i][1+j]);
				}
			bimage[m][n] = (int)bval;
		}
	}   

	// Detect Edge
	int **dedge;
	dedge = (int **)malloc(sizeof(int)*Height);
	if(dedge==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		dedge[i] = (int *)malloc(sizeof(int)*Width);
		if(dedge[i]==NULL) exit(-1);
	}

	int dedgex,dedgey;
	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			dedgex = -bimage[m-1][n-1]-2*bimage[m-1][n]-bimage[m-1][n+1] \
				+bimage[m+1][n-1]+2*bimage[m+1][n]+bimage[m+1][n+1];
			dedgey = -bimage[m-1][n-1]-2*bimage[m][n-1]-bimage[m+1][n-1] \
				+bimage[m-1][n+1]+2*bimage[m][n+1]+bimage[m+1][n+1];
			dedge[m][n] = abs(dedgex)+abs(dedgey);
		}
	}

	//  Threshold
	int **timage;
	timage = (int **)malloc(sizeof(int)*Height);
	if(timage==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		timage[i] = (int *)malloc(sizeof(int)*Width);
		if(timage[i]==NULL) exit(-1);
	}

	for(m=0;m<Height;m++) {
		for(n=0;n<Width;n++) {
			if(dedge[m][n]>Th) timage[m][n] = 255;
			else timage[m][n] = 0;
		}
	}

	//  edge diffusion
	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			if(timage[m][n]==255) {
				dedge[m][n] = 255;
//				dedge[m-1][n] = 255;
//				dedge[m][n-1] = 255;
//				dedge[m+1][n] = 255;
//				dedge[m][n+1] = 255;
			}
		}
	} 

	sprintf(fname,"d:\\hyper\\edge%02d.bmp",fnum);
	RGBSaveImage(dedge,dedge,dedge,fname);

	// save edge 
	for(m=0;m<Height;m++) 
		for(n=0;n<Width;n++) {
			if(dedge[m][n]==255) edge[m][n]=255;
		}

	for(i=0;i<Height;i++) 	free(data[i]),free(dedge[i]),free(bimage[i]);
	free(data),free(dedge),free(bimage);
	for(i=0;i<Height;i++) 	free(timage[i]);
	free(timage); 
} */

void CEdgeDetectorDlg::DetectEdgeInAllOfImage()
{
	int i,m,n;

	LoadColorImage();
//	for(i=1;i<=31;i++)	FindEdge(i);

	// Save
	int **Rd,**Gd,**Bd;

	Rd = (int **)malloc(sizeof(int)*Height);
	if(Rd==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		Rd[i] = (int *)malloc(sizeof(int)*Width);
		if(Rd[i]==NULL) exit(-1);
	}
	Gd = (int **)malloc(sizeof(int)*Height);
	if(Gd==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		Gd[i] = (int *)malloc(sizeof(int)*Width);
		if(Gd[i]==NULL) exit(-1);
	}
	Bd = (int **)malloc(sizeof(int)*Height);
	if(Bd==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		Bd[i] = (int *)malloc(sizeof(int)*Width);
		if(Bd[i]==NULL) exit(-1);
	}
	
	for(m=0;m<Height;m++) 
		for(n=0;n<Width;n++) {
			if(edge[m][n]==255) {
				Bd[m][n]=255;
				Gd[m][n]=255;
				Rd[m][n]=255;
			}
			else {
				Bd[m][n]=Bc[m][n];
				Gd[m][n]=Gc[m][n];
				Rd[m][n]=Rc[m][n];
			}
		} 

	RGBSaveImage(Rd,Gd,Bd,"d:\\hyper\\edgedetectedimage.bmp");

	for(i=0;i<Height;i++) 	free(Rd[i]),free(Gd[i]),free(Bd[i]);
	free(Rd),free(Gd),free(Bd);

	for(i=0;i<Height;i++) 	free(Rc[i]),free(Gc[i]),free(Bc[i]);
	free(Rc),free(Gc),free(Bc);
}

void CEdgeDetectorDlg::RGBSaveImage(int **Rd,int **Gd,int **Bd,char fname[50])
{
	int i,m,n;

	CClientDC dc(this);	
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,Width,Height);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bitmap);

	m_pImage=new CBaseImage;

	// Save Image
	m_pImage->m_pDib = new char[(Width*Height)*3];
	memset(m_pImage->m_pDib, 0, (Width*Height)*3); 

	CDC dcBuffer;
	dcBuffer.CreateCompatibleDC(&dc);
	CBitmap bitmap_buffer;

	bitmap_buffer.CreateCompatibleBitmap(&dc,Width,Height);
	dcBuffer.SelectObject(&bitmap_buffer);

	//	BITMAPINFOHEADER 
	SIZE size;
	size.cx=Width,size.cy=Height;

	int Colornum=0;  // no Palette
	m_pImage->AllocBmpInfo(Colornum);
	m_pImage->SetBmpInfo(size,Colornum);  // no palette in True Color

	// BITMAPFILEHEADER 
	m_pImage->SetBmpHeader(size,Colornum); 

	// edge show
	i=0;
	for(m=0;m<Height;m++) 
		for(n=0;n<Width;n++) {
			m_pImage->m_pDib[i++]=Bd[m][n];
			m_pImage->m_pDib[i++]=Gd[m][n];
			m_pImage->m_pDib[i++]=Rd[m][n];
		}

	SetStretchBltMode(dcBuffer, COLORONCOLOR);
	SetDIBitsToDevice(
			dcBuffer,                 // hDC
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

	dc.BitBlt(0,0,Width,Height,&dcBuffer,0,0,SRCCOPY);

	m_pImage->SaveImage(fname); 

	delete m_pImage->m_pDib;  
	dcMem.SelectObject(pOldBitmap);
	ReleaseDC(&dcMem); 
}

void CEdgeDetectorDlg::RGBSaveImage(int **image,int Height,int Width,char fname[50])
{
	int i,m,n;

	CClientDC dc(this);	
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,Width,Height);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bitmap);

	m_pImage=new CBaseImage;

	// Save Image
	m_pImage->m_pDib = new char[(Width*Height)*3];
	memset(m_pImage->m_pDib, 0, (Width*Height)*3); 

	CDC dcBuffer;
	dcBuffer.CreateCompatibleDC(&dc);
	CBitmap bitmap_buffer;

	bitmap_buffer.CreateCompatibleBitmap(&dc,Width,Height);
	dcBuffer.SelectObject(&bitmap_buffer);

	//	BITMAPINFOHEADER 
	SIZE size;
	size.cx=Width,size.cy=Height;

	int Colornum=0;  // no Palette
	m_pImage->AllocBmpInfo(Colornum);
	m_pImage->SetBmpInfo(size,Colornum);  // no palette in True Color

	// BITMAPFILEHEADER 
	m_pImage->SetBmpHeader(size,Colornum); 

	// edge show
	i=0;
	for(m=0;m<Height;m++) 
		for(n=0;n<Width;n++) {
			m_pImage->m_pDib[i++]=image[m][n];
			m_pImage->m_pDib[i++]=image[m][n];
			m_pImage->m_pDib[i++]=image[m][n];
		}

	SetStretchBltMode(dcBuffer, COLORONCOLOR);
	SetDIBitsToDevice(
			dcBuffer,                 // hDC
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

	dc.BitBlt(0,0,Width,Height,&dcBuffer,0,0,SRCCOPY);

	m_pImage->SaveImage(fname); 

	delete m_pImage->m_pDib;  
	dcMem.SelectObject(pOldBitmap);
	ReleaseDC(&dcMem); 
}
