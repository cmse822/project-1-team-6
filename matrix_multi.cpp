#include <iostream>
#include <vector>
#include <stdexcept>
#include <random>
extern "C" {
#include "get_walltime.c"
}

#define N 100
#define REPEAT 1000




struct ComputeComponent{
    std::string name;
    int num_cores;
    float clock_speed_ghz;
    int num_fpus;
};



double lower_bound = 0.0f;
double upper_bound = 10000.0f;
std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
std::default_random_engine re;

void MatMul(    const   std::vector<std::vector<double>>& a, 
                const   std::vector<std::vector<double>>& b,
                        std::vector<std::vector<double>>& c) {    
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


    for (size_t i = 0; i < a.size(); i++) {
        for(size_t k=0; k < a[0].size(); k++){
            for(size_t j=0; j < a[0].size(); j++){
                c[i][j] += a[i][k] * b[k][j];   
            }
        }
    }
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


double CalculateMFLOPS(double total_time){
    double flop = 2 * N * N * N * REPEAT;       // total flop
    double mflops = flop / (total_time * 1e6f); // flops to mflops
    return mflops;
}

double CalculateTheoreticalPeakPerformance(const ComputeComponent& comp){

    double peak_perf = comp.num_cores * comp.num_fpus * comp.clock_speed_ghz * 1000.0f;
    return peak_perf;

}

// f(v1, v2) -> csv
// 


double GenerateRandomNumber(){
    double a_random_double = unif(re);
    return a_random_double;
}

int main() {
    

    std::vector<std::vector<double>> mat1(N, std::vector<double>(N));
    std::vector<std::vector<double>> mat2(N, std::vector<double>(N));
    
    
    printf("Size of double is: %u\n", sizeof(double));
    printf("Size of mat1 is: %u bytes\n", mat1.size() * mat1[0].size() * sizeof(double));


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


    double start_time = 0.0f;
    double end_time = 0.0f;
    double total_time = 0.0f;
    std::vector<std::vector<double>> result_mat(mat1[0].size(), std::vector<double>(mat1[0].size()));

    for(size_t i=0; i<REPEAT; i++){

        get_walltime(&start_time);
        MatMul(mat1, mat2, result_mat);
        get_walltime(&end_time);

        const double elapsed_time = end_time - start_time;
        total_time += elapsed_time;
    }

    printf("Total Seconds: %f\n", total_time);  // Uncomment future
    printf("Average Seconds: %f\n", total_time / REPEAT);  // Uncomment future

    double mflops = CalculateMFLOPS(total_time);
    ComputeComponent cc = {"Chris", 4, 3.0f, 1};
    const auto& peak_perf_chris = CalculateTheoreticalPeakPerformance(cc);

    printf("Theoretical Performance [%s] is: %f MFLOPS\n", cc.name.c_str(), peak_perf_chris); 


    // PrintMatrix(res);

    // Peak Performance:
    // ClockSpeed(GHz) * TicksPerSecond * Number of Cores * FPUs
    // Chris theoretical peak: 3.00 GHz * 4 * 1 flop per cycle = 12 GFLOPS = 12000 MFLOPS

    // ACTIONS TODO:
    // 1. Q4: Calculate our machine theoretical performance (processor) and caches size (L1, L2, etc.)
    // 2. Q2: We believe that it should be 2*N^3 -- (Chris) Add 2 because theres addition and multiplication in inner loop
    // 3. Q3: We just do ~100 times for N=100 and collect the execution times and we will have Mflop/s
    // 4. Q4: Let's discuss it tomorrow what would knowing the cache size/layout actually helps us for calculating the peak performance.
    // 5. Q6: Just a writeup explaining architecture of your system and pointing it out in the chart we create

    // Part 2 notes:
    // 1.

    printf("Total time took with %d repeats: %f Mflop/s\n", REPEAT, mflops);

    printf("Closing on the peak performance with %f\n", mflops / peak_perf_chris);
    
}
