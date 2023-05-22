#!/usr/bin/env python3
"""
File:           prospr_interpreter.py
Description:    This file launches a Python interpreter with Prospr functions
                and two proteins loaded.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

from prospr import Protein, depth_first, depth_first_bnb, beam_search
from prospr.visualize import plot_protein
from prospr.datasets import load_vanEck250, load_vanEck1000, load_vanEck_hratio


if __name__ == "__main__":
    protein_2d = Protein("HPHPPPPPPP", dim=2, model="HP")
    protein_3d = Protein("HPPHPHPHPH", dim=3, model="HP")
