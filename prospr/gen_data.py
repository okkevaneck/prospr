#!/usr/bin/env python3
"""
File:           gen_data.py
Description:    This file generates data for testing different H-ratios.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

import os
import numpy as np
import random
import math


def new_folder(folder_name):
    """
    Create a new dataset folder and return the path.
    """
    root = "data/"
    dirs = [x[0] for x in os.walk(root)]

    if f"{root}{folder_name}" in dirs:
        raise ValueError(f"Dataset '{folder_name}' already exists.")

    os.mkdir(f"{root}{folder_name}")
    return f"{root}{folder_name}"


def generate_hratio(p_len=25, size=300):
    """
    Generates the H-ratio dataset given the provided arguments. The provided
    H-ratio range is including h_ratio_begin, and excluding h_ratio_end.
    :param int      p_len:          Protein length of dataset.
    :param float    h_ratio_begin:  H-ratio fraction range begin (including).
    :param float    h_ratio_end:    H-ratio fraction range end (excluding).
    :param float    h_ratio_step:   H-ratio fraction range step.
    :param int      size:           Number of proteins per H-ratio.
    """
    aminos = ["H", "P"]
    ds_path = "data/vanEck_hratio"

    # Create dataset folder. Throws ValueError if dataset already exists.
    if not os.path.isdir(ds_path):
        os.makedirs(ds_path, exist_ok=True)

    # Construct a list with lower and upper bounds for the h-ratio.
    h_ratio_space = np.arange(0.3, 1.0, 0.1)
    h_ratio_range = [(0.0, 0.2)]
    for h_ratio in h_ratio_space[:-1]:
        h_ratio_range.append((round(h_ratio - 0.1, 1), round(h_ratio, 1)))
    h_ratio_range.append((0.8, 1.0))

    # Check if unique dataset of given size is possible given protein length.
    if (
        len(aminos) ** p_len < size
        or math.comb(p_len, math.floor(0.8 * p_len)) < size
    ):
        print(f"Cannot produce {size} unique proteins of length {p_len}.")
        exit(-1)

    for h_ratio_low, h_ratio_high in h_ratio_range:
        cur_fname = f"{''.join(aminos)}{p_len}_r{h_ratio_high}.csv"

        # Only generate dataset if it doesn't exist already.
        if os.path.isfile(f"{ds_path}/{cur_fname}"):
            print(f"Dataset '{cur_fname}' already exists.")
            continue

        with open(f"{ds_path}/{cur_fname}", "w") as fp:
            cur_set = set()
            print(f"\nGenerating set with h-ratio of {h_ratio_high}..")
            fp.write("id,sequence\n")

            # Print debug length every 10 iterations.
            i = 0

            # Generate only unique proteins for this set.
            while len(cur_set) < size:
                new_proteins = set(
                    [
                        "".join(
                            random.choices(
                                aminos,
                                weights=[
                                    min(h_ratio_high, 0.9),
                                    max(round(1 - h_ratio_high, 1), 0.1),
                                ],
                                k=p_len,
                            )
                        )
                        for _ in range(size)
                    ]
                )

                # Filter newly generated proteins on the H-ratio requirements.
                new_proteins = set(
                    [
                        p
                        for p in new_proteins
                        if p.count("P") != 0
                        and h_ratio_low < p.count("H") / p_len <= h_ratio_high
                    ]
                )
                cur_set = cur_set.union(new_proteins)

                # Print debug if needed, update tracker.
                if i % 150 == 0:
                    print(f"    {h_ratio_high} - cur_size:  {len(cur_set)}")
                    print("\tFilter:   " f"({h_ratio_low}, {h_ratio_high}]")
                    print(
                        f"\tWeights:  [{min(h_ratio_high, 0.9)}, "
                        f"{max(round(1 - h_ratio_high, 1), 0.1)}]"
                    )
                i += 1

            # Write newly generated set to file.
            for i, p in enumerate(cur_set):
                fp.write(f"{i},{p}\n")


if __name__ == "__main__":
    # Fetch arguments from user required for generating data.
    print(
        "Requesting needed information.\nLeave blank for the default " "value."
    )
    print("==================================", end="\n\n")

    p_len_str = input("Protein length (default=25): ").strip()
    p_len = int(p_len_str) if p_len_str else 25

    size_str = input("Number of proteins per set (default=300): ").strip()
    size = int(size_str) if size_str else 300

    # Generate the data using the provided arguments.
    generate_hratio(p_len, size)
