// Processing.h : main header file for the PROCESSING application
//

#if !defined(AFX_PROCESSING_H__BB080D69_1F74_46C2_B972_94E7882BD2EF__INCLUDED_)
#define AFX_PROCESSING_H__BB080D69_1F74_46C2_B972_94E7882BD2EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "HSI_PreProcessing.h"
#include "HSI_Processing.h"
#include "HSI_PostProcessing.h"

/////////////////////////////////////////////////////////////////////////////
// CProcessingApp:
// See Processing.cpp for the implementation of this class
//

class CProcessingApp : public CWinApp
{
public:
	CProcessingApp();

	HSI_Processing hsi;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CProcessingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSING_H__BB080D69_1F74_46C2_B972_94E7882BD2EF__INCLUDED_)
