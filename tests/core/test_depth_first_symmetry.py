#!/usr/bin/env python3
"""
File:           test_depth_first.py
Description:    This file contains the pytest tests for the depth_first_symmetry search
                core code.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

from prospr import Protein, depth_first, depth_first_symmetry
import pytest


@pytest.fixture()
def protein_2d():
    return Protein("PHPHPHPPH", dim=2, model="HP")


@pytest.fixture()
def protein_3d():
    return Protein("HPPHPHPHPH", dim=3, model="HP")


@pytest.mark.order(order=2)
class TestDepthFirst:
    def test_protein_2d_depth_first_symmetry(self, protein_2d):
        """
        Test if a 2D protein is folded correctly using depth_first_symmetry search.
        """
        depth_first(protein_2d)
        solutions_checked_upper = protein_2d.solutions_checked
        aminos_placed_upper = protein_2d.aminos_placed
        protein_2d.reset()
        depth_first_symmetry(protein_2d)
        assert protein_2d.score == -3
        # Fewer evaluations than pure depth first
        assert protein_2d.solutions_checked < solutions_checked_upper
        assert protein_2d.aminos_placed < aminos_placed_upper

    def test_protein_3d_depth_first_symmetry(self, protein_3d):
        """
        Test if a 3D protein is folded correctly using depth_first_symmetry search.
        """
        depth_first(protein_3d)
        solutions_checked_upper = protein_3d.solutions_checked
        aminos_placed_upper = protein_3d.aminos_placed
        protein_3d.reset()
        depth_first_symmetry(protein_3d)
        assert protein_3d.score == -4
        assert protein_3d.solutions_checked < solutions_checked_upper
        assert protein_3d.aminos_placed < aminos_placed_upper
