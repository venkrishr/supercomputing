#include <iostream>
#include <omp.h>
#include <cstdlib>
using namespace std;

#define V 4

#define NA 100000

#define X11 xi, xj
#define X12 xi, xj + newlen
#define X21 xi + newlen, xj
#define X22 xi + newlen, xj + newlen

#define U11 ui, uj
#define U12 ui, uj + newlen
#define U21 ui + newlen, uj
#define U22 ui + newlen, uj + newlen

#define V11 vi, vj
#define V12 vi, vj + newlen
#define V21 vi + newlen, vj
#define V22 vi + newlen, vj + newlen

#define PARALLEL 1

void iterFW (int **graph, int length, int xi, int xj, int ui, int uj, int vi, int vj)
{
	for (int k = uj; k < uj + length; k++)
	{
		for (int i = xi; i < xi + length; i++)
		{
			for (int j = xj; j < xj + length; j++)
			{
				if (graph[i][k] + graph[k][j] < graph[i][j])
					graph[i][j] = graph[i][k] + graph[k][j];
			}
		}
	}
}

void DFW (int **graph, int length, int m, int xi, int xj, int ui, int uj, int vi, int vj) {
	if (length == m)
		iterFW(graph, length,X11, U11, V11);
	else {
		int newlen = length / 2;
#ifdef PARALLEL
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X11, U11, V11);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X12, U11, V12);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X21, U21, V11);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X22, U21, V12);

		#pragma omp taskwait

		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X11, U12, V21);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X12, U12, V22);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X21, U22, V21);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X22, U22, V22);
		
		#pragma omp taskwait
#else
		DFW (graph, newlen, m, X11, U11, V11);
		DFW (graph, newlen, m, X12, U11, V12);
		DFW (graph, newlen, m, X21, U21, V11);
		DFW (graph, newlen, m, X22, U21, V12);
		
		DFW (graph, newlen, m, X11, U12, V21);
		DFW (graph, newlen, m, X12, U12, V22);
		DFW (graph, newlen, m, X21, U22, V21);
		DFW (graph, newlen, m, X22, U22, V22);

#endif
	}
}

void CFW (int **graph, int length, int m, int xi, int xj, int ui, int uj, int vi, int vj) {
	if (length == m)
        iterFW(graph, length, X11, U11, V11);
	else {
		int newlen = length / 2;
#ifdef PARALLEL
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			CFW(graph, newlen, m, X11, U11, V11);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			CFW(graph, newlen, m, X21, U21, V11);
				
		#pragma omp taskwait

		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X12, U11, V12);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X22, U21, V12);
			
		#pragma omp taskwait

		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			CFW(graph, newlen, m, X12, U12, V22);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			CFW(graph, newlen, m, X22, U22, V22);
		
		#pragma omp taskwait

		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X11, U12, V21);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X21, U22, V12);
		
		#pragma omp taskwait
#else
		CFW (graph, newlen, m, X11, U11, V11);
		CFW (graph, newlen, m, X21, U21, V11);
		
		DFW (graph, newlen, m, X12, U11, V12);
		DFW (graph, newlen, m, X22, U21, V12);
		
		CFW (graph, newlen, m, X12, U12, V22);
		CFW (graph, newlen, m, X22, U22, V22);
		
		DFW (graph, newlen, m, X11, U12, V21);
		DFW (graph, newlen, m, X21, U22, V12);

#endif
	}
}

void BFW (int **graph, int length, int m, int xi, int xj, int ui, int uj, int vi, int vj) {
	if (length == m)
		iterFW(graph, length, X11, U11, V11);
	else {
		int newlen = length / 2;
#if PARALLEL

		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			BFW(graph, newlen, m, X11, U11, V11);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			BFW(graph, newlen, m, X12, U11, V12);
		
		#pragma omp taskwait

		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X21, U21, V11);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X22, U21, V12);
		
		#pragma omp taskwait

		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			BFW(graph, newlen, m, X21, U22, V21);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			BFW(graph, newlen, m, X22, U22, V22);
		
		#pragma omp taskwait

		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X11, U12, V21);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			DFW(graph, newlen, m, X12, U12, V22);
		
		#pragma omp taskwait
#else
		BFW (graph, newlen, m, X11, U11, V11);
		BFW (graph, newlen, m, X12, U11, V12);
		
		DFW (graph, newlen, m, X21, U21, V11);
		DFW (graph, newlen, m, X22, U21, V12);
		
		BFW (graph, newlen, m, X21, U22, V21);
		BFW (graph, newlen, m, X22, U22, V22);
		
		DFW (graph, newlen, m, X11, U12, V21);
		DFW (graph, newlen, m, X12, U12, V22);
#endif
	}
}

void AFW (int **graph, int length, int m, int xi, int xj, int ui, int uj, int vi, int vj) {
	if (length == m)
		iterFW(graph, length, X11, U11, V11);
	else {
		int newlen = length / 2;
#if PARALLEL
		AFW (graph, newlen, m, X11, U11, V11);

		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			BFW(graph, newlen, m, X12, U11, V12);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			CFW(graph, newlen, m, X21, U21, V11);
	
		#pragma omp taskwait

		DFW (graph, newlen, m, X22, U21, V12);
		AFW (graph, newlen, m, X22, U22, V22);

		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			BFW(graph, newlen, m, X21, U22, V21);
		#pragma omp task firstprivate(graph,newlen,m,xi,xj,ui,uj,vi,vj)
			CFW(graph, newlen, m, X12, U12, V22);
				
		#pragma omp taskwait

		DFW (graph, newlen, m, X11, U12, V21);
#else
		AFW (graph, newlen, m, X11, U11, V11);
		BFW (graph, newlen, m, X12, U11, V12);
		CFW (graph, newlen, m, X21, U21, V11);
		
		DFW (graph, newlen, m, X22, U21, V12);
		AFW (graph, newlen, m, X22, U22, V22);
		BFW (graph, newlen, m, X21, U22, V21);
		CFW (graph, newlen, m, X12, U12, V22);
		
		DFW (graph, newlen, m, X11, U12, V21);

#endif
	}
}

void printSolution(int **graph, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (graph[i][j] == NA)
				cout << "       " << "-";
			else
				cout << "       "   << graph[i][j];
		}
		cout << "\n";
	}
}

int main(int argc, char* argv[])
{

	int n=32;
	int m=32;
	int num=0;	

	while(n<1024)
	{	
    	int **graph = new int*[n];
    	for (int i = 0; i < n; ++i)
        	graph[i] = new int[n];

    	for (int i = 0; i < n; i++)
        	for (int j = 0; j < n; j++)
		        	if (i == j)
                	graph[i][j] = 0;
            	else
                	graph[i][j] = i+j;

		std::cout << "For n = "<<n<<",\n"; 
		for (num=1;num<=16;num = num+1)
		{	  
			double start = omp_get_wtime();
				#pragma omp parallel num_threads(num)	
			{	
				#pragma omp single
				{
						AFW(graph, n, m, 0, 0, 0, 0, 0, 0);
				}
			}	
			std::cout <<num << "\t" << omp_get_wtime()-start <<"\n";
		}
		

		/*double start = omp_get_wtime();
		#pragma omp parallel num_threads(16)
		{	
			#pragma omp single
			{
					AFW(graph, n, m, 0, 0, 0, 0, 0, 0);
			}
		}	
		std::cout << "m: "<< m << " Time diff: " << omp_get_wtime()-start <<"\n";
*/
		//m=m/2;
		//printSolution(graph,n);
		for ( int i = 0 ; i < n; i++ )
	    {
			delete graph[i];
	    }
	    delete[] graph;    
	    n=n*2;	
	}    
	return 0;
}
