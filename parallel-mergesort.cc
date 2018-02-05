/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */
#include <omp.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "sort.hh"
#include "mySeq.hh"

using namespace std;

#define BOUND 9
/*  returns start if T is empty or if x <= T[start] or
    returns mid if x > T[mid -1]
*/
int binarySearch(keytype x, keytype* T, int start, int end)
{
  int low = start;
  int high = max(start, end + 1);

  while(low < high){
    int mid = (low + high) / 2;
    if(x <= T[mid])
      high = mid;
    else
      low = mid + 1;
  }
  return high;
}//binarySearch

/*  merges subarrays of T to form A;
    calls binarySearch
*/
void pMerge( keytype* T, int lower1, int upper1, int lower2, int upper2,
            keytype* A, int lowerOutput)
{
  int n1 = upper1 - lower1 + 1;
  int n2 = upper2 - lower2 + 1;

  if(n1 < n2){
    swap(lower1, lower2);
    swap(upper1, upper2);
    swap(n1,n2);
  }
  if (n1 == 0)
    return;

  else {//if(n1 >= BOUND){
    int mid1 = (lower1 + upper1) / 2;
    int split2 = binarySearch(T[mid1], T, lower2, upper2);
    int indx_Divide = lowerOutput + (mid1 - lower1) + (split2 - lower2);
    A[indx_Divide] = T[mid1];
    omp_set_num_threads(1);
    #pragma omp parallel
    {
    #pragma omp task
    {
    pMerge(T, lower1, mid1 - 1, lower2, split2 - 1 , A, lowerOutput);
		pMerge(T, mid1 + 1, upper1, split2, upper2, A, indx_Divide + 1);
    }
    }
  }
  /*else if (n1 > 1 && n1 < BOUND){
    int mid1 = (lower1 + upper1) / 2;
    sMerge(T, 0, mid1, n1 + n2);
    A = T;
  }*/
}//pMerge

/*  recursively sorts A and outputs an ordered B; calls pMerge
*/
void parallelSort(keytype* A, int start, int end, keytype* B, int startOutput)
{
  int n = end - start;

  if(n == 1)
    B[startOutput] = A[start];
  else {//if (n >= BOUND){
    keytype* T = newKeys(n);
    int mid = (start + end) / 2;
    int notQ = mid - start;

  /*  for (int  i = 0; i < n; i++) {
      printf("A[%i] %i\n", i, A[i]);
    }
*/  omp_set_num_threads(1);
    #pragma omp parallel
    {
    #pragma omp task
    {
 	  parallelSort(A, start, mid, T, 0);
	  parallelSort(A, mid, end, T, notQ);
    }
    #pragma omp taskwait
    }

		pMerge(T, 0, notQ - 1, notQ, n , B, startOutput);
	}
 // for (int  i = 0; i < n; i++)
 //   printf("B[%i] %i\n", i, B[i]);
/*  else if (n > 1 && n < BOUND){
    seqSort (A, start, end);
  //  A = B;
} */
}//parallelSort


//------------------------------------------------------------------------------

/*	#pragma omp parallel num_threads(1)
	{
		printf("Hi\n");
		// For single thread! parallel sort spits into: (n : threads)
	}	//3: 5, 4: 8, 10 : 34  => 2^(exclusive-ceil(n /2) ) = depth of Div. Work
*/
/* eof */
