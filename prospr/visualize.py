#!/usr/bin/env python3
"""
File:       visualize.py
Author:     Okke van Eck

Description:    This file contains functions for visualizing a Protein instance.
"""

from .helpers import get_ordered_positions, get_scoring_pairs

from matplotlib import pyplot as plt
from matplotlib.ticker import MaxNLocator
from matplotlib.lines import Line2D
import seaborn as sns
import pandas as pd


def plot_protein_structure_2d(protein, ax):
    """

    :param protein:
    :param ax:
    """
    # Setup dataframe containing the data and set types for the coordinates.
    df = pd.DataFrame(get_ordered_positions(protein),
                      columns=["x", "y", "Type"])
    df = df.astype({"x": "int32", "y": "int32"})

    ax.plot(df["x"], df["y"], color="black", alpha=0.65, zorder=1)
    sns.scatterplot(x="x", y="y", data=df, hue="Type", hue_order=["H", "P"],
                    style="Type", markers={"H": "o", "P": "s"},
                    palette={"H": "royalblue", "P": "orange"},
                    s=80, zorder=2, ax=ax)

    # Plot dotted lines between the aminos that increase the stability.
    pairs = get_scoring_pairs(protein)

    for pos1, pos2 in pairs:
        ax.plot([pos1[0], pos2[0]], [pos1[1], pos2[1]], linestyle=":",
                color="indianred", alpha=0.9, zorder=1, lw=1.5)

    # Set axis labels.
    ax.set_title(f"2D conformation with {protein.get_score()} energy")
    ax.set_xlabel("x-axis", fontsize=13)
    ax.set_ylabel("y-axis", fontsize=13)
    ax.xaxis.set_major_locator(MaxNLocator(integer=True))
    ax.yaxis.set_major_locator(MaxNLocator(integer=True))

    # Remove title from legend and add item for bonds.
    handles, labels = ax.get_legend_handles_labels()
    score_patch = Line2D([], [], color="indianred", linestyle=":", alpha=0.9,
                         label="Contact", lw=1.5)
    handles.append(score_patch)
    labels.append(score_patch.get_label())
    ax.legend(handles=handles[1:], labels=labels[1:])


def plot_protein_structure_3d(protein, ax):
    """

    :param protein:
    :param ax:
    """
    # Setup dataframe containing the data and set types for the coordinates.
    df = pd.DataFrame(get_ordered_positions(protein),
                      columns=["x", "y", "z", "Type"])
    df = df.astype({"x": "int32", "y": "int32", "z": "int32"})

    # Split dataframe on amino acid type.
    df_H = df.loc[df["Type"] == "H"]
    df_P = df.loc[df["Type"] == "P"]

    # Plot the aminos connected with an opaque line.
    ax.scatter(df_H["x"], df_H["y"], df_H["z"], c="royalblue", marker="o",
               depthshade=False, s=60, label="H")
    ax.scatter(df_P["x"], df_P["y"], df_P["z"], c="orange", marker="s",
               depthshade=False, s=60, label="P")
    ax.plot(df["x"], df["y"], df["z"], color="black", alpha=0.65, zorder=1)

    # Plot dotted lines between the aminos that increase the stability.
    pairs = get_scoring_pairs(protein)

    for pos1, pos2 in pairs:
        ax.plot([pos1[0], pos2[0]], [pos1[1], pos2[1]], [pos1[2], pos2[2]],
                linestyle=":", color="indianred", alpha=0.9, zorder=1, lw=1.5)

    # Set axis labels and tics.
    ax.set_title(f"3D conformation with {protein.get_score()} energy")
    ax.set_xlabel("x-axis", fontsize=13)
    ax.set_ylabel("y-axis", fontsize=13)
    ax.set_zlabel("z-axis", fontsize=13)
    ax.xaxis.set_major_locator(MaxNLocator(integer=True))
    ax.yaxis.set_major_locator(MaxNLocator(integer=True))
    ax.zaxis.set_major_locator(MaxNLocator(integer=True))

    # Remove title from legend and add item for bonds.
    handles, labels = ax.get_legend_handles_labels()
    score_patch = Line2D([], [], color="indianred", linestyle=":", alpha=0.9,
                         label="Contact", lw=1.5)
    handles.append(score_patch)
    labels.append(score_patch.get_label())
    ax.legend(handles=handles, labels=labels)


def plot_protein_structure(protein):
    """
    Plot conformation of a protein.
    :param Protein      protein:        Protein object to plot the hash of.
    """
    fig = plt.figure(figsize=(6, 5))
    sns.set_style("whitegrid")

    # Plot data according to used dimension.
    dim = protein.get_dim()

    if dim == 2:
        ax = fig.gca()
        plot_protein_structure_2d(protein, ax)
    elif dim == 3:
        ax = fig.gca(projection="3d")
        plot_protein_structure_3d(protein, ax)
    else:
        raise RuntimeError(f"Cannot plot the structure of a protein with "
                           f"dimension '{dim}'")

    plt.show()
