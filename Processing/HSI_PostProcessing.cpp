// HSI_PostProcessing.cpp: implementation of the HSI_PostProcessing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Processing.h"
#include "HSI_PostProcessing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HSI_PostProcessing::HSI_PostProcessing()
{
	int i,j;

	// Allocate target
	target = new unsigned int *[Nx];
	if(target==NULL) exit(-1);

	for(i=0;i<Nx;i++) {
		target[i] = new unsigned int [Ny];
		if(target[i]==NULL) exit(-1);
	}
	
	for(i=0;i<Nx;i++)
		for(j=0;j<Ny;j++)	target[i][j]=0; 
}

HSI_PostProcessing::~HSI_PostProcessing()
{
	int i;

	for(i=0;i<Nx;i++) 	delete target[i];
	delete target;
}

void HSI_PostProcessing::get_target(bool ***endmember)
{
	int i,j,l;
	bool tag;
	
	for(i=0;i<Nx;i++) {
		for(j=0;j<Ny;j++) {
			tag = false;

			for(l=0;l<Nlib;l++) 
				if(endmember[i][j][l]==true) tag = true;

			if(tag==true) 	target[i][j]=255;
			else			target[i][j]=0;
		}
	}
}

