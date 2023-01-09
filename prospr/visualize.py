#!/usr/bin/env python3
"""
File:           visualize.py
Description:    This file contains functions for visualizing a Protein
                instance.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

from .helpers import get_ordered_positions, get_scoring_pairs

from matplotlib import pyplot as plt
from matplotlib.ticker import MaxNLocator
from matplotlib.lines import Line2D
import seaborn as sns
import pandas as pd


def _plot_aminos_2d_basic(protein, df, ax):
    """
    Plot amino acids in basic style in a 2D figure.
    :param Protein      protein:    Protein object to plot the hash of.
    :param DataFrame    df:         DataFrame with all ordered positions.
    :param Axes         ax:         Axis to plot on.
    """
    ax.plot(df["x"], df["y"], color="black", alpha=0.65, zorder=1)
    sns.scatterplot(
        x="x",
        y="y",
        data=df,
        hue="Type",
        hue_order=["H", "P"],
        style="Type",
        markers={"H": "o", "P": "s"},
        palette={"H": "royalblue", "P": "orange"},
        s=80,
        zorder=2,
        ax=ax,
    )

    # Plot dotted lines between the aminos that increase the stability.
    pairs = get_scoring_pairs(protein)

    for pos1, pos2 in pairs:
        ax.plot(
            [pos1[0], pos2[0]],
            [pos1[1], pos2[1]],
            linestyle=":",
            color="indianred",
            alpha=0.9,
            zorder=1,
            lw=2,
        )


def _plot_aminos_2d_paper(
    protein, df, ax, linewidth, markersize, annotate_first=False
):
    """
    Plot amino acids in paper style in a 2D figure.
    :param Protein      protein:    Protein object to plot the hash of.
    :param DataFrame    df:         DataFrame with all ordered positions.
    :param Axes         ax:         Axis to plot on.
    :param float        linewidth:      Width of the lines.
    :param float        markersize:     Size of the markers.
    """
    # Split dataframe on amino acid type.
    df_H = df.loc[df["Type"] == "H"]
    df_P = df.loc[df["Type"] == "P"]

    ax.plot(
        df["x"],
        df["y"],
        color="black",
        alpha=0.65,
        linewidth=linewidth,
        zorder=1,
    )
    sns.scatterplot(
        x="x",
        y="y",
        data=df_H,
        marker="o",
        edgecolor="royalblue",
        s=markersize,
        zorder=2,
        ax=ax,
        label="H",
    )
    sns.scatterplot(
        x="x",
        y="y",
        data=df_P,
        marker="o",
        facecolor="white",
        edgecolor="orange",
        linewidth=2.5,
        s=markersize,
        zorder=2,
        ax=ax,
        label="P",
    )

    # Plot first point with a subscript 1.
    if annotate_first:
        # Plot first point with a different color.
        if df.iloc[0]["Type"] == "H":
            ax.scatter(
                df.iloc[0]["x"],
                df.iloc[0]["y"],
                marker="o",
                fc="royalblue",
                ec="#00ce00",
                lw=2.5,
                s=markersize,
                zorder=2,
            )
        else:
            ax.scatter(
                df.iloc[0]["x"],
                df.iloc[0]["y"],
                marker="o",
                fc="white",
                ec="#00ce00",
                lw=2.5,
                s=markersize,
                zorder=2,
            )

    # Plot dotted lines between the aminos that increase the stability.
    pairs = get_scoring_pairs(protein)

    for pos1, pos2 in pairs:
        ax.plot(
            [pos1[0], pos2[0]],
            [pos1[1], pos2[1]],
            linestyle=":",
            color="indianred",
            alpha=0.9,
            linewidth=linewidth,
            zorder=1,
        )

    # Remove axis, and position legend in the upper right with created space.
    ax.axis("off")


def _plot_aminos_3d_basic(protein, df, ax):
    """
    Plot amino acids in basic style in a 3D figure.
    :param Protein      protein:    Protein object to plot the hash of.
    :param DataFrame    df:         DataFrame with all ordered positions.
    :param Axes         ax:         Axis to plot on.
    """
    # Split dataframe on amino acid type.
    df_H = df.loc[df["Type"] == "H"]
    df_P = df.loc[df["Type"] == "P"]

    ax.plot(df["x"], df["y"], df["z"], color="black", alpha=0.65, zorder=1)

    # Plot the aminos connected with an opaque line.
    ax.scatter(
        df_H["x"],
        df_H["y"],
        df_H["z"],
        c="royalblue",
        marker="o",
        depthshade=False,
        s=60,
        label="H",
    )
    ax.scatter(
        df_P["x"],
        df_P["y"],
        df_P["z"],
        c="orange",
        marker="s",
        depthshade=False,
        s=60,
        label="P",
    )

    # Plot dotted lines between the aminos that increase the stability.
    pairs = get_scoring_pairs(protein)

    for pos1, pos2 in pairs:
        ax.plot(
            [pos1[0], pos2[0]],
            [pos1[1], pos2[1]],
            [pos1[2], pos2[2]],
            linestyle=":",
            color="indianred",
            alpha=0.9,
            zorder=1,
            lw=2,
        )


def _plot_aminos_3d_paper(protein, df, ax):
    """
    Plot amino acids in paper style in a 3D figure.
    :param Protein      protein:    Protein object to plot the hash of.
    :param DataFrame    df:         DataFrame with all ordered positions.
    :param Axes         ax:         Axis to plot on.
    """
    # Split dataframe on amino acid type.
    df_H = df.loc[df["Type"] == "H"]
    df_P = df.loc[df["Type"] == "P"]

    ax.plot(df["x"], df["y"], df["z"], color="black", alpha=0.65, zorder=1)

    sns.scatterplot(
        df_H["x"],
        df_H["y"],
        df_H["z"],
        data=df_H,
        marker="o",
        edgecolor="royalblue",
        s=60,
        zorder=2,
        ax=ax,
        label="H",
    )
    sns.scatterplot(
        df_P["x"],
        df_P["y"],
        df_P["z"],
        data=df_P,
        marker="o",
        facecolor="white",
        edgecolor="orange",
        linewidth=2,
        s=60,
        zorder=2,
        ax=ax,
        label="P",
    )

    # Plot first point with a different color.
    if df.iloc[0]["Type"] == "H":
        ax.scatter(
            df.iloc[0]["x"],
            df.iloc[0]["y"],
            marker="o",
            fc="royalblue",
            ec="#00ce00",
            lw=2.5,
            s=80,
            zorder=2,
        )
    else:
        ax.scatter(
            df.iloc[0]["x"],
            df.iloc[0]["y"],
            marker="o",
            fc="white",
            ec="#00ce00",
            lw=2.5,
            s=80,
            zorder=2,
        )

    # Plot dotted lines between the aminos that increase the stability.
    pairs = get_scoring_pairs(protein)

    for pos1, pos2 in pairs:
        ax.plot(
            [pos1[0], pos2[0]],
            [pos1[1], pos2[1]],
            [pos1[2], pos2[2]],
            linestyle=":",
            color="indianred",
            alpha=0.9,
            zorder=1,
            lw=2,
        )

    # Remove axis, and position legend in the upper right with created space.
    ax.axis("off")


def plot_protein(
    protein,
    style="basic",
    ax=None,
    legend=True,
    legend_style="inner",
    show=True,
    linewidth=2.5,
    markersize=210,
    annotate_first=False,
):
    """
    Plot conformation of a protein.
    :param Protein  protein:        Protein object to plot the hash of.
    :param [str]    style:          What style to plot the proteins in.
    :param Axes     ax:             Axis to plot Protein on.
    :param bool     legend:         True if a legend needs to be added.
    :param str      legend_style:   Either 'inner' or 'outer'.
    :param bool     show:           True if plot.show() needs to be called.
    :param float    linewidth:      Width of the lines.
    :param float    markersize:     Size of the markers.
    :param bool     annotate_first: True if first amino acids needs annotation.
    """
    # Catch unplottable dimensions.
    if protein.dim != 2 and protein.dim != 3:
        raise RuntimeError(
            f"Cannot plot the structure of a protein with "
            f"dimension '{protein.dim}'"
        )

    # Create axis to plot onto if not given.
    if ax is None:
        if style == "paper":
            fig = plt.figure(figsize=(5, 3.125))
        else:
            fig = plt.figure(figsize=(5, 6))
            sns.set_style("whitegrid")

        if protein.dim == 2:
            ax = fig.gca()
        else:
            ax = fig.gca(projection="3d")

    # Fetch data in right dimension.
    if protein.dim == 2:
        df = pd.DataFrame(
            get_ordered_positions(protein), columns=["x", "y", "Type"]
        )
        df = df.astype({"x": "int32", "y": "int32"})
    else:
        df = pd.DataFrame(
            get_ordered_positions(protein), columns=["x", "y", "z", "Type"]
        )
        df = df.astype({"x": "int32", "y": "int32", "z": "int32"})

    # Plot the selected style.
    if style == "paper":
        if protein.dim == 2:
            _plot_aminos_2d_paper(
                protein, df, ax, linewidth, markersize, annotate_first
            )
        else:
            _plot_aminos_3d_paper(protein, df, ax)
    elif style == "basic":
        ax.set_xlabel("x-axis", fontsize=13)
        ax.set_ylabel("y-axis", fontsize=13)
        ax.xaxis.set_major_locator(MaxNLocator(integer=True))
        ax.yaxis.set_major_locator(MaxNLocator(integer=True))

        # Plot dimension specific.
        if protein.dim == 2:
            ax.set_title(f"2D conformation with {protein.score} energy")
            _plot_aminos_2d_basic(protein, df, ax)
        else:
            ax.set_title(f"3D conformation with {protein.score} energy")
            ax.set_zlabel("z-axis", fontsize=13)
            ax.zaxis.set_major_locator(MaxNLocator(integer=True))
            _plot_aminos_3d_basic(protein, df, ax)

    # If adding legend, remove title from legend and add item for bonds.
    if legend:
        handles, labels = ax.get_legend_handles_labels()

        # Only add bond label if actually in plot.
        if protein.score != 0:
            score_patch = Line2D(
                [],
                [],
                color="indianred",
                linestyle=":",
                linewidth=linewidth,
                alpha=0.9,
                label="Bond",
            )
            handles.append(score_patch)
            labels.append(score_patch.get_label())

        # Style legend according to plotting style.
        if (
            style == "paper" and legend_style == "outer"
        ) or legend_style == "outer":
            box = ax.get_position()
            ax.set_position([box.x0, box.y0, box.width * 0.7, box.height])
            ax.legend(
                handles=handles,
                labels=labels,
                loc="upper left",
                bbox_to_anchor=(1, 1),
            )
        else:
            ax.legend(handles=handles, labels=labels, prop={"size": 12})
    else:
        ax.get_legend().remove()

    # Show plot if specified.
    if show:
        plt.show()
