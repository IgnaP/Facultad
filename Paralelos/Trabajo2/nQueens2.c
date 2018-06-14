/*
<------  N-Queens Solutions  -----> <---- time ---->
 N:           Total          Unique days hh:mm:ss.--
 5:              10               2             0.00
 6:               4               1             0.00
 7:              40               6             0.00
 8:              92              12             0.00
 9:             352              46             0.00
10:             724              92             0.00
11:            2680             341             0.00
12:           14200            1787             0.00
13:           73712            9233             0.02
14:          365596           45752             0.05
15:         2279184          285053             0.22
16:        14772512         1846955             1.47
17:        95815104        11977939             9.42
18:       666090624        83263591          1:11.21
19:      4968057848       621012754          8:32.54
20:     39029188884      4878666808       1:10:55.48
21:    314666222712     39333324973       9:24:40.50

*/

#include <stdlib.h>
#include <stdio.h>

/* Checking information */
static int solutions[] = {
        0,
        0,
        0,
        0,
        0, /* 5 */
        0,
        0,
        0,
        0,
        0, /* 10 */
        0,
        0,
        0,
        0,
        0, /* 15 */
        0,
        0,
        0
};
#define MAX_SOLUTIONS sizeof(solutions)/sizeof(int)

#ifndef _NQUEENS_H_
#define _NQUEENS_H_

int ok(int queen_number, int row_position, int* position);
int put_queen(int size, int queen_number, int* position);
void nqueens(int size, int threads, int *solutions);
int find_queens(int size, int threads);

#endif /* _NQUEENS_H_ */

#define N 5
#define THREADS 1

int total_count;

int ok(int queen_number, int row_position, int* position) {
	int i;

	// Check each queen before this one
	for(i = 0; i < queen_number; i++) {
		// Get another queen's row_position
		int other_row_pos = position[i];

		// Now check if they're in the same row or diagonals
		if (other_row_pos == row_position || // Same row
			other_row_pos == row_position - (queen_number - i) || // Same diagonal
			other_row_pos == row_position + (queen_number - i))   // Same diagonal
			return 0;
	}

	return 1;
}

int put_queen(int size, int queen_number, int* position) {
	int i;
	for(i=position[queen_number]+1; i<size; i++) {
		if(ok(queen_number, i, position)) {
			position[queen_number] = i;
			return 1;
		}
	}

	position[queen_number] = -1;
	return 0;
}

void nqueens(int size, int threads, int *solutions) {
	int i, count;
	int* position;

	count = 0;

	for(i=0; i<size; i++) {
		int j;
		position = (int *) malloc(size * sizeof(int));
		position[0] = i;

		for(j = 1; j < size; j++)
			position[j] = -1;

		int queen_number = 1;
		while(queen_number > 0) {
			if(put_queen(size, queen_number, position)) {
				queen_number++;

				if(queen_number == size) {
					count += 1;

					position[queen_number-1] = -1;
					queen_number -= 2;
				}
			} else {
				queen_number--;
			}
		}
	}

	*solutions = count;
}

int find_queens(int size, int threads) {
	total_count=0;
	nqueens(size, threads, &total_count);
	return total_count;
}

long wtime();

int main(int argc, char** argv) {
	int size = N, threads = THREADS;
	long start_time, end_time;

	if(argc > 1)
		size = atoi(argv[1]);

	if(argc > 2)
		threads = atoi(argv[2]);

	start_time = wtime();
	int solutions = find_queens(size, threads);
	end_time = wtime();

	printf("Nqueens openmp_parallel;size: %d;threads: %d;cut_off: 0;time: %ld;solutions: %d\n",
			 size, threads, (long) (end_time - start_time), solutions);
}

/* wtime */
long wtime() {
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}
