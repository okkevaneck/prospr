#!/usr/bin/env python3
"""
File:           test_depth_first.py
Description:    This file contains the pytest tests for loading data from the
                datasets.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

from prospr import load_vanEck250, load_vanEck1000, load_vanEck_hratio


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

    def test_vanEck_hratio(self):
        """
        Test loading data from vanEck1000 works.
        """
        l25_r01 = load_vanEck_hratio()
        l25_r01_2 = load_vanEck_hratio(25, 0.1)
        l25_r02 = load_vanEck_hratio(25, 0.2)
        l25_r03 = load_vanEck_hratio(25, 0.3)
        l25_r04 = load_vanEck_hratio(25, 0.4)
        l25_r05 = load_vanEck_hratio(25, 0.5)
        l25_r06 = load_vanEck_hratio(25, 0.6)
        l25_r07 = load_vanEck_hratio(25, 0.7)
        l25_r08 = load_vanEck_hratio(25, 0.8)
        l25_r09 = load_vanEck_hratio(25, 0.9)
        l25_r10 = load_vanEck_hratio(25, 1.0)

        assert not l25_r01.empty
        assert not l25_r01_2.empty
        assert not l25_r02.empty
        assert not l25_r03.empty
        assert not l25_r04.empty
        assert not l25_r05.empty
        assert not l25_r06.empty
        assert not l25_r07.empty
        assert not l25_r08.empty
        assert not l25_r09.empty
        assert not l25_r10.empty
