// BaseImage.cpp: implementation of the CBaseImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseImage::CBaseImage()
{
	m_pDib=NULL;
	lpLogPalette=NULL;
	cPalette=NULL;
	pBitmapInfo=NULL;
	tDib=NULL;
	m_pBitmap=NULL;
	count=0;
}

CBaseImage::~CBaseImage()
{
	if(tDib)
		delete tDib;
	else
	{
		if(m_pDib)
			delete m_pDib;
		if(pBitmapInfo)
			delete pBitmapInfo;
	}
	if(lpLogPalette)
		delete lpLogPalette;
	if(cPalette)
		delete cPalette;
}
/*
Create
size 크기의 영상을 colornum의 색으로 배경을 backcolor로 만든다.
*/
BOOL CBaseImage::Create(SIZE size, int colornum,COLORREF backcolor)
{
	int r,g,b,i;
	if(!SetBmpInfo(size,colornum))
		return FALSE;
	if(colornum)
		if(!CreatePalette(FALSE))
			return FALSE;

	if(m_pDib)
		delete m_pDib;
	m_pDib= new char [pBitmapInfo->bmiHeader.biSizeImage];

	switch(m_nColor)
	{
	case 0:
			for(i=0;i<(int)pBitmapInfo->bmiHeader.biSizeImage;)
			{
				b=GetBValue(backcolor);
				m_pDib[i]=b;i++;
				g=GetGValue(backcolor);
				m_pDib[i]=g;i++;
				r=GetRValue(backcolor);
				m_pDib[i]=r;i++;
			}
			break;
	case 256:
			i=FindColorInPalette(backcolor);
			if(i<0)i=0;
			r=(BYTE)i;
			memset(m_pDib,r,pBitmapInfo->bmiHeader.biSizeImage);
			break;
	case 16:
			i=FindColorInPalette(backcolor);
			if(i<0)i=0;
			r=i;r=r<<4;g=r>>4;b=r|g;
			memset(m_pDib,b,pBitmapInfo->bmiHeader.biSizeImage);
			break;
	case 1:
			memset(m_pDib,0,pBitmapInfo->bmiHeader.biSizeImage);
			break;
	}
	return TRUE;
}
/*
LoadBmp
filename으로 bmp를 로드한다.
*/
BOOL CBaseImage::LoadBmp(LPCSTR filename)
{
	strcpy(m_strFileName,filename);
	HANDLE fd=CreateFile(m_strFileName,GENERIC_READ,
			0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fd<0)
		return FALSE;
	DWORD size,len;

	if(!ReadFile(fd,(LPSTR)&bmpHeader,sizeof(bmpHeader),&len,NULL))
		return FALSE;

	//화일이 bmp 일경우에는 화일 처음에 "BM"이라고 설정해있음
	if (bmpHeader.bfType != DIB_HEADER_MARKER)
		return FALSE;

	size=bmpHeader.bfSize-sizeof(bmpHeader);
	tDib= new char[size];
	if(!ReadFile(fd,tDib,size,&len,NULL))
		return FALSE;
	if(len !=size)
		return FALSE;

	CloseHandle(fd);
	pBitmapInfo=(LPBITMAPINFO)tDib;
	switch(pBitmapInfo->bmiHeader.biBitCount)
	{
	case 24:
			m_nColor=0;
			break;
	case 8:
			m_nColor=256;
			break;
	case 4:
			m_nColor=16;
			break;
	case 1:
			m_nColor=2;
			break;
	}
	m_pDib=(tDib + *(LPDWORD)tDib + (m_nColor * sizeof(RGBQUAD)));

	if(m_nColor)
		if(!CreatePalette(TRUE))
			return FALSE;
	return TRUE;
}
/*
SaveImage
bmp 파일을 저장한다.
*/
BOOL CBaseImage::SaveImage(LPCSTR file)
{
	DWORD size,len;
	if(file)
		strcpy(m_strFileName,file);

	HANDLE fd=CreateFile(m_strFileName,GENERIC_READ|GENERIC_WRITE,
			0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(!WriteFile(fd,(LPSTR)&bmpHeader,sizeof(bmpHeader),&len,NULL))
		return FALSE;
	size=((m_nColor-1) * sizeof(RGBQUAD))+sizeof(BITMAPINFO);
	if(!WriteFile(fd,pBitmapInfo,size,&len,NULL))
		return FALSE;
	if(!WriteFile(fd,m_pDib,pBitmapInfo->bmiHeader.biSizeImage,&len,NULL))
		return FALSE;

	CloseHandle(fd);
	return TRUE;
}

/* 
GetSize:
이미지의 크기를 안다.
*/
SIZE CBaseImage::GetSize()
{
	SIZE size;
	size.cx=pBitmapInfo->bmiHeader.biWidth;
	size.cy=pBitmapInfo->bmiHeader.biHeight;
	return size;
}
/*
PaintImage
화면에 이미지를 출력한다.
*/
BOOL  CBaseImage::PaintImage(int x, int y, HDC hDC)
{
	HPALETTE hPal=NULL;          
	HPALETTE hOldPal=NULL;       

	if (m_pDib == NULL)
		return FALSE;


	if (cPalette != NULL)
	{	
		hPal = (HPALETTE) cPalette->m_hObject;
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	    RealizePalette(hDC);
	}
	else
	{
		if(SetSystemPalette(hDC))
		    RealizePalette(hDC);
	}

	::SetStretchBltMode(hDC, COLORONCOLOR);

	::SetDIBitsToDevice(hDC,                    // hDC
								   x,             // DestX
								   y,              // DestY
								   GetSize().cx,        // nDestWidth
								   GetSize().cy,       // nDestHeight
								   0,            // SrcX
								   0,   // SrcY
								   0,                          // nStartScan
								   (WORD)GetSize().cy,  // nNumScans
								   m_pDib,                  // lpBits
								   pBitmapInfo,     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage


	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}
	return TRUE;
}

// Set BmpHeader
BOOL CBaseImage::SetBmpHeader(SIZE size, int colornum)
{
	// bfType
	bmpHeader.bfType=DIB_HEADER_MARKER;
	// bfSize
	bmpHeader.bfSize=sizeof(bmpHeader)+((colornum-1) * sizeof(RGBQUAD))
			+sizeof(BITMAPINFO)+pBitmapInfo->bmiHeader.biSizeImage;
	// bfReserved1 and 2
	bmpHeader.bfReserved1=0;
	bmpHeader.bfReserved2=0;
	// bfOffBits
	if(!colornum)
		bmpHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	else
		bmpHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+((colornum) * sizeof(RGBQUAD));

	return TRUE;
}

/*
SetBmpInfo:
	szBitmapInfo에 값을 설정하는 함수
	버전업: 16칼라 2칼라 각각 가로 4와 8보다 작은 이미지 설정안됨
*/
BOOL CBaseImage::SetBmpInfo(SIZE size, int colornum)
{

	if(!AllocBmpInfo(colornum))
		return FALSE;

	//bmiHeader의 구조체 크기 설정
	pBitmapInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	//이미지 가로크기
	pBitmapInfo->bmiHeader.biWidth=size.cx;
	// 세로크기
	pBitmapInfo->bmiHeader.biHeight=size.cy;
	//비트플렌은 1
	pBitmapInfo->bmiHeader.biPlanes=1;

	switch(colornum)
	{
	case 0:
			pBitmapInfo->bmiHeader.biBitCount=24;
			//가로길이 4배수 나머지 값 얻기
			m_ndumy=(size.cx*3)%4;
			((4-m_ndumy)==4)?m_ndumy=0:m_ndumy=4-m_ndumy;
			pBitmapInfo->bmiHeader.biSizeImage=((size.cx*3)+m_ndumy)*size.cy;
			m_nColor=0;
			break;
	case 256:
			pBitmapInfo->bmiHeader.biBitCount=8;
			//가로길이 4배수 나머지 값 얻기
			m_ndumy=(size.cx)%4;
			((4-m_ndumy)==4)?m_ndumy=0:m_ndumy=4-m_ndumy;
			pBitmapInfo->bmiHeader.biSizeImage=(size.cx+m_ndumy)*size.cy;
			m_nColor=256;
			break;
	case 16:
			pBitmapInfo->bmiHeader.biBitCount=4;
			//가로길이 4배수 나머지 값 얻기
			//가로크기가 1이면 에러 
			if(size.cx<2)
				return FALSE;
			m_ndumy=(size.cx/2)%4;
			((4-m_ndumy)==4)?m_ndumy=0:m_ndumy=4-m_ndumy;
			pBitmapInfo->bmiHeader.biSizeImage=((size.cx/2)+m_ndumy)*size.cy;
			m_nColor=16;
			break;
	case 2:
			pBitmapInfo->bmiHeader.biBitCount=1;
			//가로길이 4배수 나머지 값 얻기
			if(size.cy<8)
				return FALSE;
			m_ndumy=(size.cx/8)%4;
			((4-m_ndumy)==4)?m_ndumy=0:m_ndumy=4-m_ndumy;
			pBitmapInfo->bmiHeader.biSizeImage=((size.cx/8)+m_ndumy)*size.cy;
			m_nColor=2;
			break;
	default:
			return FALSE;
	}
	pBitmapInfo->bmiHeader.biCompression=BI_RGB;
	//차후에 기록할 예정
	pBitmapInfo->bmiHeader.biXPelsPerMeter=0;
	pBitmapInfo->bmiHeader.biYPelsPerMeter=0;
	pBitmapInfo->bmiHeader.biClrUsed =0;
	pBitmapInfo->bmiHeader.biClrImportant =0;

	return TRUE;
}

/*
AllocBmpInfo
pBitmapInfo를 메모리에 설정하는 함수
*/
BOOL CBaseImage::AllocBmpInfo(int colornum)
{
	int size;
	if(pBitmapInfo)
		delete pBitmapInfo;
	size=sizeof(BITMAPINFO)+(sizeof(RGBQUAD)*colornum);
	pBitmapInfo =(LPBITMAPINFO) new char[size];
	if(pBitmapInfo==NULL)
		return FALSE;
	return TRUE;
}

/*
CreatePalette
mode가 1이면 pBitmapInfo 에서
그렇지 않으면 만듬
*/
BOOL CBaseImage::CreatePalette(BOOL mode)
{
	int r,g,b;
	if(cPalette)
		delete cPalette;
	if(lpLogPalette)
		delete lpLogPalette;
	cPalette=NULL;
	lpLogPalette=NULL;

	lpLogPalette=(LPLOGPALETTE) new char[sizeof(LOGPALETTE) + sizeof(PALETTEENTRY)*m_nColor];
	cPalette=new CPalette;
	if(lpLogPalette == NULL|| cPalette==NULL)
		return FALSE;
	lpLogPalette->palVersion=PALVERSION;
	lpLogPalette->palNumEntries=m_nColor;

	if(mode)
	{
		for(int i=0;i<m_nColor;i++)
		{
			lpLogPalette->palPalEntry[i].peRed=pBitmapInfo->bmiColors[i].rgbRed;
			lpLogPalette->palPalEntry[i].peGreen=pBitmapInfo->bmiColors[i].rgbGreen;
			lpLogPalette->palPalEntry[i].peBlue=pBitmapInfo->bmiColors[i].rgbBlue;
		}
	}
	else
	{
		int i=0;
		for(r=0;r<256;r+=51)
			for(g=0;g<256;g+=51)
				for(b=0;b<256;b+=51)
				{
					lpLogPalette->palPalEntry[i].peRed=r;
					pBitmapInfo->bmiColors[i].rgbRed=r;
					lpLogPalette->palPalEntry[i].peGreen=g;
					pBitmapInfo->bmiColors[i].rgbGreen=g;
					lpLogPalette->palPalEntry[i].peBlue=b;
					pBitmapInfo->bmiColors[i].rgbBlue=b;
					i++;
					if(i>m_nColor)
						return TRUE;
				}
	}
	cPalette->CreatePalette(lpLogPalette);
	return TRUE;
}

/*
FindColorInPalette
팔레트 안에서 fi 의 색이 있는가를 찾는다.
version up: 비슷한 색으로 가기
*/
int CBaseImage::FindColorInPalette(COLORREF fi)
{
	for(int i=0;i<m_nColor;i++)
	{
		if( (lpLogPalette->palPalEntry[i].peRed == GetRValue(fi)) &&
			(lpLogPalette->palPalEntry[i].peGreen == GetGValue(fi)) &&
			(lpLogPalette->palPalEntry[i].peBlue == GetBValue(fi)) )
				return i;
	}
	return -1;
}

/*
SetSystemPalette
윈도우에 설정되어있는 팔레트를 사용한다.
*/
BOOL CBaseImage::SetSystemPalette(HDC hDC)
{
	if(m_nColor == 0) return FALSE;
	//현재 트루칼라모드이면 설정안함
	if(!(::GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE)) return FALSE;
	//현재 모드가 몇칼라 모드이며 팔레트엔트리가 몇개인지 확인
	int nSysColors = ::GetDeviceCaps(hDC, NUMCOLORS);
	int nPalEntries = ::GetDeviceCaps(hDC, SIZEPALETTE);

	if(nPalEntries ==0)
		return FALSE;
	
	int nEntries = (nPalEntries == 0) ? nSysColors : nPalEntries;

	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +
		nEntries * sizeof(PALETTEENTRY)];
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = nEntries;
	
	//시스템 팔레트를 얻고
	::GetSystemPaletteEntries(hDC, 0, nEntries,
		(LPPALETTEENTRY) ((LPBYTE) pLogPal + 2 * sizeof(WORD)));
    HPALETTE m_hPalette;
	
	//시스템 팔레트를 설정한다.
	m_hPalette = ::CreatePalette(pLogPal);
	SelectPalette(hDC, m_hPalette, TRUE);
	delete pLogPal;
	return TRUE;
}

void CBaseImage::MakeBitmap()  // for the case of DDB(Device Dependent Bitmap)
{
	if(m_pBitmap)
		delete m_pBitmap;
	m_pBitmap=new CBitmap;
	CDC memDC;     
	CDC ScreenDC;	
	m_pBitmap= new CBitmap;	
	ScreenDC.CreateDC("DISPLAY", NULL, NULL, NULL);
	memDC.CreateCompatibleDC(&ScreenDC); 
	m_pBitmap->CreateCompatibleBitmap(&ScreenDC,GetSize().cx,GetSize().cy);
	memDC.SelectObject(m_pBitmap);
	//스크린 DC의 화면을 메모리 DC에 설정한다.
	PaintImage(0,0,memDC.m_hDC);
}

void CBaseImage::Create(int Width, int Height, COLORREF * pPal)
{
	if(m_pDib)
		delete m_pDib;
	if(pBitmapInfo)
		delete pBitmapInfo;

	SIZE size;
	size.cx=Width;
	size.cy=Height;

	int rWidth = (Width + 3)/4*4;
	m_pDib = new char[rWidth * Height];
	memset(m_pDib, 0, rWidth * Height);

	SetBmpInfo(size,256);

	int i;

	if(cPalette)
		delete cPalette;
	if(lpLogPalette)
		delete lpLogPalette;
	cPalette=NULL;
	lpLogPalette=NULL;

	lpLogPalette=(LPLOGPALETTE) new char[sizeof(LOGPALETTE) + sizeof(PALETTEENTRY)*m_nColor];
	cPalette=new CPalette;
	if(lpLogPalette == NULL|| cPalette==NULL)
		return;
	lpLogPalette->palVersion=PALVERSION;
	lpLogPalette->palNumEntries=m_nColor;

	for(i=0; i<256; i++)
	{
		lpLogPalette->palPalEntry[i].peRed=GetRValue(pPal[i]);
		pBitmapInfo->bmiColors[i].rgbRed=GetRValue(pPal[i]);
		lpLogPalette->palPalEntry[i].peGreen=GetGValue(pPal[i]);
		pBitmapInfo->bmiColors[i].rgbGreen=GetGValue(pPal[i]);
		lpLogPalette->palPalEntry[i].peBlue=GetBValue(pPal[i]);
		pBitmapInfo->bmiColors[i].rgbBlue=GetBValue(pPal[i]);
	}		

	bmpHeader.bfType = 19778;
	bmpHeader.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + sizeof(RGBQUAD) * 256 + rWidth * Height; 
	bmpHeader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + sizeof(RGBQUAD) * 256;
}

BOOL CBaseImage::ImageToDC(CDC* dcMem,CBitmap* bitmap,SIZE size,char *str)
{
	int Width,Height;
	int Colornum;

	Width=size.cx, Height=size.cy;

	Colornum=0;  // no Palette

// BITMAPINFOHEADER 
	if(!AllocBmpInfo(Colornum))
		return FALSE;
	SetBmpInfo(size,Colornum);  // no palette in True Color

// BITMAPFILEHEADER 
	SetBmpHeader(size,Colornum);

// BITMAPDATA
	m_pDib= new char [pBitmapInfo->bmiHeader.biSizeImage];

	GetDIBits(
		(HDC)*dcMem,                  // Handle to DC
		(HBITMAP)*bitmap,					// Handle to bitmap
		0,                      // first Scan line to set
		size.cy,     // number of scan lines to copy
		m_pDib,                 // array for bitmap bits
		pBitmapInfo,			// bitmap data buffer
		DIB_RGB_COLORS			// RGB index
	);

// Write on image file
	SaveImage(str);

	delete m_pDib;
	return TRUE;  
}

