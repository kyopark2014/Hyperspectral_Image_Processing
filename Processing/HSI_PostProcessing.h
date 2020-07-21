// HSI_PostProcessing.h: interface for the HSI_PostProcessing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSI_POSTPROCESSING_H__F3A277A2_EC9C_4765_94E9_7645DD227206__INCLUDED_)
#define AFX_HSI_POSTPROCESSING_H__F3A277A2_EC9C_4765_94E9_7645DD227206__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "size.h"

class HSI_PostProcessing  
{
public:
	unsigned int **target;

	HSI_PostProcessing();
	virtual ~HSI_PostProcessing();

	void get_background(bool **back_index);
	void get_target(bool ***endmember);
};

#endif // !defined(AFX_HSI_POSTPROCESSING_H__F3A277A2_EC9C_4765_94E9_7645DD227206__INCLUDED_)
