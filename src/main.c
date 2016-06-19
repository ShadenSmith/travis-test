
#include "timer.h"

#include <stdio.h>
#include <stdlib.h>


#ifndef VAL_TYPEWIDTH
#define VAL_TYPEWIDTH 64
#endif


#if VAL_TYPEWIDTH == 32
  typedef float val_t;
  #define GEMM sgemm_
#else
  typedef double val_t;
  #define GEMM dgemm_
#endif


void my_dgemm(
    val_t const * const restrict A,
    val_t const * const restrict B,
    val_t       * const restrict C,
    int const M,
    int const N,
    int const K)
{
  sp_timer_t timer;
  timer_fstart(&timer);

  for(int j=0; j < N; ++j) {
    for(int i=0; i < M; ++i) {
      C[i + (j*M)] = 0.;
      for(int k=0; k < K; ++k) {
        C[i + (j*M)] += A[i + (k*M)] * B[k + (j*K)];
      }
    }
  }

  timer_stop(&timer);
  printf("my_dgemm: %0.3fs\n", timer.seconds);
}



void la_dgemm(
    val_t * const restrict A,
    val_t * const restrict B,
    val_t * const restrict C,
    int M,
    int N,
    int K)
{
  sp_timer_t timer;
  timer_fstart(&timer);

  char transA = 'N';
  char transB = 'N';
  val_t alpha = 1.;
  val_t beta  = 0.;

  int lda = M;
  int ldb = K;
  int ldc = M;

	GEMM(
      &transA, &transB,
      &M, &N, &K,
      &alpha,
      A, &lda,
      B, &ldb,
      &beta,
      C, &ldc);
  timer_stop(&timer);
  printf("la_dgemm: %0.3fs\n", timer.seconds);
}




int main(
    int argc,
    char ** argv)
{
  int const M = 138;
  int const N = 1138;
  int const K = 35;

  /* all column major */
  val_t * A = malloc(M * K * sizeof(*A));
  val_t * B = malloc(N * K * sizeof(*B));
  val_t * Cx = malloc(M * N * sizeof(*Cx));
  val_t * Cy = malloc(M * N * sizeof(*Cy));

  /* initialize A and B */
  for(int x=0; x < M * K; ++x) {
    A[x] = x * 0.001138;
    if(A[x] > 1) {
      A[x] = 0.98;
    }
  }
  for(int x=0; x < N * K; ++x) {
    B[x] = x * 0.0247;
    if(B[x] > 1) {
      B[x] = 0.98;
    }
  }

  my_dgemm(A, B, Cx, M, N, K);
  la_dgemm(A, B, Cy, M, N, K);

  long double cx_norm = 0.;
  long double cy_norm = 0.;
  long double diff_norm = 0.;
  for(int x=0; x < M * N; ++x) {
    long double const diff = Cx[x] - Cy[x];
    diff_norm += diff * diff;
    cx_norm += Cx[x] * Cx[x];
    cy_norm += Cy[x] * Cy[x];
  }
  if(diff_norm > 1e-6) {
    fprintf(stderr, "ERROR: ||A - B||_F^2 = %0.3Le\n", diff_norm);
    return EXIT_FAILURE;
  }

  free(A);
  free(B);
  free(Cx);
  free(Cy);

  return EXIT_SUCCESS;
}

