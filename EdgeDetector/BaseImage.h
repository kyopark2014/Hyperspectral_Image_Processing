// BaseImage.h: interface for the CBaseImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEIMAGE_H__0D8E8F3C_2961_11D2_8B72_006097AD9DDC__INCLUDED_)
#define AFX_BASEIMAGE_H__0D8E8F3C_2961_11D2_8B72_006097AD9DDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PALVERSION 0x300
#define DIB_HEADER_MARKER ((WORD)('M'<<8) | 'B')

class CBaseImage  
{
public:
	BITMAPFILEHEADER bmpHeader;
	LPBITMAPINFO pBitmapInfo;
	LPSTR m_pDib;
	LPSTR tDib;
	int m_ndumy;
	int m_nColor;
	LPLOGPALETTE lpLogPalette;
	CPalette *cPalette;
	char m_strFileName[512];
	int count;
public:
	CBitmap *m_pBitmap;
	CBaseImage();
	virtual ~CBaseImage();

public:
	BOOL SetBmpHeader(SIZE size, int colornum);
	BOOL ImageToDC(CDC* dcMem,CBitmap* bitmap,SIZE size,char *str);
	void MakeBitmap();
	BOOL SaveImage(LPCSTR *file=NULL);
	/*
	Create
	size ũ���� ������ colornum�� ������ ����� backcolor�� �����.
	*/
	BOOL Create(SIZE size, int colornum,COLORREF backcolor);
	void Create(int Width, int Height, COLORREF * pPal);
	/* 
	GetSize:
	�̹����� ũ�⸦ �ȴ�.
	*/
	SIZE GetSize();
	/*
	PaintImage
	ȭ�鿡 �̹����� ����Ѵ�.
	*/
	BOOL  PaintImage(int x, int y, HDC hDC);
	/*
	LoadBmp
	filename���� bmp�� �ε��Ѵ�.
	*/
	BOOL LoadBmp(LPCSTR filename);
	/*
	SaveImage
	bmp ������ �����Ѵ�.
	*/
	BOOL SaveImage(LPCSTR file);
	//���ϸ� ���
	LPCSTR GetFileName(){return m_strFileName;}
	//���� Į��� ���
	int GetColorNum(){	return m_nColor;}
	//�ȷ�Ʈ ���
	CPalette * GetPalette(){return cPalette;}
	LPBITMAPINFO GetBitmapInfo(){return pBitmapInfo;};


public:
	/*
	SetBmpInfo:
		szBitmapInfo�� ���� �����ϴ� �Լ�
		������: 16Į�� 2Į�� ���� ���� 4�� 8���� ���� �̹��� �����ȵ�
	*/
	BOOL SetBmpInfo(SIZE size, int colornum);
	/*
	AllocBmpInfo
	pBitmapInfo�� �޸𸮿� �����ϴ� �Լ�
	*/
	BOOL AllocBmpInfo(int colornum);
	/*
	CreatePalette
	mode�� 1�̸� pBitmapInfo ����
	�׷��� ������ ����
	*/

	BOOL CreatePalette(BOOL mode);
	/*
	FindColorInPalette
	�ȷ�Ʈ �ȿ��� fi �� ���� �ִ°��� ã�´�.
	version up: ����� ������ ����
	*/
	int FindColorInPalette(COLORREF fi);
	/*
	SetSystemPalette
	�����쿡 �����Ǿ��ִ� �ȷ�Ʈ�� ����Ѵ�.
	*/
	BOOL SetSystemPalette(HDC hDC);

};

#endif // !defined(AFX_BASEIMAGE_H__0D8E8F3C_2961_11D2_8B72_006097AD9DDC__INCLUDED_)
