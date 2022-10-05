#!/usr/bin/env python3
"""
File:           test_depth_first.py
Description:    This file contains the pytest tests for the depth_first search
                core code.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2022). See the LICENSE file for the
                specifics.
"""

from prospr import load_vanEck250, load_vanEck1000


class TestDatasets:
    def test_vanEck250(self):
        """
        Test loading data from vanEck250 works.
        """
        l10 = load_vanEck250()
        l10_2 = load_vanEck250(10)
        l20 = load_vanEck250(20)
        l30 = load_vanEck250(30)
        l40 = load_vanEck250(40)
        l50 = load_vanEck250(50)
        l60 = load_vanEck250(60)
        l70 = load_vanEck250(70)
        l80 = load_vanEck250(80)
        l90 = load_vanEck250(90)
        l100 = load_vanEck250(100)

        assert not l10.empty
        assert not l10_2.empty
        assert not l20.empty
        assert not l30.empty
        assert not l40.empty
        assert not l50.empty
        assert not l60.empty
        assert not l70.empty
        assert not l80.empty
        assert not l90.empty
        assert not l100.empty

    def test_vanEck1000(self):
        """
        Test loading data from vanEck1000 works.
        """
        l10 = load_vanEck1000()
        l10_2 = load_vanEck1000(10)
        l20 = load_vanEck1000(20)
        l30 = load_vanEck1000(30)
        l40 = load_vanEck1000(40)
        l50 = load_vanEck1000(50)
        l60 = load_vanEck1000(60)
        l70 = load_vanEck1000(70)
        l80 = load_vanEck1000(80)
        l90 = load_vanEck1000(90)
        l100 = load_vanEck1000(100)

        assert not l10.empty
        assert not l10_2.empty
        assert not l20.empty
        assert not l30.empty
        assert not l40.empty
        assert not l50.empty
        assert not l60.empty
        assert not l70.empty
        assert not l80.empty
        assert not l90.empty
        assert not l100.empty
