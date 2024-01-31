import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

FILENAME = "data.csv"
#FILENAME = "data_hpcc.csv"

def plot_performance_results(filename):
    df = pd.read_csv(filename)
    n_values = df['matrix_size'].astype(np.float32).sort_values()
    peak_performance = df['peak_performance'].astype(np.float32)
    achieved_performance = df['achieved_performance'].astype(np.float32)

    plt.scatter(n_values, achieved_performance, label="Achieved Performance", color="red", marker="o")
    plt.plot(n_values, achieved_performance, color='red', alpha=0.5, marker = 'o') 
    #plt.hlines(y=peak_performance[0], xmin=n_values.min(), xmax=n_values.max(), label="Peak Performance", color="blue", linestyles='dashed')
    

    plt.xlabel("Matrix Size (N)")
    plt.ylabel("GFLOPS")
    #plt.title("Peak vs. Achieved Performance")
    plt.title("Achieved Performance")
    #plt.legend(loc="center right", fancybox=True, shadow=True)
    #plt.savefig('peak_vs_achieved_performance.png', bbox_inches='tight')
    plt.savefig('achieved_performance.png', bbox_inches='tight')
    plt.show()


if __name__ == "__main__":
    plot_performance_results(FILENAME)
