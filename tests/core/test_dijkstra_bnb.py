#!/usr/bin/env python3
"""
File:           test_dijkstra_bnb.py
Description:    This file contains the pytest tests for the Dijkstra
                branch-and-bound search core code.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2022). See the LICENSE file for the
                specifics.
"""

from prospr import Protein, dijkstra_bnb
import pytest


@pytest.fixture()
def protein_2d():
    return Protein("PHPHPHPPH", 2, model="HP")


# @pytest.fixture()
# def protein_3d():
#     return Protein("HPPHPPH", 3, model="HP")


@pytest.mark.order(order=2)
class TestDepthFirstBnB:
    def test_protein_2d_dijkstra_bnb(self, protein_2d):
        """
        Test if a 2D protein is folded correctly using depth_first_bnb search.
        """
        p = dijkstra_bnb(protein_2d)
        assert p.score == -3
        assert p.solutions_checked == 740
        assert p.aminos_placed == 1168

    # def test_protein_3d_dijkstra_bnb(self, protein_3d):
    #     """
    #     Test if a 3D protein is folded correctly using depth_first_bnb
    #     search.
    #     """
    #     p = dijkstra_bnb(protein_3d)
    #     assert p.score == -2
    #     assert p.solutions_checked == 5
    #     assert p.aminos_placed == 49368
