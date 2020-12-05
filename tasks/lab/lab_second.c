#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define ISIZE 10
#define JSIZE 10

void single(double *a, double *b);

void cycle(int start, int taskSize, double *a, double *b);

int main(int argc, char **argv) {
    int rank, size;
    double *b, *res, *a, *s;
    int i, j, k;
    int taskSize, start;
    double time_start, time_end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    taskSize = ISIZE / size;
    if (rank < ISIZE % size) {
        taskSize++;
    }
    start = rank;
    a = (double *) malloc(taskSize * JSIZE * sizeof(double));
    b = (double *) malloc(taskSize * JSIZE * sizeof(double));

    if (rank == 0) {
        time_start = MPI_Wtime();
        if (size != 3) {
            printf("Error, number of processes should be 3");
        }
        res = (double *) malloc(taskSize * JSIZE * sizeof(double));

        int *sizes = (int *) malloc(size * sizeof(int));
        int *starts = (int *) malloc(size * sizeof(int));

        for (i = 0; i < size; i++) {
            sizes[i] = ISIZE / size;
            if (i < ISIZE % size) {
                sizes[i]++;
            }
            starts[i] = i;
        }
        cycle(start, taskSize, a, b);
        for (i = 1; i < size; i++) {
            for (k = 0; starts[i] + k < ISIZE - 3; k += 3) {
               MPI_Recv(res + ((starts[i] + k) * JSIZE), JSIZE, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            }
        }
        for (j = 0, k = 0; j < taskSize; j++, k += 3) {
            for (i = 0; i < JSIZE; i++) {
                res[(start + k) * JSIZE + i] = b[j * JSIZE + i];
            }
        }
        time_end = MPI_Wtime();
        printf("Time for 3 processes: %lf\n", time_end - time_start);
        FILE *ff;
        time_start = MPI_Wtime();
        s = (double *) malloc(ISIZE * JSIZE * sizeof(double));
        single(s, res);
        time_end = MPI_Wtime();
        printf("Time for 1 process: %lf\n", time_end - time_start);
        ff = fopen("result.txt", "w");
        for (i = 0; i < ISIZE; i++) {
            for (j = 0; j < JSIZE; j++) {
                fprintf(ff, "%f ", res[i * JSIZE + j]);
            }
            fprintf(ff, "\n");
        }
        free(res);
        free(s);
        free(b);
        free(a);
        fclose(ff);
    } else {
        cycle(start, taskSize, a, b);
        for (i = 0; i < taskSize; i++) {
            MPI_Send(b + (i * JSIZE), JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
        free(a);
        free(b);
    }
    MPI_Finalize();
    return 0;
}

void cycle(int start, int taskSize, double *a, double *b) {
    int i, j, k;
    for (i = start, k = 0; k < taskSize; i += 3, k++) {
        for (j = 0; j < JSIZE; j++) {
            a[k * ISIZE + j] = 10 * i + j;
            b[k * ISIZE + j] = 0.;
        }
    }
    for (i = start, k = 0; k < taskSize; i += 3, k++) {
        for (j = 0; j < JSIZE; j++) {
            a[k * ISIZE + j] = sin(0.00001 * a[k * ISIZE + j]);
        }
    }
    for (i = 0; i < taskSize - 1; i++) {
        for (j = 5; j < JSIZE; j++) {
            b[i * ISIZE + j] = a[(i + 1) * JSIZE + j - 5];
        }
    }
}

void single(double *a, double *b) {
    int i, j;
    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i * JSIZE + j] = 10 * i + j;
            b[i * ISIZE + j] = 0.;
        }
    }
    for (i = 0; i < ISIZE; i++){
        for (j = 0; j < JSIZE; j++){
            a[i * ISIZE + j] = sin(0.00001 * a[i * ISIZE + j]);
        }
    }
    for (i = 0; i < ISIZE - 3; i++) {
        for (j = 5; j < JSIZE; j++) {
            b[i * JSIZE + j] = a[(i + 3) * JSIZE + j - 5];
        }
    }
}