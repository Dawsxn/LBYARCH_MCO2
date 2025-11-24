#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

extern void asm_coord(int n, float *x1, float *x2, float *y1, float *y2, float *z);
extern void c_coord(int n, float *x1, float *x2, float *y1, float *y2, float *z);

void init_arrays(int n, float *x1, float *x2, float *y1, float *y2) {
    for (int i = 0; i < n; i++) {
        x1[i] = (float)rand() / RAND_MAX * 100.0f;
        x2[i] = (float)rand() / RAND_MAX * 100.0f;
        y1[i] = (float)rand() / RAND_MAX * 100.0f;
        y2[i] = (float)rand() / RAND_MAX * 100.0f;
    }
}

int check_correctness(int n, float *z_c, float *z_asm) {
    int correct = 1;
    int error_count = 0; 

    for (int i = 0; i < n; i++) {
        if (fabs(z_c[i] - z_asm[i]) > 0.001f) {
            printf("  [ERROR] Index %d: C=%.4f vs ASM=%.4f\n", i, z_c[i], z_asm[i]);
            correct = 0;
            
            error_count++; 
            
            if (error_count >= 10) {
                printf("  [STOPPING] Found 10 errors. Stopping check to save time.\n");
                return 0; 
            }
        }
    }
    return correct;
}

int main() {
    srand((unsigned int)time(NULL));
    
    int exponents[] = {20, 24, 28};
    int num_sizes = 3;

    printf("==============================================================\n");
    printf("  Correctness Check & Benchmarking (Average of 30 runs)\n");
    printf("==============================================================\n\n");

    for (int s = 0; s < num_sizes; s++) {
        long long n = 1LL << exponents[s]; 
        
        printf("--- DATA SIZE: 2^%d (Elements: %lld) ---\n", exponents[s], n);

        size_t bytes = n * sizeof(float);
        float *x1 = (float*)malloc(bytes);
        float *x2 = (float*)malloc(bytes);
        float *y1 = (float*)malloc(bytes);
        float *y2 = (float*)malloc(bytes);
        float *z_c = (float*)malloc(bytes);
        float *z_asm = (float*)malloc(bytes);

        if (!x1 || !x2 || !y1 || !y2 || !z_c || !z_asm) {
            printf("  [FATAL] Memory allocation failed! Your RAM is full.\n");
            return 1;
        }

        printf("  Initializing arrays... ");
        init_arrays(n, x1, x2, y1, y2);
        printf("Done.\n");

        printf("  Performing Sanity Check...\n");
        c_coord(n, x1, x2, y1, y2, z_c);        
        asm_coord(n, x1, x2, y1, y2, z_asm);    
        
        if (check_correctness(n, z_c, z_asm)) {
            printf("  [PASS] ASM Output matches C Output.\n");
        } else {
            printf("  [FAIL] Outputs do not match!\n");
            free(x1); free(x2); free(y1); free(y2); free(z_c); free(z_asm);
            return 1;
        }

        printf("\n  --- First 10 Elements of Vector Z ---\n");
        printf("   Index |      C Kernel      |     ASM Kernel\n");
        printf("  -------+--------------------+--------------------\n");
        for (int i = 0; i < 10; i++) {
            printf("   %5d | %18.6f | %18.6f\n", i, z_c[i], z_asm[i]);
        }
        printf("\n");

        double c_times[30];
        double asm_times[30];
        double total_time_c = 0.0;
        double total_time_asm = 0.0;
        clock_t start, end;

        printf("  Benchmarking C Kernel (30 runs)...\n");
        for (int run = 0; run < 30; run++) {
            start = clock();
            c_coord(n, x1, x2, y1, y2, z_c);
            end = clock();
            double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
            c_times[run] = time_taken;
            total_time_c += time_taken;
        }

        printf("  Benchmarking ASM Kernel (30 runs)...\n");
        for (int run = 0; run < 30; run++) {
            start = clock();
            asm_coord(n, x1, x2, y1, y2, z_asm);
            end = clock();
            double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
            asm_times[run] = time_taken;
            total_time_asm += time_taken;
        }

        printf("\n  --- Benchmark Results (Seconds) ---\n");
        printf("   Run # |      C Time      |     ASM Time\n");
        printf("  -------+------------------+------------------\n");
        for (int i = 0; i < 30; i++) {
            printf("   %5d | %16.6f | %16.6f\n", i + 1, c_times[i], asm_times[i]);
        }

        double avg_c = total_time_c / 30.0;
        double avg_asm = total_time_asm / 30.0;
        double diff = avg_c - avg_asm; 

        printf("\n  --- Summary for 2^%d ---\n", exponents[s]);
        printf("  Average C Time:   %.6f sec\n", avg_c);
        printf("  Average ASM Time: %.6f sec\n", avg_asm);
        printf("  Difference:       %.6f sec (C - ASM)\n", diff);
        
        if (diff > 0) 
            printf("  >> ASM is faster by %.6f sec\n", diff);
        else 
            printf("  >> C is faster by %.6f sec\n", -diff);
            
        printf("==============================================================\n\n");

        free(x1); free(x2); free(y1); free(y2); free(z_c); free(z_asm);
    }

    return 0;
}
