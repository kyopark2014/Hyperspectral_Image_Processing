// ProcessingDlg.h : header file
//

#if !defined(AFX_PROCESSINGDLG_H__09BF07B7_D918_483A_A10F_C77F5F3CC6A1__INCLUDED_)
#define AFX_PROCESSINGDLG_H__09BF07B7_D918_483A_A10F_C77F5F3CC6A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CProcessingDlg dialog

#include "size.h"

class CProcessingDlg : public CDialog
{
// Construction
public:
	CProcessingDlg(CWnd* pParent = NULL);	// standard constructor

	unsigned int **plane;
	void SavePlaneImage(char *name,unsigned int **plane);

	CBaseImage *m_pImage;	
	int Width,Height;
//	CDC dcBuffer;
	CDC dcBuf;
	int count;

// Dialog Data
	//{{AFX_DATA(CProcessingDlg)
	enum { IDD = IDD_PROCESSING_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CProcessingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSINGDLG_H__09BF07B7_D918_483A_A10F_C77F5F3CC6A1__INCLUDED_)
