from prospr import Protein, plot_protein
import matplotlib.pyplot as plt
import sys


class TestVisualization:
    def test_2d_visualization(self, show=False):
        """
        Test the 2D plotting in all different styles.
        """
        p = Protein("HPPHPPHPPH", dim=2, model="HP")
        p.set_hash([-1, -2, 1, -2, 1, 2, 1, 2, -1])
        plot_protein(p, style="paper", show=False)

        p = Protein("HPPHPPHPPH", dim=2, model="HP")
        p.set_hash([-1, -2, 1, -2, 1, 2, 1, 2, -1])
        plot_protein(p, style="paper", legend_style="outer", show=False)

        p = Protein("HPPHPPHPPH", dim=2, model="HP")
        p.set_hash([-1, -2, 1, -2, 1, 2, 1, 2, -1])
        plot_protein(
            p,
            style="paper",
            markersize=16,
            linewidth=1,
            fontsize=8,
            show=False,
        )

        p = Protein("HPPHPPHPPH", dim=2, model="HP")
        p.set_hash([-1, -2, 1, -2, 1, 2, 1, 2, -1])
        plot_protein(p, style="basic", show=False)

        p = Protein("HPPHPPHPPH", dim=2, model="HP")
        p.set_hash([-1, -2, 1, -2, 1, 2, 1, 2, -1])
        plot_protein(
            p,
            style="basic",
            markersize=20,
            linewidth=0.8,
            fontsize=8,
            show=False,
        )

        p = Protein("PHPHHPPPHHPHPHH", dim=2, model="HP")
        p.set_hash([-1, -1, 2, 2, -1, 2, 2, 1, -2, 1, -2, 1, -2, -1])
        plot_protein(p, style="basic", legend=False, show=False)

        p = Protein("HPPHPPPHPPPPPHH", dim=2, model="HP")
        p.set_hash([-1, -2, 1, -2, 1, 2, 2, 1, 2, 2, -1, -1, -2, 1])
        plot_protein(
            p,
            style="paper",
            legend_style="inner",
            annotate_first=True,
            show=False,
        )

        p = Protein("PHPPPHPPPPPHH", dim=2, model="HP")
        p.set_hash([1, -2, 1, 2, 2, 1, 2, 2, -1, -1, -2, 1])
        plot_protein(
            p,
            style="paper",
            legend_style="inner",
            annotate_first=True,
            show=False,
        )

        p = Protein("HHPHPHPPPHPHHHPPPPPPHPPPH", dim=2, model="HP")
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
        p = Protein("HPPHPPHH", dim=3, model="HP")
        p.set_hash([1, 2, -1, 3, -2, -1, -3])
        plot_protein(p, style="paper", show=False)

        p = Protein("HPHPPPPP", dim=3, model="HP")
        p.set_hash([-1, 2, 3, 1, -2, 1, -3])
        plot_protein(p, style="paper", show=False)

        p = Protein("HPPHPPHH", dim=3, model="HP")
        p.set_hash([1, 2, -1, 3, -2, -1, -3])
        plot_protein(p, style="basic", show=False)

        p = Protein("HPPHPPHH", dim=3, model="HP")
        p.set_hash([1, 2, -1, 3, -2, -1, -3])
        plot_protein(p, style="paper", legend=False, show=False)

        p = Protein("HPPHPPHH", dim=3, model="HP")
        p.set_hash([1, 2, -1, 3, -2, -1, -3])
        plot_protein(p, style="paper", legend_style="outer", show=False)

        if show:
            plt.show()


if __name__ == "__main__":
    # Check for given arguments.
    show = False
    test_2d = True
    test_3d = True

    if len(sys.argv) > 1 and sys.argv[1] == "show":
        show = True
    if (len(sys.argv) > 2 and sys.argv[2] == "not_2d") or (
        len(sys.argv) > 3 and sys.argv[3] == "not_2d"
    ):
        test_2d = False
    if (len(sys.argv) > 2 and sys.argv[2] == "not_3d") or (
        len(sys.argv) > 3 and sys.argv[3] == "not_3d"
    ):
        test_3d = False

    # Execute tests as instructed.
    test_vis_inst = TestVisualization()

    if test_2d:
        print("\tExecuting 2D visualization tests..")
        test_vis_inst.test_2d_visualization(show=show)
    else:
        print("\tSkipping 2D visualization tests..")

    if test_3d:
        print("\tExecuting 3D visualization tests..")
        test_vis_inst.test_3d_visualization(show=show)
    else:
        print("\tSkipping 3D visualization tests..")
