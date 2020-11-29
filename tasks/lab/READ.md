# Efficiency and acceleration of parallel programs
Laboratory work
## Launch
```bash
./build.sh
./run.sh [number of processes]
```
## Content
```lab.pdf``` - task

```lab.c``` - main task

```lab_second.c``` - optional task
## First Task 
Make parallel version of this function:
```
void cycle(double ** a) {
    int i, j;
    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = 10 * i + j;
        }
    }
    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = sin(0.00001 * a[i][j]);
        }
    }
}
```
### Functions
```void func(int rank, int size, double* res)``` - function that runs on every process. 
We make parallelization of external cycle, devide rows of matrix and gave them for slave processes
as a task. Master process collect the result of computations. 

```void cycle(double* res, int start, int limit)``` - computation function

```void func_second(int rank, int size, double* res_second)``` - function that runs on every process. 
We make parallelization of external and internal cycles, devide rows * columns of matrix and gave them for slave processes
as a task. Master process collect the result of computations. 

```void cycle_second(double* res, int start, int limit)``` - computation function
### Results
