#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include "sort.hh"
#include <utility>
#include <iostream>
#include <utility>
#include <vector>
#include <iterator>
using namespace std;


int binsearch(int x, keytype* T, int p, int r){
int low = p;
int high = max(p, r+1);
while (low < high){
  int mid = (low+high)/2;

  if (x<=T[mid]) {
    high = mid;
  } else {
    low = mid + 1;
  }
}
return high;
}

void seq_merge(keytype* A, int p, int q, int p2, int q2, int p3, keytype* B){

  int n1=q-p+1;
  int n2=q2-p2+1;
  int* L = (int *)malloc (n1 * sizeof (int));
  int* R = (int *)malloc (n2 * sizeof (int));
  if ( n1 < n2 ){std::swap(n1,n2);std::swap(p,p2);std::swap(q,q2);}
  if(n1==0){
  }else{

if(q<p){
    for (int c = 0; c < (n1); c++) {
      L[c]=A[c+q];
    }

  }else{

    for (int c = 0; c < (n1); c++) {
      L[c]=A[c+p];
  }
}

if(q2<p2){
    for (int c = 0; c < n2;c++){
      R[c] = A[c+q2];
    }
  }else{
    for (int c = 0; c < n2;c++){
      R[c] = A[c+p2];

  }
}

    int i=0,j=0,k=p3;



    while ( i < n1 && j < n2) {
    if (L[i] <=R[j] ) {
      B[k]=L[i];
      i++;

    } else{
      B[k] = R[j];
      j++;

     }
     k++;
    }

    while (i < n1){
      B[k] = L[i];
      i++;
      k++;
    }

    while (j < n2){
      B[k] = R[j];
      j++;
      k++;
    }
  }
  free(L);
  free(R);

}


void seqmerge2 (keytype* A, int index1, int mid, int index2){


  if (index1==index2-1)
  {
    if (A[index1]>=A[index2]){std::swap(A[index1],A[index2]);}else{}
  }else{

  int size = index2 - index1 +1;
  int* C = (int *)malloc (size * sizeof (int));
  for (int c = 0; c < (size); c++) {
    C[c]=A[c+index1];
  }
int n1=mid-index1+1;
int n2=index2-mid;
int i=0;
int j=0;
int k=index1;

while ( i < n1 && j < n2) {
if (C[i] <=C[j+n1] ) {
  A[k]=C[i];
  i++;

} else{
  A[k] = C[j+n1];
  j++;

 }
 k++;
}

while (i < n1){
  A[k] = C[i];
  i++;
  k++;
}

while (j < n2){
  A[k] =C[j+n1];
  j++;
  k++;
}
free (C);
}

}

void mergesort_seq(int index1, int m, int index2, keytype* A){
if (index1<index2){
  m=(index2+index1) / 2;
  mergesort_seq(index1,m,m,A);
  mergesort_seq(m+1,m,index2,A);
  seqmerge2(A,index1,m,index2);

}
}

void mergesort(int index1,int m, int index2, keytype* A, int count){
if (index1 < index2 ){
const int cutoff=1000;
m=(index2+index1)/2;

if ( ((index2-index1+1) < cutoff) || ((1 << count)>=8)){
  mergesort_seq(index1,m,index2,A);
}else{
  count++;
  #pragma omp task
  mergesort(index1,m,m,A,count);
  mergesort(m+1,m,index2,A,count);
  #pragma omp taskwait
  seqmerge2(A,index1,m,index2);
}
  }

  }

  void
  parallelSort_1 (int N, keytype* A){
  if (N>1){

    #pragma omp parallel
    {
    #pragma omp single nowait
    {
    int middle=(N-1)/2;
    mergesort(0,middle,N-1,A,0);
    }
  }
  }

  }


  void parmerge(keytype* T, int p1, int r1, int p2, int r2, keytype* B, int p3, int count2){
    const int cutoff=1000;
    int n1 = r1-p1+1;
    int n2 = r2-p2+1;
    if ( n1 < n2 ){std::swap(n1,n2);std::swap(p1,p2);std::swap(r1,r2);}
    if(n1==0){
    }else{
    count2++;
      int q1 = (p1+r1)/2;
      int q2 = binsearch(T[q1],T,p2,r2);
      int q3 = p3+(q1-p1)+(q2-p2);
      B[q3] = T[q1];

      if ( n1+n2 < cutoff || ((1 << count2)>=8)){
       seq_merge(T,p1,r1,p2,r2,p3,B);
      }else{
      #pragma omp task
      parmerge(T,p1,q1-1,p2,q2-1,B,p3 ,count2);
      parmerge(T,q1+1,r1,q2,r2,B,q3+1,count2);
  }


    }


  }
  void parallelSort_2 (int N, keytype* A, keytype* B){
  if (N>1){
   #pragma omp parallel
   #pragma omp single nowait
    parmergesort(A,0,N-1,B,0);

  }
  }


  void parmergesort(keytype* A, int p, int r, keytype* B, int s){
    const int cutoff=1000;
    int n=r-p+1;
    if (n==1){
      B[s]=A[p];
    }else{
    int q = (p+r)/2;
      if ( ((n) < cutoff) /*|| ((1 << count)>=8)*/){
       mergesort_seq(p,q,r,A);
      }else{
      int qp= q-p+1;
      #pragma omp task
      parmergesort(A,p,q,B,s);
      parmergesort(A,q+1,r,B,qp);
      #pragma omp taskwait
      parmerge(A,0,qp-1, qp,n-1,B,0,0);

  }
    }
  }


  void seqmerge (keytype* A, int index1, int mid, int index2){

  int n1=mid-index1+1;
  int n2=index2-mid;

  keytype* L = newKeys(n1);
  keytype* R = newKeys(n2);

  for (int c = 0; c < (n1); c++) {
    L[c]=A[c+index1];
  }

  for (int c = 0; c < n2;c++){
    R[c] = A[c+mid+1];
  }

  int i=0,j=0,k=index1;



  while ( i < n1 && j < n2) {
  if (L[i] <=R[j] ) {
    A[k]=L[i];
    i++;

  } else{
    A[k] = R[j];
    j++;

   }
   k++;
  }

  while (i < n1){
    A[k] = L[i];
    i++;
    k++;
  }

  while (j < n2){
    A[k] = R[j];
    j++;
    k++;
  }



  }
