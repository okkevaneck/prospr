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


def generate_hratio(p_len=25, size=1000):
    """
    Generates the H-ratio dataset given the provided arguments. The provided
    H-ratio range is including h_ratio_begin, and excluding h_ratio_end.
    NOTE: This script has a 1-off bug for lenght 10 H-ratio 1.0!
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
    h_ratio_range = zip(
        np.round(np.arange(0.0, 1.0, 0.1), 2),
        np.round(np.arange(0.1, 1.1, 0.1), 2),
    )

    # Keep track of all produced proteins to prevent duplicates across files.
    all_proteins = set()
    prev_size_all = 0

    for h_ratio_low, h_ratio_high in h_ratio_range:
        cur_fname = f"{''.join(aminos)}{p_len}_r{h_ratio_high}.csv"

        # Only generate dataset if it doesn't exist already.
        if os.path.isfile(f"{ds_path}/{cur_fname}"):
            print(f"Dataset '{cur_fname}' already exists.")
            continue

        # Track number of consecutive iterations no new proteins were produced.
        no_changes = 0

        # Compute maximum of given size and theoretically possible.
        if (
            len(aminos) ** p_len < size
            or math.comb(p_len, math.floor(h_ratio_high * p_len)) < size
        ):
            print(
                f"\n\nCannot produce {size} unique proteins of length {p_len} "
                f"with upper H-ratio of {h_ratio_high}"
            )
            cur_size = math.comb(p_len, math.floor(h_ratio_high * p_len))
            print(f"Producing maximally possible {cur_size} proteins instead.")
        else:
            cur_size = size
            print(
                f"\n\nProducing {cur_size} unique proteins of length {p_len} "
                f"with upper h-ratio of {h_ratio_high}"
            )

        with open(f"{ds_path}/{cur_fname}", "w") as fp:
            cur_set = set()
            fp.write("id,sequence\n")

            # Print debug length every 10 iterations.
            i = 0

            # Generate only unique proteins for this set.
            while len(cur_set) < cur_size:
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
                        for _ in range(cur_size)
                    ]
                )

                # Filter newly generated proteins on the H-ratio requirements
                # and on previously generated proteins.
                new_proteins = set(
                    [
                        p
                        for p in new_proteins
                        if p.count("P") != 0
                        and h_ratio_low < p.count("H") / p_len <= h_ratio_high
                    ]
                )
                new_proteins -= all_proteins
                all_proteins = all_proteins.union(new_proteins)
                cur_set = cur_set.union(new_proteins)

                # Put cut-off at 1000 iterations of no changes.
                if len(all_proteins) == prev_size_all:
                    no_changes += 1

                    if no_changes == 1000:
                        print("Cutting off due to no changes..")
                        break
                else:
                    prev_size_all = len(all_proteins)
                    no_changes = 0

                # Print debug if needed, update tracker.
                if i % 250 == 0:
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

    size_str = input("Maximum #proteins per set (default=1000): ").strip()
    size = int(size_str) if size_str else 1000

    # Generate the data using the provided arguments.
    generate_hratio(p_len, size)
