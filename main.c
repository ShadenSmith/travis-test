
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void my_dgemm(
    double const * const restrict A,
    double const * const restrict B,
    double       * const restrict C,
    int const M,
    int const N,
    int const K)
{
  for(int j=0; j < N; ++j) {
    for(int i=0; i < M; ++i) {
      C[i + (j*M)] = 0.;
      for(int k=0; k < K; ++k) {
        C[i + (j*M)] += A[i + (k*M)] * B[k + (j*K)];
      }
    }
  }
}



void la_dgemm(
    double * const restrict A,
    double * const restrict B,
    double * const restrict C,
    int M,
    int N,
    int K)
{
  char transA = 'N';
  char transB = 'N';
  double alpha = 1.;
  double beta  = 0.;

  int lda = M;
  int ldb = K;
  int ldc = M;

	dgemm_(
      &transA, &transB,
      &M, &N, &K,
      &alpha,
      A, &lda,
      B, &ldb,
      &beta,
      C, &ldc);
}




int main(
    int argc,
    char ** argv)
{
  int const M = 128;
  int const N = 457;
  int const K = 11;

  /* all column major */
  double * A = malloc(M * K * sizeof(*A));
  double * B = malloc(N * K * sizeof(*B));
  double * Cx = malloc(M * N * sizeof(*Cx));
  double * Cy = malloc(M * N * sizeof(*Cy));

  /* initialize A and B */
  for(int x=0; x < M * K; ++x) {
    A[x] = x + 1.138;
  }
  for(int x=0; x < N * K; ++x) {
    B[x] = x + 0.247;
  }

  my_dgemm(A, B, Cx, M, N, K);
  la_dgemm(A, B, Cy, M, N, K);

  double norm = 0.;
  for(int x=0; x < M * N; ++x) {
    double const diff = Cx[x] - Cy[x];
    norm += diff * diff;
  }
  if(norm > 1e-12) {
    printf("ERROR: ||A - B||_F^2 = %0.3e\n", norm);
    return EXIT_FAILURE;
  }

  free(A);
  free(B);
  free(Cx);
  free(Cy);

  return EXIT_SUCCESS;
}

