#!/usr/bin/env python3
"""
File:           test_depth_first_bnb.py
Description:    This file contains the pytest tests for the depth_first_bnb
                search core code.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

from prospr import Protein, depth_first_bnb, depth_first_bnb_parallel
import pytest


@pytest.fixture()
def protein_2d():
    return Protein("PHPHPHPPH", dim=2, model="HP")


@pytest.fixture()
def protein_3d():
    return Protein("HPPHPHPHPH", dim=3, model="HP")


@pytest.mark.order(order=2)
class TestDepthFirstBnB:
    def test_protein_2d_depth_first_bnb_naive(self, protein_2d):
        """
        Test if a 2D protein is folded correctly using depth_first_bnb search.
        """
        depth_first_bnb(protein_2d)
        assert protein_2d.score == -3
        assert protein_2d.solutions_checked == 4
        assert protein_2d.aminos_placed == 53

    def test_protein_2d_depth_first_bnb_reach(self, protein_2d):
        """
        Test if a 2D solutions is folded correctly using reach_prune criteria.
        """
        depth_first_bnb(protein_2d, "reach_prune")
        assert protein_2d.score == -3
        assert protein_2d.solutions_checked == 4
        assert protein_2d.aminos_placed == 31

    def test_protein_3d_depth_first_bnb(self, protein_3d):
        """
        Test if a 3D protein is folded correctly using depth_first_bnb search.
        """
        depth_first_bnb(protein_3d)
        assert protein_3d.score == -4
        assert protein_3d.solutions_checked == 5
        assert protein_3d.aminos_placed == 49368

    
    def test_protein_2d_depth_first_bnb_parallel(self, protein_2d):
        """
        Test if parallel algorithm solution matches serial algorithm.
        """
        depth_first_bnb_parallel(protein_2d)
        score = protein_2d.score
        protein_2d.reset()
        depth_first_bnb(protein_2d)
        assert score == protein_2d.score
