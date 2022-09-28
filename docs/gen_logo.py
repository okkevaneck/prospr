import seaborn as sns
import pandas as pd
from prospr import Protein
from prospr.helpers import get_ordered_positions, get_scoring_pairs
from matplotlib import pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches


if __name__ == "__main__":
    protein = Protein("HPPHP", dim=2, bond_values={"H": -1, "P": 0})
    protein.set_hash([1, -2, -1, -2])
    fig = plt.figure(figsize=(14, 6))
    plt.axis("off")
    sns.set_style("whitegrid")
    ax = fig.gca()

    df = pd.DataFrame(
        get_ordered_positions(protein), columns=["x", "y", "Type"]
    )
    df = df.astype({"x": "int32", "y": "int32"})

    # Plot straight lines and add last amino acid.
    line_width = 8
    alpha_val = 0.85
    ax.plot(
        df["x"],
        df["y"],
        color="black",
        alpha=alpha_val,
        zorder=1,
        lw=line_width,
    )
    # last_amino = {"x": 3.5, "y": -0.75, "Type": "H"}
    last_amino = {"x": 5, "y": -0.80, "Type": "H"}
    df = df.append(last_amino, ignore_index=True)

    # Plot curvy line.
    verts = [
        (df.iloc[-2]["x"], df.iloc[-2]["y"]),
        (0.70, 0),
        # (1.375, -0.5),
        (last_amino["x"], last_amino["y"]),
    ]
    codes = [
        Path.MOVETO,
        Path.CURVE3,
        Path.CURVE3,
    ]
    path = Path(verts, codes)
    patch = patches.PathPatch(
        path,
        facecolor="none",
        edgecolor="black",
        lw=line_width,
        # alpha=alpha_val,
        alpha=0.7,
        zorder=1,
    )
    ax.add_patch(patch)

    # Add amino acids.
    sns.scatterplot(
        x="x",
        y="y",
        data=df,
        hue="Type",
        hue_order=["H", "P"],
        style="Type",
        markers={"H": "o", "P": "s"},
        palette={"H": "royalblue", "P": "orange"},
        s=280,
        linewidth=0,
        zorder=2,
        ax=ax,
    )

    # Plot dotted lines between the aminos that increase the stability.
    pairs = get_scoring_pairs(protein)

    for pos1, pos2 in pairs:
        ax.plot(
            [pos1[0], pos2[0]],
            [pos1[1], pos2[1]],
            linestyle="dashed",
            color="indianred",
            alpha=1,
            zorder=1,
            lw=line_width,
        )

    # Plot text.
    # ax.text(0.73, -1.88, "rospr", fontsize=121, fontstyle="italic")
    ax.text(
        0.60,
        -1.95,
        "rospr",
        size=225,
        style="italic",
        variant="small-caps",
        weight=5,
    )

    # Remove axis and legend.
    ax.get_xaxis().set_visible(False)
    ax.get_yaxis().set_visible(False)
    ax.get_legend().remove()

    fig.savefig(
        "source/_static/gen_logo.png", bbox_inches="tight", pad_inches=0
    )
    plt.show()
