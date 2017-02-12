#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
void hello (void);

int main (int argx, char* argv[]){
        int thread_conut = strtol(argv[1], NULL, 10);

    #pragma omp parallel num_threads(thread_count)
        Hello(); omp_get_thread_num

        return =0;

}
