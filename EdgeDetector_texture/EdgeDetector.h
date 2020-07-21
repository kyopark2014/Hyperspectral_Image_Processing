// EdgeDetector.h : main header file for the EDGEDETECTOR application
//

#if !defined(AFX_EDGEDETECTOR_H__EE06A6C4_263C_4B01_8952_62E30BB2699E__INCLUDED_)
#define AFX_EDGEDETECTOR_H__EE06A6C4_263C_4B01_8952_62E30BB2699E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEdgeDetectorApp:
// See EdgeDetector.cpp for the implementation of this class
//

class CEdgeDetectorApp : public CWinApp
{
public:
	CEdgeDetectorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdgeDetectorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEdgeDetectorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDGEDETECTOR_H__EE06A6C4_263C_4B01_8952_62E30BB2699E__INCLUDED_)
