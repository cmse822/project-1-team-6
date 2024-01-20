import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Step: On CPP side, export information to CSV
# TODO: Save as CSV then Plot results
# Chris will work on


# data.csv looks like this:
# 

FILENAME = "data.csv"

def plot_performance_results(filename):
    df = pd.read_csv(filename)
    n_values = df['matrix_size'].astype(np.float32)
    peak_performance = df['peak_performance'].astype(np.float32)
    achieved_performance = df['achieved_performance'].astype(np.float32)
    plt.plot(n_values, achieved_performance, label="Achieved Peformance", color="red")
    plt.axhline(y=peak_performance[0], label="Peak Performance", color="blue")
    plt.xlabel("Matrix Size (N)")
    plt.ylabel("GFLOPS")
    plt.title("Peak vs. Achieved Performance")
    plt.legend()
    plt.savefig('test.png')
    plt.show()


if __name__ == "__main__":
    plot_performance_results(FILENAME)