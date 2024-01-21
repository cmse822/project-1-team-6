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

<span style="color:red">Group 6 warm-up results are in the table below:</span>

|Kernel|Operations|Memory access|Arithmetic Intensity|
|------|----------|-------------|--------------------|
|Y[j]+=Y[j]+A[j][i]*B[i]|3|4|3/4(8)bytes|
|s+=A[i]*A[i]|2|2|1/8bytes|
|s+=A[i]*B[i]|2|3|2/3(8)bytes|
|Y[i]=A[i]+C*B[i]|2|3|2/3(8) bytes|

## Part 1: Matrix-matrix Multiplication

In this first part of the project, you will test the performance of the basic matrix-matrix multiplication operation as a function of matrix size on multiple compute platforms. Complete the following using at least two different compute architectures (e.g., your laptop and HPCC, or two different node architectures on HPCC).

1. With your group, write a program that multiplies two matrices together (see: <http://mathworld.wolfram.com/MatrixMultiplication.html>). Use GitHub to manage the code development and version history. 
   
   <span style="color:red">Done.</span>

2. For a given matrix size _N_, what is the total number of floating point operations performed by this operator?

    <span style="color:red">The total number of floating point operations peformed by this operator is `2N^3` since there are 2 operations in the inner loop (addition and multiplication) and 3 loops, each iterating to size N.</span>

3. Using the supplied C routine `get_walltime.c`, or any other accurate means of measuring time, compute the performance in Mflop/s of the matrix-matrix multiply for _N_=100\. Be sure to perform enough repeat calculations of the timing to overcome any statistical noise in the measurement.

    <span style="color:red">For `N=100` and using the supplied C routine, the performance of the matrix-matrix multiply is: `3312.133 Mflop/s`. This was repeated `20` times to eliminate any statistical noise.</span>

4. For the system you are running on, determine the clock speed of the processor and the cache size/layout. Use this information to estimate the theoretical peak performance of the system, assuming that the processor is capable of one flop per clock cycle (generally NOT true on modern architectures). How does the performance you measured in (3) compare to the theoretical peak performance of your system? 
       
      <span style="color:red">Below is the breakdown of our processor speed, cache size/layout, and the theoretical peak performance: </span>

      | Name   | Processor          | L1 Instruction Cache | L1 Data Cache | L2 Cache | L3 Cache | Clock Speed | Cores | FPU (Assume 1) | Theoretical Peak Performance | Hardware Source |
      |--------|--------------------|-----------------------|---------------|----------|----------|-------------|-------|----------------|-------------------------------|-----------------|
      | Chris  | 11th Gen i7-1185G7 | 48 KB per core        | 32 KB per core | 256 KB per core | 12 MB shared | 3.00 GHz    | 4     | 1              | **12 GFLOPS** | [Source](https://www.cpubenchmark.net/cpu.php?cpu=Intel+Core+i7-1185G7+%40+3.00GHz&id=3793) |

      
5. Now repeat the performance measurement for a range of matrix size `N` from 1 to 10,000,000. Make a plot of the resulting measured Gflop/s vs. `N`. On this plot place a horizontal line representing the theoretical peak performance based upon your system's clock speed.

      <span style="color:red">The image below is the peak vs achieved peformance results:</span>

      ![Performance Image](/peak_vs_achieved_performance.png)

6. How does the measured performance for multiple _N_'s compare to peak? Are there any "features" in your plot? Explain them in the context of the hardware architecture of your system. Include in your write-up a description of your system's architecture (processor, cache, etc.).
   
      <span style="color:red">The measured performance is far off from the peak performance. The peak of the achieved performance is close to 4 Gflop/s, which is roughly 1/3 of the theoretical peak performance of the system. The achieved performance is best in the beginning when data is read into the L1 cache, but drops off later when the L1 cache size is reached and the data needs to be retireved from the L2 cache. Again, we see this negative slope happen later when the L2 cache then reaches it's size limit and data needs to be grabbed from the L3 cache.
      </span>

To your project git repo, commit your code for performing the matrix-matrix multiply performance measurements, the plots of your results, and a brief write-up (in plain text or markdown) addressing the above questions and discussing your results. Pay particular attention to the comparison between different architectures and give explanations for them.

## Part 2: The Roofline Model 

In this part, you will explore the roofline model for analyzing the interplay between arithmetic intensity and memory bandwidth for architectures with complex memory hierarchies. Complete the following exercises on the _SAME_ compute architectures that you used in Part 1 above. 

1. Reference the materials on the Roofline Performance model at <https://crd.lbl.gov/divisions/amcr/computer-science-amcr/par/research/roofline/>. In particular, look through ["Roofline: An Insightful Visual Performance Model for Floating-Point Programs and Multicore Architectures"](https://www2.eecs.berkeley.edu/Pubs/TechRpts/2008/EECS-2008-134.pdf) and the slides at <https://crd.lbl.gov/assets/pubs_presos/parlab08-roofline-talk.pdf>.
2. Clone the CS Roofline Toolkit, `git clone https://bitbucket.org/berkeleylab/cs-roofline-toolkit.git`. Modify one of the config files in `Empirical_Roofline_Tool-1.1.0/Config` as necessary for the machine you are using.
3. Run the ERT in serial mode on your local machine. Report the peak performances and bandwidths (for all caches levels as well as DRAM). Where is the "ridge point" of the roofline for the various cases?
4. Consider the four FP kernels in "Roofline: An Insightful Visual Performance Model for Floating-Point Programs and Multicore Architectures" (see their Table 2). Assuming the high end of operational (i.e., "arithmetic") intensity, how would these kernels perform on the platforms you are testing? What optimization strategy would you recommend to increase performance of these kernels?
5. Address the same questions in (4) for the four kernels given in the Warm-up above. 
6. Compare your results for the roofline model to what you obtained for the matrix-matrix multiplication operation from Part 1. How are the rooflines of memory bandwidth related to the features in the algorithmic performance as a function of matrix size?

To your project write-up, add your plots of the roofline model for the systems you tested, and responses addressing the above questions. 

