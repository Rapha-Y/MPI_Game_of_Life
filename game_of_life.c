#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>

#define GRID_SIZE 64
#define LIVING 1
#define DEAD 0

void init_grid(int **grid, int start, int end) {
    for (int i = start; i < end; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = DEAD;
        }

        int row = i;
        int col = 1;

        //Glider
        switch (row) {
            case 1:
                grid[row][col + 1] = LIVING;
                break;
            case 2:
                grid[row][col + 2] = LIVING;
                break;
            case 3:
                grid[row][col] = LIVING;
                grid[row][col + 1] = LIVING;
                grid[row][col + 2] = LIVING;
                break;
        }

        col = 30;

        //R-Pentonimo
        switch (row) {
            case 10:
                grid[row][col + 1] = LIVING;
                grid[row][col + 2] = LIVING;
                break;
            case 11:
                grid[row][col] = LIVING;
                grid[row][col + 1] = LIVING;
                break;
            case 12:
                grid[row][col + 1] = LIVING;
        }
    }
}

int getNeighbors(int **grid, int i, int j) {  
    int prev_i = i - 1;
    int prev_j = j - 1;
    int next_i = i + 1;
    int next_j = j + 1;

    //loop through the board's edges
    if (prev_i < 0) {
        prev_i = GRID_SIZE - 1;
    }
    if (prev_j < 0) {
        prev_j = GRID_SIZE - 1;
    }
    if (next_i > GRID_SIZE - 1) {
        next_i = 0;
    }
    if (next_j > GRID_SIZE - 1) {
        next_j = 0;
    }

    //count number of living neighbors
    int neighbor_num = 0;
    if (grid[prev_i][prev_j] == LIVING) {
        neighbor_num++;
    }
    if (grid[prev_i][j] == LIVING) {
        neighbor_num++;
    }
    if (grid[prev_i][next_j] == LIVING) {
        neighbor_num++;
    }
    if (grid[i][prev_j] == LIVING) {
        neighbor_num++;
    }
    if (grid[i][next_j] == LIVING) {
        neighbor_num++;
    }
    if (grid[next_i][prev_j] == LIVING) {
        neighbor_num++;
    }
    if (grid[next_i][j] == LIVING) {
        neighbor_num++;
    }
    if (grid[next_i][next_j] == LIVING) {
        neighbor_num++;
    }

    return neighbor_num;
}

int count_living(int **grid, int start, int end) {
    int living_num = 0;
    for (int i = start; i < end; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == LIVING) {
                living_num++;
            }
        }
    }
    return living_num;
}

int main(int argc, char *argv[]) {

    int process_num;
    int process_rank;

    int **grid = (int**) malloc(GRID_SIZE * sizeof(int*));
    for (int i = 0; i < GRID_SIZE; i++) {
        grid[i] = (int*) malloc(GRID_SIZE * sizeof(int));
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &process_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    
    int start = process_rank * GRID_SIZE / process_num;
    int end = (process_rank + 1) * GRID_SIZE / process_num;;

    init_grid(grid, start, end);

    int sum;
    int partial_sum = count_living(grid, start, end);
    MPI_Reduce(&partial_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    printf("%d\n", partial_sum);

    MPI_Finalize();

    if (process_rank == 0) {
        printf("%d\n", sum);
    }

    return 0; 
}