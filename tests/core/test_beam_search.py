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
    return Protein("PHPHPHPPH", 2, model="HP")


@pytest.fixture()
def protein_3d():
    return Protein("HPPHPHPHPH", 3, model="HP")


@pytest.mark.order(order=2)
class TestBeamSearch:
    def test_protein_2d_beam_search_no_bw(self, protein_2d):
        """
        Check if 2D solutions are found correctly.
        """
        p = beam_search(protein_2d)
        assert p.score == -3
        assert p.solutions_checked == 1
        assert p.aminos_placed == 9

    def test_protein_2d_beam_search_bw_m1(self, protein_2d):
        """
        Check if 2D solutions are found correctly with a beam width of -1.
        """
        p = beam_search(protein_2d, -1)
        assert p.score == -3
        assert p.solutions_checked == 1
        assert p.aminos_placed == 9

    def test_protein_2d_beam_search_bw99(self, protein_2d):
        """
        Check if 2D solutions are found correctly with a beam width of 99.
        """
        p = beam_search(protein_2d, 99)
        assert p.score == -3
        assert p.solutions_checked == 1
        assert p.aminos_placed == 9

    def test_protein_2d_beam_search_bw40(self, protein_2d):
        """
        Check if 2D solutions are found correctly with a beam width of 40.
        """
        p = beam_search(protein_2d, 40)
        assert p.score == -2
        assert p.solutions_checked == 1
        assert p.aminos_placed == 9

    def test_protein_3d_beam_search_bw99(self, protein_3d):
        """
        Check if 3D solutions are found correctly with a beam width of 99.
        """
        p = beam_search(protein_3d, 99)
        assert p.score == -4
        assert p.solutions_checked == 1
        assert p.aminos_placed == 10

    def test_protein_3d_beam_search_bw10(self, protein_3d):
        """
        Check if 3D solutions are found correctly with a beam width of 10.
        """
        p = beam_search(protein_3d, 10)
        assert p.score == -4
        assert p.solutions_checked == 1
        assert p.aminos_placed == 10

    def test_protein_3d_beam_search_bw5(self, protein_3d):
        """
        Check if 3D solutions are found correctly with a beam width of 5.
        """
        p = beam_search(protein_3d, 5)
        assert p.score == -3
        assert p.solutions_checked == 1
        assert p.aminos_placed == 10
