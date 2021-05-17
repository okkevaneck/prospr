import seaborn as sns
import pandas as pd
from prospr import Protein
from prospr.helpers import get_ordered_positions, get_scoring_pairs
from matplotlib import pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches


if __name__ == "__main__":
    protein = Protein("HPPHP")
    protein.set_hash([1, -2, -1, -2])

    fig = plt.figure(figsize=(8, 5))
    sns.set_style("whitegrid")
    ax = fig.gca()

    df = pd.DataFrame(
        get_ordered_positions(protein), columns=["x", "y", "Type"]
    )
    df = df.astype({"x": "int32", "y": "int32"})

    ax.plot(df["x"], df["y"], color="black", alpha=0.65, zorder=1)
    last_amino = {"x": 4, "y": -0.5, "Type": "H"}
    df = df.append(last_amino, ignore_index=True)

    verts = [
        (df.iloc[-2]["x"], df.iloc[-2]["y"]),
        (1.5, -0.5),
        (last_amino["x"], last_amino["y"]),
    ]
    codes = [
        Path.MOVETO,
        Path.CURVE3,
        Path.CURVE3,
    ]
    path = Path(verts, codes)
    patch = patches.PathPatch(
        path, facecolor="none", edgecolor="black", lw=1.5, alpha=0.65, zorder=1
    )
    ax.add_patch(patch)

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
            lw=1.5,
        )

    # Remove axis and legend.
    ax.get_xaxis().set_visible(False)
    ax.get_yaxis().set_visible(False)
    ax.get_legend().remove()

    plt.show()
