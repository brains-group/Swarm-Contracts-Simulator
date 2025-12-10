#!/usr/bin/env python3

import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Compare profit distributions from two CSV files."
    )
    parser.add_argument("random_csv", help="CSV file with Random results")
    parser.add_argument("swarm_csv", help="CSV file with Swarm Contracts results")
    parser.add_argument(
        "-o", "--output",
        default="profit_histogram.png",
        help="output image filename (default: profit_histogram.png)",
    )
    args = parser.parse_args()

    # Read CSVs
    random_df = pd.read_csv(args.random_csv, skipinitialspace=True)
    swarm_df = pd.read_csv(args.swarm_csv, skipinitialspace=True)

    # Extract profit columns
    random_profit = random_df["profit"]
    swarm_profit = swarm_df["profit"]

    # ---- COMMON BINS HERE ----
    # Combine both profit series and compute shared bin edges
    combined_profits = pd.concat([random_profit, swarm_profit])
    bins = np.histogram_bin_edges(combined_profits, bins="auto")
    # --------------------------

    plt.figure(figsize=(8, 5))

    plt.hist(
        random_profit,
        bins=bins,
        alpha=0.5,
        label="Random",
        color="black",
        edgecolor="black",
    )
    plt.hist(
        swarm_profit,
        bins=bins,
        alpha=0.5,
        label="Swarm Contracts",
        color="#d6001c",
        edgecolor="#d6001c",
    )

    plt.xlabel("Profit")
    plt.ylabel("Frequency")
    plt.title("Profit Distribution: Random vs Swarm Contracts")
    plt.legend()
    plt.tight_layout()

    # Save instead of showing (works in non-GUI environments)
    plt.savefig(args.output, dpi=300)


if __name__ == "__main__":
    main()

