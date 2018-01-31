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

/*  merges subarrays of T to form A;
    calls binarySearch
*/
void merge( keytype* T, int lower1, int upper1, int lower2, int upper2,
            keytype* A, int lowerOutput)
{
  int n1 = upper1 - lower1 + 1;
  int n2 = upper2 - lower2 + 1;

  if(n1 < n2){
    int myp = lower2;
    lower2 = lower1;
    lower1 = myp;
    int myr = upper2;
    upper2 = upper1;
    upper1 = myr;
    int myn = n2;
    n2 = n1;
    n1 = myn;
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
    merge(T, lower1, mid1 - 1, lower2, split2 - 1 , A, lowerOutput);
    merge(T, mid1 + 1, upper1, split2, upper2, A, indx_Divide + 1);
  }
}//merge

/*  recursively orders A; calls merge
*/
void parallelSort(keytype* A, int start, int end, keytype* B, int startOutput)
{
  int n = end - start + 1;
  int mid = 0;
  int notQ = 0;

  if(n == 1)
    B[startOutput] = A[start];
  else {
    keytype* T = newKeys(n);
    mid = (start + end) / 2;
    notQ = mid - start + 1;
    parallelSort(A, start, mid, T, 1);
    parallelSort(A, mid + 1, end, T, notQ + 1);
    merge(T, 1, notQ, notQ + 1, n, B, startOutput);
  }
}//parallelSort

/* eof */
