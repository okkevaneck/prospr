#!/usr/bin/env python3
"""
File:           test_beam_search.py
Description:    This file contains the pytest tests for the beam search core
                code.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

from prospr import Protein, beam_search
import pytest


@pytest.fixture()
def protein_2d():
    return Protein("PHPHPHPPH", dim=2, model="HP")


@pytest.fixture()
def protein_3d():
    return Protein("HPPHPHPHPH", dim=3, model="HP")


@pytest.mark.order(order=2)
class TestBeamSearch:
    def test_protein_2d_beam_search_no_bw(self, protein_2d):
        """
        Check if 2D solutions are found correctly.
        """
        beam_search(protein_2d)
        assert protein_2d.score == -3
        assert protein_2d.solutions_checked == 1
        assert protein_2d.aminos_placed == 10

    def test_protein_2d_beam_search_bw_m1(self, protein_2d):
        """
        Check if 2D solutions are found correctly with a beam width of -1.
        """
        beam_search(protein_2d, -1)
        assert protein_2d.score == -3
        assert protein_2d.solutions_checked == 1
        assert protein_2d.aminos_placed == 10

    def test_protein_2d_beam_search_bw99(self, protein_2d):
        """
        Check if 2D solutions are found correctly with a beam width of 99.
        """
        beam_search(protein_2d, 99)
        assert protein_2d.score == -3
        assert protein_2d.solutions_checked == 1
        assert protein_2d.aminos_placed == 10

    def test_protein_2d_beam_search_bw40(self, protein_2d):
        """
        Check if 2D solutions are found correctly with a beam width of 40.
        """
        beam_search(protein_2d, 40)
        assert protein_2d.score == -2
        assert protein_2d.solutions_checked == 1
        assert protein_2d.aminos_placed == 10

    def test_protein_3d_beam_search_bw99(self, protein_3d):
        """
        Check if 3D solutions are found correctly with a beam width of 99.
        """
        beam_search(protein_3d, 99)
        assert protein_3d.score == -4
        assert protein_3d.solutions_checked == 1
        assert protein_3d.aminos_placed == 11

    def test_protein_3d_beam_search_bw10(self, protein_3d):
        """
        Check if 3D solutions are found correctly with a beam width of 10.
        """
        beam_search(protein_3d, 10)
        assert protein_3d.score == -4
        assert protein_3d.solutions_checked == 1
        assert protein_3d.aminos_placed == 11

    def test_protein_3d_beam_search_bw5(self, protein_3d):
        """
        Check if 3D solutions are found correctly with a beam width of 5.
        """
        beam_search(protein_3d, 5)
        assert protein_3d.score == -3
        assert protein_3d.solutions_checked == 1
        assert protein_3d.aminos_placed == 11
