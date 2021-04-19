#!/usr/bin/env python3
"""
File:           test_protein.py
Description:    This file contains the pytest tests for the Protein core code.
"""

from prospr import Protein
import pytest


@pytest.fixture
def protein_2d():
    return Protein("HPPHPPH", 2)


@pytest.fixture
def protein_3d():
    return Protein("HPPHPPH", 3)


def test_protein_2d_generation(protein_2d):
    """ Test if a 2D protein is generated correctly. """
    assert protein_2d.sequence == "HPPHPPH"
    assert protein_2d.dim == 2
    assert protein_2d.cur_len == 0
    assert protein_2d.last_move == 0
    assert protein_2d.last_pos == [0, 0]
    assert protein_2d.score == 0
    assert protein_2d.changes == 0


def test_protein_3d_generation(protein_3d):
    """ Test if a 3D protein is generated correctly. """
    assert protein_3d.sequence == "HPPHPPH"
    assert protein_3d.dim == 3
    assert protein_3d.cur_len == 0
    assert protein_3d.last_move == 0
    assert protein_3d.last_pos == [0, 0, 0]
    assert protein_3d.score == 0
    assert protein_3d.changes == 0
