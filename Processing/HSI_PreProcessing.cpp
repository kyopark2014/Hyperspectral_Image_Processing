// HSI_PreProcessing.cpp: implementation of the HSI_PreProcessing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Processing.h"
#include "HSI_PreProcessing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HSI_PreProcessing::HSI_PreProcessing()
{
	int i,j,k;

	// Allocate a cube memory
	cube = new unsigned int **[Nx];
	if(cube==NULL) exit(-1);
	for(i=0;i<Nx;i++) {
		cube[i] = new unsigned int *[Ny];
		if(cube[i]==NULL) exit(-1);

		for(j=0;j<Ny;j++) {
			cube[i][j] = new unsigned int [Nz];
			if(cube[i][j]==NULL) exit(-1);
		}
	}
	for(i=0;i<Nx;i++)
		for(j=0;j<Ny;j++)
			for(k=0;k<Nz;k++)	cube[i][j][k]=0; 
}

HSI_PreProcessing::~HSI_PreProcessing()
{
	int i,j;

	// cube
	for(i=0;i<Nx;i++) {
		for(j=0;j<Ny;j++) delete cube[i][j];
		delete cube[i];
	}
	delete cube;
}

void HSI_PreProcessing::LoadCubeImage()
{
	int i,j,k;
	
	// Read a cube from txt-file
	FILE *fp;
	
//	fp = fopen("d:\\project\\HSI\\MakeCubeImage\\cubedata.txt","rt");
	fp = fopen("scene1.txt","rt");
//	fp = fopen("scene1_s.txt","rt");

	char buf[4]={0,};
	
	for(i=0;i<Nx;i++) {
		for(j=0;j<Ny;j++) {					
			for(k=0;k<Nz;k++) {
				buf[0]=NULL;
				buf[1]=NULL;
				buf[2]=NULL;
				buf[3]=NULL;
				for(int t=0;t<4;t++) {
					buf[t]=fgetc(fp);
				}
				sscanf(buf,"%4d",&cube[i][j][k]);				
			}
			fgetc(fp);
		}
		fgetc(fp);
	}
	fclose(fp);
}
