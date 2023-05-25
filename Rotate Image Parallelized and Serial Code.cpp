#include <iostream>
#include <vector>
#include <omp.h>

void normal_rotate(std::vector<std::vector<char>>& matrix, int n) {
    int maxPos = n - 1;
    int startingPosX = 0;
    int startingPosY = 0;

    for (int rotationWidth = n; rotationWidth > 1; rotationWidth -= 2) {
        int swapPosX = startingPosX;
        int swapPosY = startingPosY;

        for (int i = 0; i < rotationWidth - 1; i++) {
            for (int j = 0; j < 3; j++) {
                if (j == 0) {
                    char temp = matrix[swapPosX][swapPosY];
                    int x = i + swapPosX;
                    int y = maxPos - startingPosX;
                    matrix[swapPosX][swapPosY] = matrix[x][y];
                    matrix[x][y] = temp;
                }
                else if (j == 1) {
                    char temp = matrix[swapPosX][swapPosY];
                    int x = maxPos - startingPosX;
                    int y = maxPos - swapPosY;
                    matrix[swapPosX][swapPosY] = matrix[x][y];
                    matrix[x][y] = temp;
                }
                else if (j == 2) {
                    char temp = matrix[swapPosX][swapPosY];
                    int x = maxPos - i - swapPosX;
                    int y = startingPosY;
                    matrix[swapPosX][swapPosY] = matrix[x][y];
                    matrix[x][y] = temp;
                }
            }
            swapPosY++;
        }
        startingPosX++;
        startingPosY++;
    }
}

void parallel_rotate(std::vector<std::vector<char>>& matrix, int n) {
    int maxPos = n - 1;

    #pragma omp parallel for
    for (int rotationWidth = n; rotationWidth > 1; rotationWidth -= 2) {
        int swapPos = (n - rotationWidth) / 2;
        int startingPosX = swapPos;
        int startingPosY = swapPos;

        #pragma omp parallel for
        for (int i = 0; i < rotationWidth - 1; i++) {
            int tempSwapPosY = swapPos + i;
            for (int j = 0; j < 3; j++) {
                if (j == 0) {
                    char temp = matrix[swapPos][tempSwapPosY];
                    int x = i + swapPos;
                    int y = maxPos - startingPosX;
                    matrix[swapPos][tempSwapPosY] = matrix[x][y];
                    matrix[x][y] = temp;
                }
                else if (j == 1) {
                    char temp = matrix[swapPos][tempSwapPosY];
                    int x = maxPos - startingPosX;
                    int y = maxPos - tempSwapPosY;
                    matrix[swapPos][tempSwapPosY] = matrix[x][y];
                    matrix[x][y] = temp;
                }
                else if (j == 2) {
                    char temp = matrix[swapPos][tempSwapPosY];
                    int x = maxPos - i - swapPos;
                    int y = startingPosY;
                    matrix[swapPos][tempSwapPosY] = matrix[x][y];
                    matrix[x][y] = temp;
                }
            }
        }
    }
}

int main() {
    const int size = 10000;
    std::vector<std::vector<char>> matrix(size, std::vector<char>(size, '_'));

    // Fill in the arrow shape
    int mid = size / 2;
    int offset = 0;
    for (int i = mid; i < size; i++) {
        for (int j = mid - offset; j <= mid + offset; j++) {
            matrix[i][j] = '*';
        }
        offset++;
    }
    /*
    // Display the matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }*/

    double normal_start = omp_get_wtime();
    normal_rotate(matrix, size);
    double normal_end = omp_get_wtime();
    double normal_elapsed = normal_end - normal_start;

    /*
    // Display the matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }*/

    double parallel_start = omp_get_wtime();
    parallel_rotate(matrix, size);
    double parallel_end = omp_get_wtime();
    double parallel_elapsed = parallel_end - parallel_start;

    /*
    // Display the matrix
    for (int k = 0; k < size; k++) {
        for (int l = 0; l < size; l++) {
            std::cout << matrix[k][l] << ' ';
        }
        std::cout << std::endl;
    }*/
    
    // Print the elapsed time for each function
    std::cout << "Time taken for normal_rotate: " << normal_elapsed << " seconds" << std::endl;
    std::cout << "Time taken for parallel_rotate: " << parallel_elapsed << " seconds" << std::endl;

    return 0;
}
