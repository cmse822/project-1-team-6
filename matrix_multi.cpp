#include <iostream>
#include <vector>
#include <stdexcept>
#include <random>

extern "C" {
#include "get_walltime.c"
}

#define N 100

double lower_bound = 0.0f;
double upper_bound = 10000.0f;
std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
std::default_random_engine re;

std::vector<std::vector<double>> MatMul(  const std::vector<std::vector<double>>& a, 
                        const std::vector<std::vector<double>>& b) {    
    const auto& rows = a.size();
    const auto& cols = a[0].size();

    if (a.empty() || b.empty()) {
        throw std::runtime_error("Incoming matrix should not be empty!");
        exit(1);
    }

    if(rows != cols){
        throw std::runtime_error("Incoming matrix should be square!");
        exit(1);
    }


    std::vector<std::vector<double>> c(a[0].size(), std::vector<double>(a[0].size()));

    for (size_t i = 0; i < a.size(); i++) {
        for(size_t j=0; j < a[0].size(); j++){
            for(size_t k=0; k < a[0].size(); k++){
                c[i][j] += a[i][k] * b[k][j];   
            }
        }
    }
    return c;
}


template <typename T>
void PrintMatrix(std::vector<std::vector<T>> matrix){
    
    const auto& rows = matrix.size();
    const auto& cols = matrix[0].size();
    
    printf("###### Matrix #######\n");
    for (size_t i=0; i<rows; i++){
        printf("[");
        for (size_t j=0; j<cols; j++)
            printf("%f ", matrix[i][j]);
        printf("]\n");
    }
}

double GenerateRandomNumber(){
    double a_random_double = unif(re);
    return a_random_double;
}

int main() {
    try {
        double start_time;
        double end_time;


        std::vector<std::vector<double>> mat1(N, std::vector<double>(N));
        std::vector<std::vector<double>> mat2(N, std::vector<double>(N));

        
        printf("Starting the program with %dx%d matrices\n", N, N);

        // mat1 = {{1, 2},
        //         {3, 4}};
        

        // mat2 = {{1, 2},
        //         {3, 4}};
        

        for(size_t j=0; j < mat1[0].size(); j++){
            for(size_t k=0; k < mat1[0].size(); k++){
                mat1[j][k] = GenerateRandomNumber();
                mat2[j][k] = GenerateRandomNumber();
            }
        }
        

        get_walltime(&start_time);
        auto res = MatMul(mat1, mat2);
        get_walltime(&end_time);

        // PrintMatrix(res);

        // Peak Performance:
        // ClockSpeed(GHz) * TicksPerSecond * Number of Cores * FPUs

        // ACTIONS TODO:
        // 1. Q4: Calculate our machine theoretical performance (processor) and caches size (L1, L2, etc.)
        // 2. Q2: We believe that it should be N^3
        // 3. Q3: We just do ~100 times for N=100 and collect the execution times and we will have Mflop/s
        // 4. Q4: Let's discuss it tomorrow what would knowing the cache size/layout actually helps us for calculating the peak performance.
        // 5. Q6: Just a writeup explaining architecture of your system and pointing it out in the chart we create

        // Part 2 notes:
        // 1. 

        double time_it_takes = end_time - start_time;
        printf("Finished the program with %f seconds\n", time_it_takes);

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}
