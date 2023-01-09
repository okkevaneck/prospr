#!/usr/bin/env python3
"""
File:           datasets.py
Description:    This file contains functions for loading datasets.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

import pkg_resources
import pandas as pd


def _load_dataset(folder, filename):
    """Returns a specified dataset as a dataframe."""
    stream = pkg_resources.resource_stream(
        "prospr", f"data/{folder}/{filename}"
    )
    return pd.read_csv(stream)


def load_vanEck250(length=10):
    """Returns a vanEck250 dataset as a pandas dataframe."""
    return _load_dataset("vanEck250", f"HP{length}.csv")


def load_vanEck1000(length=10):
    """Returns a vanEck1000 dataset as a pandas dataframe."""
    return _load_dataset("vanEck1000", f"HP{length}.csv")


def load_vanEck_hratio(length=25, hratio=0.1):
    """Returns a vanEck_hratio dataset as a pandas dataframe."""
    return _load_dataset("vanEck_hratio", f"HP{length}_r{hratio}.csv")
