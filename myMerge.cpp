#include <omp.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#define SIZE 5
typedef unsigned long keytype;

keytype *
newKeys (int N)
{
  keytype* A = (keytype *)malloc (N * sizeof (keytype));
  assert (A);
  return A;
}

/** Returns a new copy of A[0:N-1] */
keytype *
newCopy (int N, const keytype* A)
{
  keytype* A_copy = newKeys (N);
  memcpy (A_copy, A, N * sizeof (keytype));
  return A_copy;
}

//book
int binarySearch(keytype x, keytype* T, int p, int r)
{
  int low = p;
  int high = p;
  if(p < r + 1)
    high = r + 1;

  int mid = 0;
  while(low < high){
    mid = (low + high) / 2;
    if(x <= T[mid])
      high = mid;
    else
      low = mid + 1;
  }
  return high;
}
 //new
void merge(keytype* T, int lower1, int upper1, int lower2, int upper2, keytype* A, int lowerOutput)
{
  int n1 = upper1 - lower1 + 1;
  int n2 = upper2 - lower2 + 1;

//  printf("T size = %i\t A size = %i\n", n1, n2);

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
  //  printf("in if\n");
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

    //printf("index = %i\n", indx_Divide);
    merge(T, lower1, mid1 - 1, lower2, split2 - 1 , A, lowerOutput);
    merge(T, mid1 + 1, upper1, split2, upper2, A, indx_Divide + 1);
  }
  //  printf("outside\n");
}

// new
void parallelSort(keytype* A, int start, int end, keytype* B, int startOutput)
{
  printf("do I even run?\n");
  int n = end - start + 1;
  int mid = 0;
  int notQ = 0;

  if(n == 1)
    B[startOutput] = A[start];
  else {
    keytype* T = newKeys(n);
    mid = (start + end) / 2;
    notQ = mid - start + 1;
    printf("pass?\n");
    parallelSort(A, start, mid, T, 1);
    parallelSort(A, mid + 1, end, T, notQ + 1);
    //printf("so far so good\n");
    merge(T, 1, notQ, notQ + 1, n, B, startOutput);
    //printf("good?\n");
  }
}

void assertIsSorted (int N, const keytype* A)
{
  for (int i = 1; i < N; ++i) {
    if (A[i-1] > A[i]) {
      fprintf (stderr, "*** ERROR ***\n");
      fprintf (stderr, "  A[i=%d] == %lu > A[%d] == %lu\n", i-1, A[i-1], i, A[i]);
      assert (A[i-1] <= A[i]);
    }
  } /* i */
  printf ("\t(Array is sorted.)\n");
}

void assertIsEqual (int N, const keytype* A, const keytype* B)
{
  for (int i = 0; i < N; ++i) {
    if (A[i] != B[i]) {
      fprintf (stderr, "*** ERROR ***\n");
      fprintf (stderr, "  A[i=%d] == %lu, but B[%d] == %lu\n", i, A[i], i, B[i]);
      assert (A[i] == B[i]);
    }
  } /* i */
  printf ("\t(Arrays are equal.)\n");
}

int main()
{
  /* Create an input array of length N, initialized to random values */
  keytype* A_in = newKeys (SIZE);
  for (int i = 0; i < SIZE; ++i)
    A_in[i] = lrand48 ();

//keytype A_in[]  = {5, 4, 10, 40, 44};
  keytype* B = A_in;
  for (int i = 0; i < SIZE; ++i) {
    printf("B%i = %i\n",i,B[i]);
  }

  printf("Given\n");
  for (int i = 0; i < SIZE; ++i) {
    printf("%i\n",A_in[i]);
  }

  printf ("\nN == %d\n\n", SIZE);

  /*for (int i = 0; i < SIZE; i++){
    keytype test = binarySearch(A_in[i], A_in, 0, SIZE);
    printf("test = %i\n", test);
  }*/

  //merge(keytype* T, int lower1, int upper1, int lower2, int upper2, keytype* A, int lowerOutput)
//  merge(A_in, 0, SIZE/2, SIZE/2 + 1, SIZE, B, 0);
  parallelSort(A_in, 0, SIZE , B, 0 );
   for (int i = 0; i < SIZE; ++i) {
    printf("B[%i] = %i\n",i, B[i]);
  }
  //assertIsSorted(SIZE, B);
}
