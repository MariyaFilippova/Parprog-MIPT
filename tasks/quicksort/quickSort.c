#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

void printArray(int32_t *a, int n) {
    printf("%s", "Your array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
}

void quickSort(int32_t *a, int l, int r) {
    if (l < r) {
        int mid = (l + r) / 2;
        int i = l;
        int j = r;
        do {
            while (a[i] < a[mid]) {
                i++;
            }
            while (a[j] > a[mid]) {
                j--;
            }
            if (i <= j) {
                int c = a[i];
                a[i] = a[j];
                a[j] = c;
                i++;
                j--;
            }

        } while (i <= j);
        quickSort(a, l, j);
        quickSort(a, i, r);
    }
}

int main() {
    int mode = 0, N = 0;
    int32_t* a;
    printf("Please, print the number, illustrating data.txt source: 1 - console, 2 - file \n");
    scanf("%d", &mode);
    switch (mode) {
        case 1: {
            printf("%s", "Number of elements: \n");
            scanf("%d", &N);
            if (N <= 0 || N > 10000) {
                printf("%s", "Invalid number of elements");
                return -1;
            }
            a = (int32_t *) malloc(N * sizeof(int32_t));
            for (int i = 0; i < N; i++) {
                printf("%s %d\n", "element:", i);
                scanf("%d", a + i);
            }
            printArray(a, N);
            break;
        }
        case 2: {
            FILE *fp;
            char name[] = "data.txt";
            if ((fp = fopen(name, "r")) == NULL) {
                printf("Can't open file");
                getchar();
                return -1;
            }
            fscanf(fp, "%d", &N);
            if (N <= 0 || N > 10000) {
                printf("%s", "Invalid number of elements");
                return -1;
            }
            a = (int *) malloc(N * sizeof(int));
            for (int i = 0; i < N; i++) {
                fscanf(fp, "%d", a + i);
            }
            fclose(fp);
            printArray(a, N);
        }
    }
    quickSort(a, 0, N - 1);
    printArray(a, N);
}