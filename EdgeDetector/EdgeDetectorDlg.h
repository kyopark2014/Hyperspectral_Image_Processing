// EdgeDetectorDlg.h : header file
//

#if !defined(AFX_EDGEDETECTORDLG_H__20DAC2DB_F466_4ED5_9173_53B7FCE61F8A__INCLUDED_)
#define AFX_EDGEDETECTORDLG_H__20DAC2DB_F466_4ED5_9173_53B7FCE61F8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseImage.h"

/////////////////////////////////////////////////////////////////////////////
// CEdgeDetectorDlg dialog

class CEdgeDetectorDlg : public CDialog
{
// Construction
public:
	void RGBSaveImage(int **Rd,int **Gd,int **Bd,char fname[50]);
	void DetectEdgeInAllOfImage();
	void FindEdge(int fnum);
	void LoadImage(int **data,char fname[50]);
	void LoadColorImage();
	void ExtreactRGB();
	int **R,**G,**B;
	int **Rc,**Gc,**Bc;
	int **imdata;
	int **edge,**bimage;
	int Width,Height;
	double cof;
	
	void EdgeDetector(char fname[50]);
	CBaseImage *m_pImage;
	
	CEdgeDetectorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CEdgeDetectorDlg)
	enum { IDD = IDD_EDGEDETECTOR_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdgeDetectorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEdgeDetectorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDGEDETECTORDLG_H__20DAC2DB_F466_4ED5_9173_53B7FCE61F8A__INCLUDED_)
