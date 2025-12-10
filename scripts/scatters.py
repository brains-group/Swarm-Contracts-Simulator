#!/usr/bin/env python3
import argparse
import pandas as pd
import matplotlib.pyplot as plt


def main() -> None:
    parser = argparse.ArgumentParser(
        description=(
            "Create side-by-side scatter plots comparing Random vs Swarm Contracts:\n"
            "  1) Profit vs Total Ability (skill_sum)\n"
            "  2) Profit vs Completed Contracts (num_contracts)"
        )
    )
    parser.add_argument("random_csv", help="CSV file with Random results")
    parser.add_argument("swarm_csv", help="CSV file with Swarm Contracts results")
    parser.add_argument(
        "-o",
        "--output",
        default="profit_scatter.png",
        help="output image filename (default: profit_scatter.png)",
    )
    args = parser.parse_args()

    # Read CSVs; skipinitialspace handles spaces after commas in headers
    random_df = pd.read_csv(args.random_csv, skipinitialspace=True)
    swarm_df = pd.read_csv(args.swarm_csv, skipinitialspace=True)

    # Extract columns
    random_skill = random_df["skill_sum"]
    random_profit = random_df["profit"]
    random_contracts = random_df["num_contracts"]

    swarm_skill = swarm_df["skill_sum"]
    swarm_profit = swarm_df["profit"]
    swarm_contracts = swarm_df["num_contracts"]

    # Set up side-by-side plots with shared y-axis (profit)
    fig, (ax1, ax2) = plt.subplots(
        1, 2, figsize=(10, 5), sharey=True, constrained_layout=True
    )

    # ---- Left: Profit vs Total Ability (skill_sum) ----
    ax1.scatter(
        random_skill,
        random_profit,
        label="Random",
        color="black",
        alpha=0.6,
        s=15,
    )
    ax1.scatter(
        swarm_skill,
        swarm_profit,
        label="Swarm Contracts",
        color="#d6001c",
        alpha=0.6,
        s=15,
    )
    ax1.set_xlabel("Total Ability")
    ax1.set_ylabel("Profit")
    ax1.set_title("Profit vs Total Ability")
    ax1.legend()

    # ---- Right: Profit vs Completed Contracts (num_contracts) ----
    ax2.scatter(
        random_contracts,
        random_profit,
        label="Random",
        color="black",
        alpha=0.6,
        s=15,
    )
    ax2.scatter(
        swarm_contracts,
        swarm_profit,
        label="Swarm Contracts",
        color="#d6001c",
        alpha=0.6,
        s=15,
    )
    ax2.set_xlabel("Completed Contracts")
    ax2.set_title("Profit vs Completed Contracts")

    # Save to file (works in non-interactive environments)
    fig.suptitle("Profit Comparison: Random vs Swarm Contracts", fontsize=14)
    fig.savefig(args.output, dpi=300)


if __name__ == "__main__":
    main()
