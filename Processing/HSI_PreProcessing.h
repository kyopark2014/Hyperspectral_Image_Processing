// HSI_PreProcessing.h: interface for the HSI_PreProcessing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSI_PREPROCESSING_H__F7DBE213_26C1_401D_85B9_E35B3A5E10E5__INCLUDED_)
#define AFX_HSI_PREPROCESSING_H__F7DBE213_26C1_401D_85B9_E35B3A5E10E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "size.h"

class HSI_PreProcessing  
{
public:
	unsigned int ***cube;

	HSI_PreProcessing();
	virtual ~HSI_PreProcessing();

	void LoadCubeImage(void);
};

#endif // !defined(AFX_HSI_PREPROCESSING_H__F7DBE213_26C1_401D_85B9_E35B3A5E10E5__INCLUDED_)
