from prospr import Protein, plot_protein
import matplotlib.pyplot as plt
import sys


class TestVisualization:
    def test_2d_visualization(self, show=False):
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

    def test_3d_visualization(self, show=False):
        """
        Test the 3D plotting in all different styles.
        """
        if show:
            plt.show()
        return


if __name__ == "__main__":
    # Check whether to show the proteins.
    show = False
    if len(sys.argv) > 1 and sys.argv[1] == "show":
        show = True

    test_vis_inst = TestVisualization()
    test_vis_inst.test_2d_visualization(show=show)
    test_vis_inst.test_3d_visualization(show=show)
