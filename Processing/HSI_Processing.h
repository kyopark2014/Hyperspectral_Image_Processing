// HSI_Processing.h: interface for the HSI_Processing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSI_PROCESSING_H__4540B730_1707_4951_B6CB_08E046EAC407__INCLUDED_)
#define AFX_HSI_PROCESSING_H__4540B730_1707_4951_B6CB_08E046EAC407__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "baseimage.h"
#include "size.h"
#include "math.h"

class HSI_Processing  
{
public:
	HSI_Processing();
	virtual ~HSI_Processing();

	unsigned int **plane;
	unsigned int **lib_e,**lib,**lib_basic;
	
	bool ***endmember;
	bool **background;
	unsigned int bs[Ne];

	bool **bksamples_pos;  // position 
	unsigned int **bksamples,**bk_candidates; // spectrum
	unsigned int **tgsamples,***tg_candidates;
	unsigned int order_of_bksample,order_of_bk_candidate;
	unsigned int order_of_tgsample[Nlib],order_of_tg_candidate[Nlib];

	unsigned int xt_addr[Nlib],yt_addr[Nlib]; // the address of target samples
	
	void Processing(unsigned int ***cube);
	void initialize(void);
	void finalize(void);
	double get_correlation(unsigned int t[Ne],unsigned int r[Ne]);
	void ReadCubeImage(void);
	void LoadBasicLibrary(void);
	void get_samples(void);
	void do_library_refinement(unsigned int **tgsamples,unsigned int order_of_tg_candidate[Nlib]);
	void get_effective_bands(unsigned int **bksamples,unsigned int order_of_bksamples);
	void get_position_of_background_samples(void);
	void load_library_for_processing();
};

#endif // !defined(AFX_HSI_PROCESSING_H__4540B730_1707_4951_B6CB_08E046EAC407__INCLUDED_)
