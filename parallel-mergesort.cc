/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */
#include <omp.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sort.hh"
#include "mySeq.hh"

#define BOUND 10000
/*  returns start if T is empty or if x <= T[start] or
    returns mid if x > T[mid -1]
*/

int binarySearch(keytype x, keytype* T, int start, int end)
{
  int low = start;
  int high = start;
  if(start < end + 1)
    high = end + 1;

  int mid = 0;
  while(low < high){
    mid = (low + high) / 2;
    if(x <= T[mid])
      high = mid;
    else
      low = mid + 1;
  }
  return high;
}//binarySearch

/*swaps the values between first and sec*/
void swap(int *first, int *sec)
{
 		int temp = *sec;
    *sec = *first;
    *first = temp;
} 

/*  merges subarrays of T to form A;
    calls binarySearch
*/
void pMerge( keytype* T, int lower1, int upper1, int lower2, int upper2,
            keytype* A, int lowerOutput)
{
  int n1 = upper1 - lower1 + 1;
  int n2 = upper2 - lower2 + 1;

  if(n1 < n2){
	//	#pragma omp parallel sections num_threads(8)
//		{
	//	#pragma omp section
  //  {
		int myp = lower2;
    lower2 = lower1;
    lower1 = myp;
	//	}
	//	#pragma omp section
  //  {
		int myr = upper2;
    upper2 = upper1;
    upper1 = myr;
	//	} 
	//	#pragma omp section
	//	{   
		int myn = n2;
    n2 = n1;
    n1 = myn; 
//		}
	//	}
		/*swap(&n1,&n2);
		swap(&lower1,&lower2);
		swap(&upper1,&upper2);	
		*/
  }
  if (n1 == 0){
    return;
    printf("return\n");
  }
  else {
    int mid1 = (lower1 + upper1) / 2;
    int split2 = binarySearch(T[mid1], T, lower2, upper2);
    int indx_Divide = lowerOutput + (mid1 - lower1) + (split2 - lower2);
    A[indx_Divide] = T[mid1];
		#pragma omp task
		{
    pMerge(T, lower1, mid1 - 1, lower2, split2 - 1 , A, lowerOutput);
		pMerge(T, mid1 + 1, upper1, split2, upper2, A, indx_Divide + 1);	
		}
		}	
}//pMerge

/*  recursively sorts A and outputs an ordered B; calls pMerge
*/
void parallelSort(keytype* A, int start, int end, keytype* B, int startOutput)
{
  int n = end - start + 1;
  int mid = 0;
  int notQ = 0;

  if(n == 1)
    B[startOutput] = A[start];
  
	else /*if(n > 90) */{
    keytype* T = newKeys(n);
    mid = (start + end) / 2;
    notQ = mid - start + 1;
	
		//#pragma omp task
	  parallelSort(A, start, mid, T, 1);
	  parallelSort(A, mid + 1, end, T, notQ + 1);
		//#pragma omp taskwait
		omp_set_num_threads(8);
		#pragma omp parallel
	//	#pragma omp single
		pMerge(T, 1, notQ, notQ + 1, n, B, startOutput); 
	}
	//else
	//	seqSort(A, start, end);
	
}//parallelSort


//------------------------------------------------------------------------------

/*	#pragma omp parallel num_threads(1)
	{
		printf("Hi\n");
		// For single thread! parallel sort spits into: (n : threads)
	}	//3: 5, 4: 8, 10 : 34  => 2^(exclusive-ceil(n /2) ) = depth of Div. Work
*/
/* eof */
