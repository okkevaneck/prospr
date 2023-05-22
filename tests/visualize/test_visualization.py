from prospr import Protein, plot_protein
import matplotlib.pyplot as plt


def test_2d_visualization(show=False):
    """
    Test the 2D plotting in all different styles.
    """
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

    if show:
        plt.show()


def test_3d_visualization(show=False):
    """
    Test the 3D plotting in all different styles.
    """
    if show:
        plt.show()


if __name__ == "__main__":
    test_2d_visualization()
    test_3d_visualization()
