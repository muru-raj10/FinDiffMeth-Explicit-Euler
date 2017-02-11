// TestBSPDE1.cpp
//
// Testing 1 factor BS model.
//
// (C) Datasim Education BV 2005-2011
//
//Required to add the folder "Murugaraj Odiathevar Level 9 HW submission" under additional
//include libraries under project properties, VC++Directories

#include "FdmDirector.hpp"

#include <iostream>
#include <string>
using namespace std;

#include "UtilitiesDJD/ExcelDriver/ExcelMechanisms.hpp"

namespace BS // Black Scholes
{
	double sig = 0.4;
	double K = 120.0;
	double T = 1.5;
	double r = 0.04;
	double D = 0.0; // aka q = cont div yield. b=r-q (standard) 

	double mySigma (double x, double t)
	{

		double sigmaS = sig*sig;

		return 0.5 * sigmaS * x * x;	//Diffusion term
	}

	double myMu (double x, double t)
	{
		
		return (r - D) * x;				//Convection term
	
	}

	double myB (double x, double t)
	{	
	
		return  -r;						//reaction term
	}

	double myF (double x, double t)
	{
		return 0.0;
	}

	double myBCL (double t)		
	{
		// Put
		return K *exp(-r * t);		//Boundary condition

		//call
		// return 0.0;
	}

	double myBCR (double t)
	{
			
		// Put
		return 0.0; // P
		
		//Call
		// return 2*BS::K;		//Change this function to return the appropriate S_max.
	}

	double myIC (double x)
	{ // Payoff 
	
		// Put
		return max(K - x, 0.0);

		//call
		//return max(x - K, 0.0);
	}

}


int main()
{
	using namespace ParabolicIBVP;

	// Assignment of functions
	sigma = BS::mySigma;
	mu = BS::myMu;
	b = BS::myB;
	f = BS::myF;
	BCL = BS::myBCL;
	BCR = BS::myBCR;
	IC = BS::myIC;
								//Change to 1000-1. If mesh size is too big(i.e. less mesh points), it won't work.
	int J= 2*BS::K; int N = 20000; // k = O(h^2) !!!!!!!!! J = NS=Space,h=change in S, N=NT=time,k=change it t 
	//[0,T] is divided by N.
	//[0,S_max] is divided by J.

	double Smax = 2*BS::K;			// Magix

	cout << "start FDM\n";
	FDMDirector fdir(Smax, BS::T, J, N);		//N is the nbr of mesh points in time = J*J, where J is number of mesh points in space

	fdir.doit();
	
	cout << "Finished\n";

	// Have you Excel installed (ExcelImports.cpp)
	printOneExcel(fdir.xarr, fdir.current(), string("Value"));

	return 0;
}
/*
You want mesh size(i.e. each square) to be small

Note that for Batch 4, time to maturity T is bigger than the other batches. So we need
to divide [0,T] by a much bigger N. The convergence is also much slower here compared to 
other batches.

To get more accurate answers, we need to increase both J and N. The accuracy increases 
faster when you increase J. To get the same accuracy for a corresponding increase in J,
one needs a huge increase in N. i.e to say, if we increase J/N, we will see a greater
convergence. The smaller the mesh size in both S and t, the more accurate it will be. 
So increase J, increase N with increase in J/N and decrease S_max if possible 
for more accurate results. N should be at O(j^2).

With a huge T, be it Montecarlo or FDM, the convergence will definitely be slower. 
*/