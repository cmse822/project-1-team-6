[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/z6CYgFxj)

# Project 1: Memory Hierarchies and Performance of Serial Applications

## Due: EOD, 23 January

## Learning goals

In this project you will explore how memory hierarchies affect performance for serial applications.

- arithmetic intensity: how to compute; divides; memory access
- at least 2 different system architectures. Compare and contrast.

All of these aspects of serial applications are transferable to parallel applications, as we shall see later in the course.

## Warm-up

Review the section in [HPSC](../assets/EijkhoutIntroToHPC2020.pdf) on computing arithmetic intensity for given compute kernels.
Then, as a group, compute the arithmetic intensities of the following kernels in units of FLOPs/byte, assuming 8 bytes per float.

```C
  Y[j] += Y[j] + A[j][i] * B[i]
```

```C
  s += A[i] * A[i]
```

```C
  s += A[i] * B[i]
```

```C
  Y[i] = A[i] + C*B[i]
```

Included a table in your project report listing the arithmetic intensities for these kernels.

Group 6 warm-up results are in the table below:

|Kernel|Operations|Memory access|Arithmetic Intensity|
|------|----------|-------------|--------------------|
|Y[j]+=Y[j]+A[j][i]*B[i]|3|4|3/4(8)bytes = 3/32 FLOPs/byte|
|s+=A[i]*A[i]|2|1|2/1(8)bytes = 1/4 FLOPs/byte|
|s+=A[i]*B[i]|2|2|2/2(8)bytes = 1/8 FLOPs/byte|
|Y[i]=A[i]+C*B[i]|2|3|2/3(8) bytes = 1/12 FLOPS/byte|

## Part 1: Matrix-matrix Multiplication

In this first part of the project, you will test the performance of the basic matrix-matrix multiplication operation as a function of matrix size on multiple compute platforms. Complete the following using at least two different compute architectures (e.g., your laptop and HPCC, or two different node architectures on HPCC).

1. With your group, write a program that multiplies two matrices together (see: <http://mathworld.wolfram.com/MatrixMultiplication.html>). Use GitHub to manage the code development and version history.

   Done.

2. For a given matrix size _N_, what is the total number of floating point operations performed by this operator?

    The total number of floating point operations peformed by this operator is `2N^3` since there are 2 operations in the inner loop (addition and multiplication) and 3 loops, each iterating to size N.

3. Using the supplied C routine `get_walltime.c`, or any other accurate means of measuring time, compute the performance in Mflop/s of the matrix-matrix multiply for _N_=100\. Be sure to perform enough repeat calculations of the timing to overcome any statistical noise in the measurement.

    For `N=100` and using the supplied C routine, the performance of the matrix-matrix multiply for both compute architectures is listed below:

    - **LAPTOP**: `6562.484 Mflop/s`

    - **HPCC dev-amd20**: `7906.695 Mflop/s`

    This was repeated `20` times to eliminate any statistical noise.

4. For the system you are running on, determine the clock speed of the processor and the cache size/layout. Use this information to estimate the theoretical peak performance of the system, assuming that the processor is capable of one flop per clock cycle (generally NOT true on modern architectures). How does the performance you measured in (3) compare to the theoretical peak performance of your system?

      Below is the breakdown of our processor speed, cache size/layout, and the theoretical peak performance:

      | Name   | Processor          | L1 Instruction Cache | L1 Data Cache | L2 Cache | L3 Cache | Clock Speed | Cores | FPU (Assume 1) | Theoretical Peak Performance | Hardware Source |
      |--------|--------------------|-----------------------|---------------|----------|----------|-------------|-------|----------------|-------------------------------|-----------------|
      | Onur Laptop  | 11th Gen i7-1185G7 | 3KB per core        | 48 KB per core | 1280 KB per core | 24 MB shared | 4.20 GHz    | 4     | 1              | **33.6 GFLOPS** | [Source](https://www.cpubenchmark.net/cpu.php?cpu=Intel+Core+i7-1185G7+%40+3.00GHz&id=3793) |
      | HPCC dev-amd20  | AMD EPYC 7H12 64-Core Processor | 32 KB per core        | 32 KB per core | 512 KB per core | 256 MB shared | 2.60 GHz    | 64     | 1              | **166.4 GFLOPS** | [Source](https://www.amd.com/en/products/cpu/amd-epyc-7h12) |

    For the laptop, the theoretical peak peformance was much higher then the performance measured in question 3. We believe this is because of the other processes running on the laptop that are also taking up a lot of the processors utilization and limiting the performance potential.

    For HPCC, the theoretical peak performance was also much higher then the performance in question 3. Again, this could be because of the other processes running on the system, taking up some resources.

5. Now repeat the performance measurement for a range of matrix size `N` from 1 to 10,000,000. Make a plot of the resulting measured Gflop/s vs. `N`. On this plot place a horizontal line representing the theoretical peak performance based upon your system's clock speed.

      The image below is the peak vs achieved peformance results for the two compute architectures:

      **LAPTOP**

      ![Local Performance Image](local_system_results/peak_vs_achieved_performance.png)

      ![Local Performance Image](local_system_results/achieved_perf.png)

      **HPCC dev-amd20**

      ![HPCC dev-amd20 Performance Image](hpcc_system_results/peak_vs_achieved_performance.png)
   
      ![HPCC dev-amd20 Performance Image](hpcc_system_results/achieved_perf_hpcc.png)

7. How does the measured performance for multiple _N_'s compare to peak? Are there any "features" in your plot? Explain them in the context of the hardware architecture of your system. Include in your write-up a description of your system's architecture (processor, cache, etc.).

      The measured performance is far off from the peak performance. The peak of the achieved performance is close to 8 Gflop/s, which is roughly 1/4 of the theoretical peak performance of the system. The achieved performance is best in the beginning when data is read into the L1 cache, but drops off at around 1100 matrix size when the L1 cache size is reached and the data needs to be retireved from the L2 cache. Again, we see this big drop off around 1300 when the L2 cache then reaches it's size limit and data needs to be grabbed from the L3 cache. Finally, the final drop occurs around 1600 when the L3 cache size is reached and it needs to go out to DRAM.

      The features are similar to that in the HPCC architecture, where the performance is best in the beginning with smaller matrix sizes, but continues to degrade overtime as each level of the cache sizes fill up.

To your project git repo, commit your code for performing the matrix-matrix multiply performance measurements, the plots of your results, and a brief write-up (in plain text or markdown) addressing the above questions and discussing your results. Pay particular attention to the comparison between different architectures and give explanations for them.

## Part 2: The Roofline Model

In this part, you will explore the roofline model for analyzing the interplay between arithmetic intensity and memory bandwidth for architectures with complex memory hierarchies. Complete the following exercises on the _SAME_ compute architectures that you used in Part 1 above.

1. Reference the materials on the Roofline Performance model at <https://crd.lbl.gov/divisions/amcr/computer-science-amcr/par/research/roofline/>. In particular, look through ["Roofline: An Insightful Visual Performance Model for Floating-Point Programs and Multicore Architectures"](https://www2.eecs.berkeley.edu/Pubs/TechRpts/2008/EECS-2008-134.pdf) and the slides at <https://crd.lbl.gov/assets/pubs_presos/parlab08-roofline-talk.pdf>.

    Done.

2. Clone the CS Roofline Toolkit, `git clone https://bitbucket.org/berkeleylab/cs-roofline-toolkit.git`. Modify one of the config files in `Empirical_Roofline_Tool-1.1.0/Config` as necessary for the machine you are using.

    Done.

3. Run the ERT in serial mode on your local machine. Report the peak performances and bandwidths (for all caches levels as well as DRAM). Where is the "ridge point" of the roofline for the various cases?

    **LAPTOP**

    The charts below were generated when running the empirical roofline tool to give us the bandwidths and peak performances for each memory hierarchy level :

    ![Bandwidth Model Image](https://github.com/cmse822/project-1-team-6/blob/main/local_system_results/empirical_peak_performance_bandwidth.png)
   
    ![Peak Performance Model Image](https://github.com/cmse822/project-1-team-6/blob/main/local_system_results/empirical_peak_performance_gflops.png)
   
    ![Empirical Roofline Graph](https://github.com/cmse822/project-1-team-6/blob/main/local_system_results/roofline.png)

    Below is the roofline model that was generated using the roofline visualizer. This was using the imported JSON that was generated when running the empiracle roofline tool:

    ![Annotated Roofline Model](https://github.com/cmse822/project-1-team-6/blob/main/local_system_results/roofline_model.png)

    As shown above, we can see the compute bound and memory bound sections of the graph. The ridge point is the minimum operational intensity required to achieve maximum performance. This is also the point where the memory hierarchies can become compute bound.

    These results are reported in the table below, along with the peak performance for each cache level (and DRAM), and the bandwidths:

    |                       |L1|L2|L3|DRAM|
    |-----------------------|---|---|---|----|
    | **Ridge Point (Flops/Byte)** | 0.22  | 0.26  | 0.36  | 1.99   |
    | **Peak Performance (GFLOP/s)** | 62.08  | 62.08  | 62.08  | 62.08  |
    | **Bandwidths (GB/s)** | 288.40  | 238.60  | 172.60  | 31.10   |

    ---

    **HPCC-dev-amd20**

    Below is the results repeated for the second architecture of choice (HPCC)

    ![Bandwidth Model Image](https://github.com/cmse822/project-1-team-6/blob/main/hpcc_system_results/empirical_peak_performance_bandwidth.png)
   
    ![Peak Performance Model Image](https://github.com/cmse822/project-1-team-6/blob/main/hpcc_system_results/empirical_peak_performance_gflops.png)
   
    ![Empirical Roofline Graph](https://github.com/cmse822/project-1-team-6/blob/main/hpcc_system_results/roofline.png)
   
    ![Annotated Roofline Model](https://github.com/cmse822/project-1-team-6/blob/main/hpcc_system_results/roofline_model.png)
   

    |                       |L1|L2|L3|DRAM|
    |-----------------------|---|---|---|----|
    | **Ridge Point (Flops/Byte)** | 0.25  | 0.58  | 0.74  | 0.98   |
    | **Peak Performance (GFLOP/s)** | 25.52  | 25.52  | 25.52  | 25.52   |
    | **Bandwidths (GB/s)** | 101.30  | 44.30  | 34.50  | 26.10   |

4. Consider the four FP kernels in "Roofline: An Insightful Visual Performance Model for Floating-Point Programs and Multicore Architectures" (see their Table 2). Assuming the high end of operational (i.e., "arithmetic") intensity, how would these kernels perform on the platforms you are testing? What optimization strategy would you recommend to increase performance of these kernels?

    If we consider Sparse Matrix-Vector Multiplication(SpMV), we can see that amount of data that are unnecessarily computed, stored or loaded from memory is huge. That's also mentioned in the [Paper], suggesting that conventional implementations are less than 10% of what the system can do. There are lots of representations regarding to the calculations that includes the sparse matrices. Most of those representations try to compress the matrix itself into a couple of vectors so that only not-zero data can stay in the memory and operated on.

    One example of those representations is Compressed Row Storage (CRS). CRS generates 3 different vectors, namely value vector, column index vector and row pointer vector. Value vector only consists of the non-zero floating points and the others are integer vectors since they store addresses or indices.

    CRS travels the matrix in a rowwise fashion and stores the non-zero floating points into contiguous memory locations to increase the affinity of temporal-locality. In parallel, it also stores the found non-zero floating point number's row pointer and column index to the respective vectors.

    In the end, amount of storage we succesfully ignored depends on the sparsity of the matrix itself. While normal representation of those matrices fills up the space with $n^2$, with CRS, we only require $2nnz + n + 1$. Access to an element from this compressed matrix requires more attention. Many more libraries are using CRS or a variant of a CRS to represent and operate with matrices and Eigen[https://eigen.tuxfamily.org/dox/group__TutorialSparse.html] is a good example

5. Address the same questions in (4) for the four kernels given in the Warm-up above.

    1. **Kernel 1**
       1. Kernel has Arithmetic Intensity of the $3/32 = 0.09375$ which shows that the kernel is memory-bound for both systems.
       2. We can also infer that most of the time processor spends time waiting for memory load/store operations rather than doing floating point operations
       3. If we swap the $j$ and $i$ index variables for the for loop, we can increase the affinity of spatial-locality.
    2. **Kernel 2**
       1. Kernel has Arithmetic Intensity of the $1/4 = 0.25$ which shows that the kernel is memory-bound for both systems.
       2. For comparison with the other kernels, we see that kernel has higher Arithmetic Intensity and it is past the L1 Cache's ridge point (for HPCC, but same as ridge point for laptop), which is $0.22$. However, if it is before the L2 Cache's ridge point, which is $0.26$. If this kernel only uses L1 cache, it is compute bound. If it goes to L2 cache, then it is memory bound. It depends on the size of the vectors.
       3. We can use loop-unrolling here to utilize the pipelining for our system.
    3. **Kernel 3**
       1. Kernel has Arithmetic Intensity of the $1/8 = 0.125$ which shows that the kernel is memory-bound for both systems.
       2. We can use loop-unrolling here to utilize the pipelining for our system.
    4. **Kernel 4**
       1. Kernel has Arithmetic Intensity of the $1/12 = 0.083$ which shows that the kernel is memory-bound for both systems.
       2. We can use loop-unrolling here to utilize the pipelining for our system.

6. Compare your results for the roofline model to what you obtained for the matrix-matrix multiplication operation from Part 1. How are the rooflines of memory bandwidth related to the features in the algorithmic performance as a function of matrix size?

    They are related in that the same dropoffs in memory bandwidth also happens when cache sizes (larger N) are reached and data needs to be fetched from lower levels of the memory hierarchy. We can see that in the algorithmic performance charts of our two systems, where after a certain matrix size, the performance degrades. This is very noticeable when we reported the achieved performance and compared it to the attainable from the roofline model. Looking at our matrix-matrix multiplication algorithm, we have 2 FLOPS and 3 memory accesses (1 store, 2 reads) per iteration with 8 bytes each, putting us at an operational intensity of around 0.0833. Lining that up to the roofline model shows us that we are memory bound, no matter what memory access level we are at. If this is the case, then our bottleneck is on memory bandwidth, and we see the dropoff in performance become correlated to the levels of cache we access. This makes sense physically as the L2, L3 and DRAM are further away from the CPU then L1 cache, so bandwidth is degraded and latency is increased. The bandwidth peaks for each level in question 3 above further proves that bandwidth performance degrades. The nail in the coffin is knowing that we are memory bound, and seeing the GFLOPs dropoff at each cache level in Part 1, which is directly impacted not because of CPU performance, but because of data transfer limits inbetween the memory levels.

To your project write-up, add your plots of the roofline model for the systems you tested, and responses addressing the above questions.

<br>
<br>

---

# Running Project 1

## Pre-requisites

- gcc or g++
- Python 3

## Getting the Matrix-Matrix Multiplication Results

To run the C++ program, simply run the following two commands

```bash
gcc -g -o matrix_mul .\matrix_multi.cpp -lstdc++ -Wall -O3
.\matrix_mul.exe 100
```

This will create an executable named `matrix_mul.exe` in the root directory that takes an optional argument, `N`, which is the size of the matrices you want to multiply together. The argument is optional (default size N=100), and needs to be a numeric value between 1 and 10,000.

## Plotting the Results with Python

The C++ program will save each run to a CSV file. The information saved is the matrix size (N), peak performance, and achieved performance. This information is used to plot the peak vs. achieved performance.

To plot the results, run the following in the root directory:

```python
python plot_performance.py
```

This will save an image to the root directory, which is showing how the achieved performance compares to the systems peak performance and how it differs over the range of results you ran for the C++ program.

## Deliverables

Our deliverables are already found in the GitHub repository. Locate the following:

- Code for performing the matrix-matrix multiply performance measurements: `matrix_multi.cpp`
- Plot of the performance and roofline model results, located in: `local_system_results/` and `hpcc_system_results/`
- Brief write-up: In this `README.md` filling in the questions for Part 1 and Part 2.
