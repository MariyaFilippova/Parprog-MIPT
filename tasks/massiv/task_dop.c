#include <omp.h>
#include <stdio.h>

#define n 100000
int main(int argc, char **argv) {
	int a[n];
	for (int i = 0; i < n; i++) {
		a[i] = i;
	}
	#pragma omp parallel 
	{	
		// чтобы не было зависимости по данным i += 3 
		#pragma omp for
		for (int i = 1; i < n; i+=3) {
			a[i] = (a[i - 1] * a[i] * a[i+1]) / 3;
		}
		// не уверена, что это нормальное решение, которое надо было придумать
		#pragma omp for
		for (int i = 2; i < n; i+=3) {
			a[i] = ((i - 1) * a[i] * a[i+1]) / 3;
		}
		#pragma omp for
		for (int i = 3; i < n; i+=3) {
			a[i] = ((i - 1) * a[i] * (i + 1)) / 3;
		}
	}
}