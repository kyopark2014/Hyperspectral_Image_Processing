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

/*
#define Th 100
#define sigma 0.5
#define cof 0.02 */
#define Th 70
#define sigma 1
#define cof 1

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
//	m_pImage->LoadBmp("d:\\hyper\\fig31.bmp");
	m_pImage->LoadBmp("d:\\hyper\\edge31.bmp");
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
//	EdgeDetector("d:\\hyper\\btestpattern.bmp");
	DetectEdgeInAllOfImage();

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

	// Read Image data
	imdata = (int **)malloc(sizeof(int)*Height);
	if(imdata==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		imdata[i] = (int *)malloc(sizeof(int)*Width);
		if(imdata[i]==NULL) exit(-1);
	}

	LoadImage(imdata,fname);

// Image bluring - 3by3 mask
	// initilize 3by3 mask
/*	double mask[3][3];
	double x[3],y[3];

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
					bval += (imdata[m+i][n+j]*mask[1+i][1+j]);
				}
			bimage[m][n] = (int)bval;
		}
	} */



	// Bluring for noise elumination
// 5by5 mask
	double mask[5][5];
	double x[5],y[5];

	for(i=0;i<5;i++) {
		x[i] = cof*(i-2);
		y[i] = cof*(i-2);
	}

	for(i=0;i<5;i++) {
		for(j=0;j<5;j++) {
			mask[i][j] = 1/(2*3.141592*sigma*sigma) \
				*exp(-(x[i]*x[i]+y[j]*y[j])/(2*sigma*sigma));
		}
	}

	bimage = (int **)malloc(sizeof(int)*Height);
	if(bimage==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		bimage[i] = (int *)malloc(sizeof(int)*Width);
		if(bimage[i]==NULL) exit(-1);
	} 
	
	double bval;
	for(m=2;m<Height-2;m++) {
		for(n=2;n<Width-2;n++) {
			bval = 0;
			for(i=-2;i<5-2;i++) 
				for(j=-2;j<5-2;j++) {
					bval += (imdata[m+i][n+j]*mask[2+i][2+j]);
				}
			bimage[m][n] = (int)bval;
		}
	} 

	// 5 by 5 mask from books
/*	int mask[5][5];
	mask[0][0]=2,mask[0][1]=4, mask[0][2]=5, mask[0][3]=4, mask[0][4]=2;
	mask[0][0]=4,mask[0][1]=9, mask[0][2]=12,mask[0][3]=9, mask[0][4]=4;
	mask[0][0]=5,mask[0][1]=12,mask[0][2]=15,mask[0][3]=12,mask[0][4]=5;
	mask[0][0]=4,mask[0][1]=9, mask[0][2]=12,mask[0][3]=9, mask[0][4]=4;
	mask[0][0]=2,mask[0][1]=4, mask[0][2]=5, mask[0][3]=4, mask[0][4]=2;

	bimage = (int **)malloc(sizeof(int)*Height);
	if(bimage==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		bimage[i] = (int *)malloc(sizeof(int)*Width);
		if(bimage[i]==NULL) exit(-1);
	}
	
	int bval;
	for(m=2;m<Height-2;m++) {
		for(n=2;n<Width-2;n++) {
			bval = 0;
			for(i=-2;i<5-2;i++) 
				for(j=-2;j<5-2;j++) {
					bval += (imdata[m+i][n+j]*mask[2+i][2+j])/115;
				}
			bimage[m][n] = bval;
		}
	} */

	// 3by3 mask from books
/*	int mask[3][3];
	mask[0][0]=-1,mask[0][1]=-1, mask[0][2]=-1;
	mask[0][0]=-1,mask[0][1]=8, mask[0][2]=-1;
	mask[0][0]=-1,mask[0][1]=-1, mask[0][2]=-1; // 16

//	mask[0][0]=0,mask[0][1]=-1, mask[0][2]=0;
//	mask[0][0]=-1,mask[0][1]=4, mask[0][2]=-1;
//	mask[0][0]=0,mask[0][1]=-1, mask[0][2]=0;  // 9
	
	bimage = (int **)malloc(sizeof(int)*Height);
	if(bimage==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		bimage[i] = (int *)malloc(sizeof(int)*Width);
		if(bimage[i]==NULL) exit(-1);
	}
	
	int bval;
	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			bval = 0;
			for(i=-1;i<3-1;i++) 
				for(j=-1;j<3-1;j++) {
					bval += (imdata[m+i][n+j]*mask[1+i][1+j]);
				}
			bimage[m][n] = bval;
		}
	}  */

	// No bluring
/*	bimage = (int **)malloc(sizeof(int)*Height);
	if(bimage==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		bimage[i] = (int *)malloc(sizeof(int)*Width);
		if(bimage[i]==NULL) exit(-1);
	}

	for(m=0;m<Height;m++) {
		for(n=0;n<Width;n++) {
			bimage[m][n] = imdata[m][n];
		}
	} */

	RGBSaveImage(bimage,bimage,bimage,"d:\\hyper\\bimage.bmp");

	// Detect Edge
	int **dedge,**dedgex,**dedgey;
	dedge = (int **)malloc(sizeof(int)*Height);
	if(dedge==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		dedge[i] = (int *)malloc(sizeof(int)*Width);
		if(dedge[i]==NULL) exit(-1);
	}
	dedgex = (int **)malloc(sizeof(int)*Height);
	if(dedgex==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		dedgex[i] = (int *)malloc(sizeof(int)*Width);
		if(dedgex[i]==NULL) exit(-1);
	}
	dedgey = (int **)malloc(sizeof(int)*Height);
	if(dedgey==NULL) exit(-1);
	for(i=0;i<Height;i++) {
		dedgey[i] = (int *)malloc(sizeof(int)*Width);
		if(dedgey[i]==NULL) exit(-1);
	}

	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			dedgex[m][n] = -bimage[m-1][n-1]-2*bimage[m-1][n]-bimage[m-1][n+1] \
				+bimage[m+1][n-1]+2*bimage[m+1][n]+bimage[m+1][n+1];
			dedgey[m][n] = -bimage[m-1][n-1]-2*bimage[m][n-1]-bimage[m+1][n-1] \
				+bimage[m-1][n+1]+2*bimage[m][n+1]+bimage[m+1][n+1];
			dedge[m][n] = (abs(dedgex[m][n])+abs(dedgey[m][n]));
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

	// one more edge detection 
	// bluring
/*	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			bval = 0;
			for(i=-1;i<3-1;i++) 
				for(j=-1;j<3-1;j++) bval += (dedge[m+i][n+j]*mask[1+i][1+j]);
			bimage[m][n] = (int)bval;
		}
	} 
	// edge detection
	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			dedgex[m][n] = -bimage[m-1][n-1]-2*bimage[m-1][n]-bimage[m-1][n+1] \
				+bimage[m+1][n-1]+2*bimage[m+1][n]+bimage[m+1][n+1];
			dedgey[m][n] = -bimage[m-1][n-1]-2*bimage[m][n-1]-bimage[m+1][n-1] \
				+bimage[m-1][n+1]+2*bimage[m][n+1]+bimage[m+1][n+1];
			dedge[m][n] = (abs(dedgex[m][n])+abs(dedgey[m][n]));
		}
	}
	// Threshold
	for(m=0;m<Height;m++) {
		for(n=0;n<Width;n++) {
			if(dedge[m][n]>Th) timage[m][n] = 255;
			else timage[m][n] = 0;
		}
	}
	// edge diffusion
	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			if(timage[m][n]==255) {
				dedge[m][n] = 255;
				dedge[m-1][n] = 255;
				dedge[m][n-1] = 255;
				dedge[m+1][n] = 255;
				dedge[m][n+1] = 255;
			}
		}
	} */

	RGBSaveImage(dedgey,dedgey,dedgey,"d:\\hyper\\edgey.bmp");
	RGBSaveImage(dedgex,dedgex,dedgex,"d:\\hyper\\edgex.bmp");

	// save edge 
	for(m=0;m<Height;m++) 
		for(n=0;n<Width;n++) {
			if(dedge[m][n]==255) edge[m][n]=255;
		}
	RGBSaveImage(edge,edge,edge,"d:\\hyper\\edge.bmp");

	for(i=0;i<Height;i++) 	free(timage[i]);
	free(timage); 

// Display edge detected image
	LoadColorImage();

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
	free(dedge),free(dedgex),free(dedgey),free(imdata),free(bimage);
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

void CEdgeDetectorDlg::LoadImage(int **data,char fname[50])
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
//	m_pImage->LoadBmp("d:\\hyper\\edge_ori.bmp");
//	m_pImage->LoadBmp("d:\\hyper\\btestpattern.bmp");

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
	/* 3by3 mask*/
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

/*	// one more edge detection 
	// bluring
	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			bval = 0;
			for(i=-1;i<3-1;i++) 
				for(j=-1;j<3-1;j++) bval += (dedge[m+i][n+j]*mask[1+i][1+j]);
			bimage[m][n] = (int)bval;
		}
	} 
	// edge detection
	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			dedgex = -bimage[m-1][n-1]-2*bimage[m-1][n]-bimage[m-1][n+1] \
				+bimage[m+1][n-1]+2*bimage[m+1][n]+bimage[m+1][n+1];
			dedgey = -bimage[m-1][n-1]-2*bimage[m][n-1]-bimage[m+1][n-1] \
				+bimage[m-1][n+1]+2*bimage[m][n+1]+bimage[m+1][n+1];
			dedge[m][n] = abs(dedgex)+abs(dedgey);
		}
	}
	// Threshold
	for(m=0;m<Height;m++) {
		for(n=0;n<Width;n++) {
			if(dedge[m][n]>Th) timage[m][n] = 255;
			else timage[m][n] = 0;
		}
	}
	// edge diffusion
	for(m=1;m<Height-1;m++) {
		for(n=1;n<Width-1;n++) {
			if(timage[m][n]==255) {
				dedge[m][n] = 255;
				dedge[m-1][n] = 255;
				dedge[m][n-1] = 255;
				dedge[m+1][n] = 255;
				dedge[m][n+1] = 255;
			}
		}
	} */

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
}

void CEdgeDetectorDlg::DetectEdgeInAllOfImage()
{
	int i,m,n;

	LoadColorImage();
	for(i=1;i<=31;i++)	FindEdge(i);

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

