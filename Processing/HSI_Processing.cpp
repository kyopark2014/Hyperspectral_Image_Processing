// HSI_Processing.cpp: implementation of the HSI_Processing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Processing.h"
#include "HSI_Processing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HSI_Processing::HSI_Processing()
{
	initialize();
}

void HSI_Processing::initialize(void)
{
	int i,j,k;

	int gap,nb;

	if(Ne==Nz) {
		for(k=0;k<Ne;k++) {
			bs[k] = k;
		}
	}
	else {
		gap = (int)(Nz/(double)(Ne-1));
		nb = 0;

		bs[0] = nb;
		for(k=1;k<Ne;k++) {
			nb += gap;
			if(nb>Nz-1) nb = Nz-1;

			bs[k] = nb;
		}	
	}

	// Background samples for spectrum
	bksamples = new unsigned int *[Nbs];
	if(bksamples==NULL) exit(-1);
	for(i=0;i<Nbs;i++) {
		bksamples[i] = new unsigned int [Nz];
		if(bksamples[i]==NULL) exit(-1);
	}
	for(i=0;i<Nbs;i++)
		for(k=0;k<Nz;k++)	bksamples[i][k]=0; 

	// Background candidate for spectrum
	bk_candidates = new unsigned int *[Nbs];
	if(bk_candidates==NULL) exit(-1);
	for(i=0;i<Nbs;i++) {
		bk_candidates[i] = new unsigned int [Nz];
		if(bk_candidates[i]==NULL) exit(-1);
	}
	for(i=0;i<Nbs;i++)
		for(k=0;k<Nz;k++)	bk_candidates[i][k]=0; 

	// Background samples for position
	bksamples_pos = new bool *[Nx];
	if(bksamples_pos==NULL) exit(-1);
	for(i=0;i<Nx;i++) {
		bksamples_pos[i] = new bool [Ny];
		if(bksamples_pos[i]==NULL) exit(-1);
	}	
	for(i=0;i<Nx;i++)
		for(j=0;j<Ny;j++)	bksamples_pos[i][j]=false; 

	// Target samples
	tgsamples = new unsigned int *[Nlib];
	if(tgsamples==NULL) exit(-1);
	for(i=0;i<Nlib;i++) {
		tgsamples[i] = new unsigned int [Nz];
		if(tgsamples[i]==NULL) exit(-1);
	}
	for(i=0;i<Nlib;i++)
		for(k=0;k<Nz;k++)	tgsamples[i][k]=0; 

	// Candidates of Taget sample
	tg_candidates = new unsigned int **[Nlib];
	if(tg_candidates==NULL) exit(-1);
	for(i=0;i<Nlib;i++) {
		tg_candidates[i] = new unsigned int *[Nts];
		if(tg_candidates[i]==NULL) exit(-1);

		for(j=0;j<Nts;j++) {
			tg_candidates[i][j] = new unsigned int [Nz];
			if(tg_candidates[i][j]==NULL) exit(-1);
		}
	}
	for(i=0;i<Nlib;i++)
		for(j=0;j<Nts;j++)
			for(k=0;k<Nz;k++)	tg_candidates[i][j][k]=0; 

	// the memory for library with effective bands from database
	lib = new unsigned int *[Nlib];
	if(lib==NULL) exit(-1);
	for(i=0;i<Nlib;i++) {
		lib[i] = new unsigned int [Nz];
		if(lib[i]==NULL) exit(-1);
	}	
	for(i=0;i<Nlib;i++)
		for(j=0;j<Nz;j++)	lib[i][j]=0; 

	lib_e = new unsigned int *[Nlib];
	if(lib_e==NULL) exit(-1);
	for(i=0;i<Nlib;i++) {
		lib_e[i] = new unsigned int [Ne];
		if(lib_e[i]==NULL) exit(-1);
	}	
	for(i=0;i<Nlib;i++)
		for(j=0;j<Ne;j++)	lib_e[i][j]=0; 

	lib_basic = new unsigned int *[Nlib];
	if(lib_basic==NULL) exit(-1);
	for(i=0;i<Nlib;i++) {
		lib_basic[i] = new unsigned int [Nz];
		if(lib_basic[i]==NULL) exit(-1);
	}	
	for(i=0;i<Nlib;i++)
		for(j=0;j<Nz;j++)	lib_basic[i][j]=0; 

	// Target Index 
	endmember = new bool **[Nx];
	if(endmember==NULL) exit(-1);
	for(i=0;i<Nx;i++) {
		endmember[i] = new bool *[Ny];
		if(endmember[i]==NULL) exit(-1);

		for(j=0;j<Ny;j++) {
			endmember[i][j] = new bool [Nlib];
			if(endmember[i][j]==NULL) exit(-1);
		}
	}

	for(i=0;i<Nx;i++)
		for(j=0;j<Ny;j++)
			for(k=0;k<Nlib;k++)	endmember[i][j][k]=true; 

	// Background Index
	background = new bool *[Nx];
	if(background ==NULL) exit(-1);
	for(i=0;i<Nx;i++) {
		background[i] = new bool [Ny];
		if(background[i]==NULL) exit(-1);
	}
	for(i=0;i<Nx;i++)
		for(j=0;j<Ny;j++)	background[i][j]=false; 

}

HSI_Processing::~HSI_Processing()
{
	finalize();
}

void HSI_Processing::finalize(void)
{
	int i,j,l;

	// Target Index - endmember
	for(i=0;i<Nx;i++) {
		for(j=0;j<Ny;j++) delete endmember[i][j];
		delete endmember[i];
	}
	delete endmember;

	// Background Index
	for(i=0;i<Nx;i++) delete background[i];
	delete background;

	for(i=0;i<Nbs;i++) delete bksamples[i],bk_candidates[i];
	delete bksamples,bk_candidates;

	for(i=0;i<Nlib;i++) {
		for(j=0;j<Nts;j++) 	delete tg_candidates[i][j];
		delete tg_candidates[i];
	}
	delete tg_candidates;

	// library
	for(l=0;l<Nlib;l++) delete lib[l],lib_e[l],lib_basic[l],tgsamples[l];
	delete lib,lib_e,lib_basic,tgsamples;
}

#define randomize() (srand(time(0)))
#define random(x) (rand() %x)

void HSI_Processing::load_library_for_processing(void)
{
	int l,k;

	for(l=0;l<Nlib;l++) {
		for(k=0;k<Ne;k++) {
			lib_e[l][k] = lib[l][bs[k]];  
		}
	} 
}

void HSI_Processing::Processing(unsigned int ***cube)
{
	int i,j,k,l;
	unsigned int t[Ne]={0,};
	unsigned int pixel[Nz]={0,};
	double A;
	bool b;

	// External Memory
	//		cube, bksample, tgsamples, tg_candidates, lib
	// Internal Memory
	//		sample, t, lib_e, bs

	// Step0 - Load library
	load_library_for_processing();

	// Step0 - Choose the position of background samples randomly but they are just candidates.
	get_position_of_background_samples();
	
	order_of_bk_candidate=0;
	for(l=0;l<Nlib;l++) 
		order_of_tg_candidate[l] = 0;

	for(i=0;i<Nx;i++) {		
		for(j=0;j<Ny;j++) {
			// Step0 - Load samples
			for(k=0;k<Nz;k++)	
				pixel[k]=cube[i][j][k]; 

			// Step0 - Store background samples
			if(bksamples_pos[i][j]==true) { 
				for(int k=0;k<Nz;k++) { 
					bk_candidates[order_of_bk_candidate][k] = pixel[k];  
				}
				order_of_bk_candidate++;
			}

			// Step0 - Save effective bands of a pixel
			for(k=0;k<Ne;k++)	
				t[k]=pixel[bs[k]]; // Internal Memory

			// Step0 - initialize the tag which is for background detection
			b = false;

			for(l=0;l<Nlib;l++) {
				// Step1
				A = get_correlation(t,lib_e[l]);
				
				// Step2
				if(A>=At) { // 10%
					// Target Index(endmember)
					endmember[i][j][l] = true; 

					// Target samples
					if(order_of_tg_candidate[l]<Nts) {
						for(k=0;k<Nz;k++) {
							tg_candidates[l][order_of_tg_candidate[l]][k] = pixel[k];  // External Memory
						}
						order_of_tg_candidate[l]++;
					}

				}
				else {  // 90%
					// Target Index(endmember)
					endmember[i][j][l] = false; // External Memory
					
					if(A<Ab) 
						b=true;	
					else 
						b=false;
				}  
			}

			// Step2
			if(b==true) // Background Index
				background[i][j] = true;
			else
				background[i][j] = false; 
		}
	}

	// Step3 - Choose Target Samples
	int pos;
	for(l=0;l<Nlib;l++) {
		// get target candidate
		pos = random(order_of_tg_candidate[l]);

		// make sample
		for(k=0;k<Nz;k++) {	
			tgsamples[l][k]=tg_candidates[l][pos][k];
		}
	}

	// Step3 - Choose Background Samples
	order_of_bksample=0, order_of_bk_candidate=0;
	for(i=0;i<Nx;i++) {
		for(j=0;j<Ny;j++) {
			if(bksamples_pos[i][j]==1) {
				if(background[i][j]==1) {
					for(k=0;k<Nz;k++) {
						bksamples[order_of_bksample][k] = bk_candidates[order_of_bk_candidate][k];
					}
					order_of_bk_candidate++;
				}
				order_of_bksample++;
			}
		}
	}

	// Step4 - Library refinement
	do_library_refinement(tgsamples,order_of_tgsample);

	// Step5 - Get effective bands
	get_effective_bands(bksamples,order_of_bksample);  
}

void HSI_Processing::LoadBasicLibrary()
{
	int l,k;

	// Read libraries from txt-file
	FILE *fp;
	fp = fopen("scene1_lib.txt","rt");

	char buf[6]={0,};
	for(l=0;l<Nlib;l++) {
		for(k=0;k<Nz;k++) {
			buf[0]=NULL;
			buf[1]=NULL;
			buf[2]=NULL;
			buf[3]=NULL;
			buf[4]=NULL;
			buf[5]=NULL;
			for(int t=0;t<6;t++) {
				buf[t]=fgetc(fp);
			}
			sscanf(buf,"%6d",&lib_basic[l][k]);
		}
		fgetc(fp);
	}
	fclose(fp);

	for(l=0;l<Nlib;l++)
		for(k=0;k<Nz;k++) 
			lib[l][k] = lib_basic[l][k];
}

void HSI_Processing::get_position_of_background_samples(void) //---------------- Step 0
{
	int i,j,k,l;
	
	// initialize background sample
	for(i=0;i<Nx;i++)
		for(j=0;j<Ny;j++)	
			bksamples_pos[i][j]=0;

	// the position of background samples
	for(k=0;k<Nbs;k++) 
		bksamples_pos[random(Nx)][random(Ny)] = true;
	
	order_of_bksample=0;
	for(l=0;l<Nlib;l++) 
		order_of_tgsample[l]=0;
}

//------ Step4
void HSI_Processing::do_library_refinement(unsigned int **tgsamples,unsigned int order_of_tg_candidate[Nlib])  
{
	int l,k;
	unsigned int t[Nz]={0,};
	unsigned int r[Nz]={0,};
	double u=0,d1=0,d2=0,d=0;
	double A=0;

	for(l=0;l<Nlib;l++) {	
		if(order_of_tg_candidate[l]>0) {
			// Load data of a spectrum
			for(k=0;k<Nz;k++)	
				t[k]=tgsamples[l][k];
			// Load a library
			for(k=0;k<Nz;k++)	
				r[k]=lib_basic[l][k];
		
			for(k=0;k<Nz;k++) {
				u = u+t[k]*r[k];
				d1 = d1+t[k]*t[k];
				d2 = d2+r[k]*r[k];
			}         
			d = sqrt(d1)*sqrt(d2); 
			
			if(d==0) A = 1;
			else A = 1.0-acos(u/d);
       
			if(A>=At) 
				for(k=0;k<Nz;k++)
					lib[l][k] = t[k];
			else 
				for(k=0;k<Nz;k++)	
					lib[l][k] = lib_basic[l][k]; 
		}
		else {  // when target was not detected, go back to lib_basic
			for(k=0;k<Nz;k++)	
				lib[l][k] = lib_basic[l][k];
		}
	}
}

// --------- Step5
void HSI_Processing::get_effective_bands(unsigned int **bksamples,unsigned int order_of_bksamples)  
{
	int l,k;
	double diff;
	double e_k;
	double c[Nz]={0,};

	// get contribution
	for(k=0;k<Nz;k++) {
		e_k = 0;
		for(l=0;l<Nlib;l++) {		
			diff=0;
			for(unsigned int i=0;i<order_of_bksamples;i++) {
				diff += (lib[l][k]-bksamples[i][k]);
			}
		
			e_k += fabs(diff)/(order_of_bksamples+1.0);
		}
		c[k] = e_k/(Nlib+1.0); 
	}

	// Get effective bends
	int max_b=0,min_b=0;	
	for(k=1;k<Nz;k++) {
		if(c[k]>c[max_b])	max_b = k;
	    if(c[k]<c[min_b])	min_b = k;
	}

	bs[0] = min_b;
	bs[Ne-1] = max_b;

	double c_b;  // the contribution of a effective band
	int pos,t;
	for(k=1;k<Ne-1;k++) {
		c_b = (c[max_b]-c[min_b])/(Ne-1.0)*k+c[min_b];

		pos=0;
		for(t=1;t<Nz;t++) {
			if(fabs(c[t]-c_b) < fabs(c[pos]-c_b))
	            pos = t;
		}
		bs[k] = pos;
	}
}

double HSI_Processing::get_correlation(unsigned int t[Ne],unsigned int r[Ne])
{
	double u=0,d1=0,d2=0;
	double A=0,d=0;
	int k;

	for(k=0;k<Ne;k++) {
		u = u+t[k]*r[k];
		d1 = d1+t[k]*t[k];
		d2 = d2+r[k]*r[k];
	}

	d = sqrt(d1)*sqrt(d2); 
         
	if(d==0) A = 1;
	else A = 1.0-acos(u/d);

	return A;
}