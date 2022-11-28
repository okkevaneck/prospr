from prospr import Protein, plot_protein
import matplotlib.pyplot as plt


if __name__ == "__main__":
    p = Protein("HPHPPPPPPP", model="HP")
    p.set_hash([-1, -1, 2, 2, 2, 2, 2, 2, 2])
    plot_protein(p, style="paper", show=False)

    p = Protein("HPPHPPHPPH", model="HP")
    p.set_hash([-1, -2, 1, -2, 1, 2, 1, 2, -1])
    plot_protein(p, style="paper", show=False)

    p = Protein("PHPHHPPPHHPHPHH", model="HP")
    p.set_hash([-1, -1, 2, 2, -1, 2, 2, 1, -2, 1, -2, 1, -2, -1])
    plot_protein(p, style="basic", legend=False, show=False)

    p = Protein("PHPHHPPPHHPHPHH", model="HP")
    p.set_hash([-1, -1, 2, 2, -1, 2, 2, 1, -2, 1, -2, 1, -2, -1])
    plot_protein(p, style="paper", legend_style="inner", show=False)

    p = Protein("HPPHPPPHPPPPPHH", model="HP")
    p.set_hash([-1, -2, 1, -2, 1, 2, 2, 1, 2, 2, -1, -1, -2, 1])
    plot_protein(p, style="paper", legend_style="inner", show=False)

    p = Protein("HHPHPHPPPHPHHHPPPPPPHPPPH", model="HP")
    p.set_hash(
        [
            -1,
            -2,
            1,
            1,
            2,
            1,
            2,
            2,
            -1,
            -2,
            -1,
            2,
            -1,
            2,
            2,
            2,
            -1,
            -2,
            -2,
            -2,
            -1,
            -2,
            1,
            1,
        ]
    )
    plot_protein(p, style="paper", show=False)

    plt.show()
