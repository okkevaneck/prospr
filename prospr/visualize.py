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

# Global defaults for correct scaling.
BASIC_MARKERSIZE = 80
BASIC_LINEWIDTH = 2
PAPER_MARKERSIZE = 210
PAPER_LINEWIDTH = 2.5
PAPER_EDGEWIDTH = 2.5

# Global colors.
COLOR_H = "tab:blue"
COLOR_P = "orange"
COLOR_ANNOTATE = "#ff00ff"
MARKER_ANNOTATE = "D"
COLOR_CONTACT = "indianred"
COLOR_CHAIN = "black"


def _plot_aminos_2d_basic(
    protein, df, ax, linewidth, markersize, annotate_first=False
):
    """
    Plot amino acids in basic style in a 2D figure.
    :param Protein      protein:        Protein object to plot the hash of.
    :param DataFrame    df:             DataFrame with all ordered positions.
    :param Axes         ax:             Axis to plot on.
    :param float        linewidth:      Width of the lines.
    :param float        markersize:     Size of the markers.
    :param bool         annotate_first: Annotate first amino acid.
    """
    # Plot protein chain.
    ax.plot(
        df["x"], df["y"], color=COLOR_CHAIN, alpha=0.65, zorder=1, lw=linewidth
    )

    # Plot all aminos except first as it might be annotated.
    sns.scatterplot(
        x="x",
        y="y",
        data=df.iloc[1:],
        hue="Type",
        hue_order=["H", "P"],
        style="Type",
        markers={"H": "o", "P": "s"},
        palette={"H": COLOR_H, "P": COLOR_P},
        s=markersize,
        zorder=2,
        ax=ax,
    )

    # Plot the first amino acid as a diamond when annotating it.
    if df.iloc[0]["Type"] == "H":
        facecolor = COLOR_H
        edgecolor = COLOR_H
        marker_first = "o"
    elif df.iloc[0]["Type"] == "P":
        facecolor = COLOR_P
        edgecolor = COLOR_P
        marker_first = "s"

    if annotate_first:
        edgecolor = COLOR_ANNOTATE
        marker_first = MARKER_ANNOTATE

    ax.scatter(
        df.iloc[0]["x"],
        df.iloc[0]["y"],
        fc=facecolor,
        ec=edgecolor,
        linewidth=2,
        marker=marker_first,
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
            color=COLOR_CONTACT,
            alpha=0.9,
            zorder=1,
            lw=linewidth,
        )


def _plot_aminos_2d_paper(
    protein, df, ax, linewidth, markersize, annotate_first=False
):
    """
    Plot amino acids in paper style in a 2D figure.
    :param Protein      protein:        Protein object to plot the hash of.
    :param DataFrame    df:             DataFrame with all ordered positions.
    :param Axes         ax:             Axis to plot on.
    :param float        linewidth:      Width of the lines.
    :param float        markersize:     Size of the markers.
    :param bool         annotate_first: Annotate first amino acid.
    """
    # Scale edgewidth according to markersize scale.
    edgewidth = PAPER_EDGEWIDTH
    if markersize != PAPER_MARKERSIZE:
        edgewidth *= markersize / PAPER_MARKERSIZE
    if markersize < PAPER_MARKERSIZE:
        edgewidth *= 2

    # Plot protein chain.
    ax.plot(
        df["x"],
        df["y"],
        color=COLOR_CHAIN,
        alpha=0.65,
        linewidth=linewidth,
        zorder=1,
    )

    # Plot everything but the first amino acid as it might be annotated.
    sns.scatterplot(
        x="x",
        y="y",
        data=df.iloc[1:].loc[df["Type"] == "H"],
        marker="o",
        facecolor=COLOR_H,
        edgecolor=COLOR_H,
        s=markersize,
        zorder=2,
        ax=ax,
        label="H",
    )
    sns.scatterplot(
        x="x",
        y="y",
        data=df.iloc[1:].loc[df["Type"] == "P"],
        marker="o",
        facecolor="white",
        edgecolor=COLOR_P,
        linewidth=edgewidth,
        s=markersize,
        zorder=2,
        ax=ax,
        label="P",
    )

    # Plot the first amino acid as a diamond when annotating it.
    if df.iloc[0]["Type"] == "H":
        edgecolor = COLOR_H
        facecolor = COLOR_H
    elif df.iloc[0]["Type"] == "P":
        edgecolor = COLOR_P
        facecolor = "white"
    marker_first = "o"

    if annotate_first:
        marker_first = MARKER_ANNOTATE
        facecolor = edgecolor
        edgecolor = COLOR_ANNOTATE

    ax.scatter(
        df.iloc[0]["x"],
        df.iloc[0]["y"],
        marker=marker_first,
        fc=facecolor,
        ec=edgecolor,
        lw=edgewidth,
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
            color=COLOR_CONTACT,
            alpha=0.9,
            linewidth=linewidth,
            zorder=1,
        )

    # Remove axis, and position legend in the upper right with created space.
    ax.axis("off")


def _plot_aminos_3d_basic(
    protein, df, ax, linewidth, markersize, annotate_first=False
):
    """
    Plot amino acids in basic style in a 3D figure.
    :param Protein      protein:        Protein object to plot the hash of.
    :param DataFrame    df:             DataFrame with all ordered positions.
    :param Axes         ax:             Axis to plot on.
    :param float        linewidth:      Width of the lines.
    :param float        markersize:     Size of the markers.
    :param bool         annotate_first: Annotate first amino acid.
    """
    # Plot protein chain.
    ax.plot(
        df["x"],
        df["y"],
        df["z"],
        color="black",
        alpha=0.65,
        zorder=1,
        lw=linewidth,
    )

    # Plot all except the first amino acid, as it might be annotated.
    ax.scatter(
        df.iloc[1:].loc[df["Type"] == "H"]["x"],
        df.iloc[1:].loc[df["Type"] == "H"]["y"],
        df.iloc[1:].loc[df["Type"] == "H"]["z"],
        c=COLOR_H,
        marker="o",
        depthshade=False,
        s=markersize,
        label="H",
    )
    ax.scatter(
        df.iloc[1:].loc[df["Type"] == "P"]["x"],
        df.iloc[1:].loc[df["Type"] == "P"]["y"],
        df.iloc[1:].loc[df["Type"] == "P"]["z"],
        c=COLOR_P,
        marker="s",
        depthshade=False,
        s=markersize,
        label="P",
    )

    # Plot the first amino acid as a diamond when annotating it.
    if df.iloc[0]["Type"] == "H":
        facecolor = COLOR_H
        edgecolor = COLOR_H
    elif df.iloc[0]["Type"] == "P":
        facecolor = COLOR_P
        edgecolor = COLOR_P
    marker_first = "o"

    if annotate_first:
        edgecolor = COLOR_ANNOTATE
        marker_first = MARKER_ANNOTATE

    ax.scatter(
        df.iloc[0]["x"],
        df.iloc[0]["y"],
        df.iloc[0]["z"],
        fc=facecolor,
        ec=edgecolor,
        linewidth=2.0,
        marker=marker_first,
        s=markersize,
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
            color=COLOR_CONTACT,
            alpha=0.9,
            zorder=1,
            lw=linewidth,
        )


def _plot_aminos_3d_paper(
    protein, df, ax, linewidth, markersize, annotate_first=False
):
    """
    Plot amino acids in paper style in a 3D figure.
    :param Protein      protein:    Protein object to plot the hash of.
    :param DataFrame    df:         DataFrame with all ordered positions.
    :param Axes         ax:         Axis to plot on.
    """
    # Scale edgewidth according to markersize scale.
    edgewidth = PAPER_EDGEWIDTH
    if markersize != PAPER_MARKERSIZE:
        edgewidth *= markersize / PAPER_MARKERSIZE
    if markersize < PAPER_MARKERSIZE:
        edgewidth *= 2

    # Plot protein chain.
    ax.plot(
        df["x"],
        df["y"],
        df["z"],
        color=COLOR_CHAIN,
        alpha=0.65,
        linewidth=linewidth,
        zorder=1,
    )

    # Plot everything but the first amino acid as it might be annotated.
    ax.scatter(
        df.iloc[1:].loc[df["Type"] == "H"]["x"],
        df.iloc[1:].loc[df["Type"] == "H"]["y"],
        df.iloc[1:].loc[df["Type"] == "H"]["z"],
        marker="o",
        edgecolor=COLOR_H,
        s=markersize,
        zorder=2,
        label="H",
        depthshade=False,
    )
    ax.scatter(
        df.iloc[1:].loc[df["Type"] == "P"]["x"],
        df.iloc[1:].loc[df["Type"] == "P"]["y"],
        df.iloc[1:].loc[df["Type"] == "P"]["z"],
        marker="o",
        facecolor="white",
        edgecolor=COLOR_P,
        lw=edgewidth,
        s=markersize,
        zorder=2,
        label="P",
        depthshade=False,
    )

    # Plot the first amino acid as a diamond when annotating it.
    if df.iloc[0]["Type"] == "H":
        edgecolor = COLOR_H
        facecolor = COLOR_H
    if df.iloc[0]["Type"] == "P":
        edgecolor = COLOR_P
        facecolor = "white"
    marker_first = "o"

    if annotate_first:
        marker_first = MARKER_ANNOTATE
        facecolor = edgecolor
        edgecolor = COLOR_ANNOTATE

    ax.scatter(
        df.iloc[0]["x"],
        df.iloc[0]["y"],
        df.iloc[0]["z"],
        marker=marker_first,
        fc=facecolor,
        ec=edgecolor,
        lw=edgewidth,
        s=markersize,
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
            color=COLOR_CONTACT,
            alpha=0.9,
            zorder=1,
            lw=linewidth,
        )

    # Remove axis, and position legend in the upper right with created space.
    ax.axis("off")


def plot_protein(
    protein,
    style="basic",
    ax=None,
    legend=True,
    legend_style="inner",
    fontsize=13,
    show=True,
    linewidth=None,
    markersize=None,
    annotate_first=False,
):
    """
    Plot conformation of a protein.
    :param Protein  protein:        Protein object to plot the hash of.
    :param [str]    style:          What style to plot the proteins in.
    :param Axes     ax:             Axis to plot Protein on.
    :param bool     legend:         True if a legend needs to be added.
    :param str      legend_style:   Either 'inner' or 'outer'.
    :param str      fontsize:       Fontsize to be used for all text.
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

    # Catch unsupported styles.
    supported_styles = ["basic", "paper"]
    if style not in supported_styles:
        raise RuntimeError(
            f"Style '{style}' not supported. Choose from {supported_styles}."
        )

    # Set markersize and line width according to provided style.
    if style == "basic":
        if not markersize:
            markersize = BASIC_MARKERSIZE
        if not linewidth:
            linewidth = BASIC_LINEWIDTH
    elif style == "paper":
        if not markersize:
            markersize = PAPER_MARKERSIZE
        if not linewidth:
            linewidth = PAPER_LINEWIDTH

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
            ax = fig.add_subplot(projection="3d")

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
            _plot_aminos_3d_paper(
                protein, df, ax, linewidth, markersize, annotate_first
            )
    elif style == "basic":
        ax.set_xlabel("x-axis", fontsize=fontsize)
        ax.set_ylabel("y-axis", fontsize=fontsize)
        ax.xaxis.set_major_locator(MaxNLocator(integer=True))
        ax.yaxis.set_major_locator(MaxNLocator(integer=True))

        # Plot dimension specific.
        if protein.dim == 2:
            ax.set_title(f"2D conformation with {protein.score} energy")
            _plot_aminos_2d_basic(
                protein, df, ax, linewidth, markersize, annotate_first
            )
        else:
            ax.set_title(f"3D conformation with {protein.score} energy")
            ax.set_zlabel("z-axis", fontsize=fontsize)
            ax.zaxis.set_major_locator(MaxNLocator(integer=True))
            _plot_aminos_3d_basic(
                protein, df, ax, linewidth, markersize, annotate_first
            )

    # Transform axis limits to make protein segments squared.
    ax.axis("equal")

    # If adding legend, remove title from legend and add item for bonds.
    if legend:
        handles, labels = ax.get_legend_handles_labels()

        # Only add bond label if actually in plot.
        if protein.score != 0:
            score_patch = Line2D(
                [],
                [],
                color=COLOR_CONTACT,
                linestyle=":",
                linewidth=linewidth,
                alpha=0.9,
                label="Contact",
            )
            handles.append(score_patch)
            labels.append(score_patch.get_label())

        # Only add annotation label if option is enabled.
        if annotate_first:
            annotate_patch = Line2D(
                [0],
                [0],
                marker=MARKER_ANNOTATE,
                linestyle="None",
                markerfacecolor="white",
                markeredgecolor=COLOR_ANNOTATE,
                markeredgewidth=1.75,
                markersize=10,
                label="Head",
            )
            handles.append(annotate_patch)
            labels.append(annotate_patch.get_label())

        # Style legend according to plotting style.
        if (
            style == "paper" and legend_style == "outer"
        ) or legend_style == "outer":
            box = ax.get_position()
            ax.set_position((box.x0, box.y0, box.width * 0.8, box.height))
            ax.legend(
                handles=handles,
                labels=labels,
                loc="center left",
                bbox_to_anchor=(1, 0.5),
                fontsize=fontsize,
            )
        else:
            ax.legend(handles=handles, labels=labels, fontsize=fontsize)
    elif protein.dim == 2:
        if legend := ax.get_legend():
            legend.remove()

    # Show plot if specified.
    if show:
        plt.show()
