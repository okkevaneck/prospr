#!/usr/bin/env python3
"""
File:       build.py
Author:     Okke van Eck

Description:    This file contains functions for compiling the C++ files into
                Python modules.
"""

from setuptools import setup, Extension
import os

path = os.path.dirname(__file__)


Protein_module = Extension("_Protein",
                           sources=[f"{path}/Protein/Protein_wrap.cxx",
                                    f"{path}/Protein/Protein.cpp"],
                           )

setup(name="core",
      version="0.1",
      author="okkevaneck",
      description="C++ core compiled into a Python module.",
      ext_modules=[Protein_module],
      py_modules=["Protein"],
      license="LICENSE",
      )
