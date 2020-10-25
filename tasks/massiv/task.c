#include <omp.h>
#include <stdio.h>

#define n 100000
int main(int argc, char **argv) {
	int a[n];
	int b[n];
	for (int i = 0; i < n; i++) {
		a[i] = i;
	}
	#pragma omp parallel 
	{	
		// параллельный фор
		#pragma omp for
		for (int i = 1; i < n; i++) {
			b[i] = (b[i - 1] * b[i] * b[i+1]) / 3;
		}
		
	}
}