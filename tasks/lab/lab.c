#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h>
#include <mpi.h>

#define ISIZE 1000
#define JSIZE 1000

void func(int rank, int size, double* res);
void cycle(double* res, int start, int limit);
void func_second(int rank, int size, double* res_second);
void cycle_second(double* res, int start, int limit);

int main(int argc, char **argv) {
	int rank, size;
	double* res;
	double *res_second;
	double start, end;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		res = (double*)malloc(ISIZE * JSIZE * sizeof(double));
        res_second = (double*)malloc(ISIZE * JSIZE * sizeof(double));

        start = MPI_Wtime();
	}

	func(rank, size, res);

	if (rank == 0) {
	    free(res);
		end = MPI_Wtime();
		printf("Time for parallelization of external cycle %d processes: %lf\n", size, end - start);
        start = MPI_Wtime();
    }

    func_second(rank, size, res_second);

    if (rank == 0) {
        end = MPI_Wtime();
        printf("Time for parallelization of both cycles %d processes: %lf\n", size, end - start);
    }

	if (rank == 0) {
		start = MPI_Wtime();
		cycle(res, 0, ISIZE);
		end = MPI_Wtime();
		printf("Time for single process: %lf\n", end - start);

		FILE *ff;
		ff = fopen("result.txt", "w");
		for(int i = 0; i < ISIZE; i++) {
			for (int j = 0; j < JSIZE; j++) { 
				fprintf(ff, "%f ", res[i * ISIZE + j]);
			}
			fprintf(ff, "\n");
		}
		free(res_second);
		fclose(ff);
	}
	MPI_Finalize();
	return 0;
}

void cycle(double* res, int start, int limit) {
	for (int i = start; i < start + limit; i++) {
	    for (int j = 0; j < JSIZE; j++) {
	    	res[(i - start) * JSIZE + j] = 10 * i + j;
	    }
	}
	for (int i = 0; i < limit; i++) {
	    for (int j = 0; j < JSIZE; j++) {
	    	res[i * JSIZE + j] = sin(0.00001 * res[i * JSIZE + j]);
	    }
	}
}

void func(int rank, int size, double* res) {
	MPI_Status  status;
    int start, taskSize;

    taskSize = ISIZE / size;
    if (rank < ISIZE % size) {
        taskSize++;
    }
    start = rank * taskSize;
    if (rank >= ISIZE % size) {
        start += (ISIZE % size) ;
    }
	if (rank == 0) {
        cycle(res, start, taskSize);
        start = taskSize * JSIZE;
        for (int i = 1; i < size; i++) {
			if (i == ISIZE % size) {
				taskSize--;
			}
			MPI_Recv(res + start, taskSize * JSIZE, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            start += taskSize * JSIZE;
		}
	}
	else {
		double* a = (double*)malloc(taskSize * JSIZE * sizeof(double));

		cycle(a, start, taskSize);
		MPI_Send(a, taskSize * JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		free(a);
	}
}


void cycle_second(double* res, int start, int limit) {
    for (int i = start; i < start + limit; i++) {
        res[i - start] = 10 * (i / ISIZE) + (i % JSIZE);
    }
    for (int i = 0; i < limit; i++) {
        res[i] = sin(0.00001 * res[i]);
    }
}

void func_second(int rank, int size, double* res) {
    MPI_Status  status;
    int start, taskSize;

    taskSize = ISIZE * JSIZE / size;
    if (rank < ISIZE * JSIZE % size) {
        taskSize++;
    }
    start = rank * taskSize;
    if (rank >= ISIZE * JSIZE % size) {
        start += ISIZE * JSIZE % size;
    }
    if (rank == 0) {
        cycle_second(res, start, taskSize);
        start = taskSize;
        for (int i = 1; i < size; i++) {
            if (i == ISIZE * JSIZE % size) {
                taskSize--;
            }
            MPI_Recv(res + start, taskSize, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);

            start += taskSize;
        }
    }
    else {
        double* a = (double*)malloc(taskSize * sizeof(double));

        cycle_second(a, start, taskSize);

        MPI_Send(a, taskSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        free(a);
    }
}