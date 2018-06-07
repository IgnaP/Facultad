#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#define MAX_N 16

double dwalltime(){
	double sec;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
int check_acceptable(int queen_rows[MAX_N], int n){
	int i, j;
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			// two queens in the same row => not a solution!
			if (queen_rows[i] == queen_rows[j]) 
				return 0;
			// two queens in the same diagonal => not a solution!
			if (queen_rows[i] - queen_rows[j] == i - j || queen_rows[i] - queen_rows[j] == j - i)
				return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[]){
	int n,i,iter;
	int max_iter = 1;
	int number_solutions = 0;
	
        n = (argc > 1) ? atoi(argv[1]) : 8;
	        
        for (i = 0; i < n; i++){
            max_iter *= n;
        }

	double start_time = dwalltime();

	for (iter = 0; iter < max_iter; iter++){
		int code = iter;
		int i;
		int queen_rows[MAX_N];
		// the index correspond to the queen's number and the queen's collumn
		// we only generate configurations where there's only one queen per collumn
		for (i = 0; i < n; i++){
			queen_rows[i] = code % n;
			code /= n;
		}
		if (check_acceptable(queen_rows, n)){
			number_solutions++;
		}
	}
	// print results
	printf("The execution time is %g sec\n", dwalltime() - start_time);
	printf("Number of found solutions is %d\n", number_solutions);
	return 0;
}
