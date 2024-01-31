#!/bin/bash
echo -e "=================================\n| CMSE-822 | Team-6 | Project-1 |\n=================================\n"

rm data.csv 2> /dev/null
# Compile and link
g++ matrix_multi.cpp -lstdc++ -O3 -g -std=c++17 -o matmul

for ((i=1; i<=200; i=i+5))
do 
    taskset -c 3 ./matmul $i
    echo -e "\n=======\n"
done


for ((i=200; i<=500; i=i+20))
do 
    taskset -c 3 ./matmul $i
    echo -e "\n=======\n"
done


for ((i=600; i<=2000; i=i+100))
do 
    taskset -c 3 ./matmul $i
    echo -e "\n=======\n"
done

python3 plot_performance.py

exit 1