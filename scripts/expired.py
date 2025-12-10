#!/usr/bin/env python3
import argparse
import pandas as pd
import matplotlib.pyplot as plt


def main() -> None:
    parser = argparse.ArgumentParser(
        description=(
            "Plot expired contracts vs frame number for Random and Swarm Contract Runs."
        )
    )
    parser.add_argument("random_csv", help="CSV file with Random results")
    parser.add_argument("greedy_csv", help="CSV file with Swarm Contracts results")
    parser.add_argument(
        "-o",
        "--output",
        default="expired_contracts_over_time.png",
        help="output image filename (default: expired_contracts_over_time.png)",
    )
    args = parser.parse_args()

    # Read CSVs
    random_df = pd.read_csv(args.random_csv, skipinitialspace=True)
    greedy_df = pd.read_csv(args.greedy_csv, skipinitialspace=True)

    # Make sure they're sorted by frameNumber
    random_df = random_df.sort_values("frameNumber")
    greedy_df = greedy_df.sort_values("frameNumber")

    # Extract columns
    random_frames = random_df["frameNumber"]
    random_expired = random_df["expiredContracts"]

    greedy_frames = greedy_df["frameNumber"]
    greedy_expired = greedy_df["expiredContracts"]

    # Plot
    plt.figure(figsize=(8, 5))

    plt.plot(
        random_frames,
        random_expired,
        label="Random",
        color="black",
        linewidth=1.5,
    )
    plt.plot(
        greedy_frames,
        greedy_expired,
        label="Swarm Contracts",
        color="#d6001c",
        linewidth=1.5,
    )

    plt.xlabel("Frame Number")
    plt.ylabel("Expired Contracts")
    plt.title("Expired Contracts Over Time: Random vs Swarm Contracts")
    plt.legend()
    plt.tight_layout()

    # Save to file (non-interactive safe)
    plt.savefig(args.output, dpi=300)


if __name__ == "__main__":
    main()
