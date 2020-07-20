#!/usr/bin/env python3
"""
File:       setup.py
Author:     Okke van Eck

Description:    This file contains functions for compiling the C++ files into
                Python modules.
"""

from distutils.core import setup, Extension


Protein_module = Extension("_Protein",
                           sources=["Protein/Protein_wrap.cxx",
                                    "Protein/Protein.cpp"],
                           )

setup(name="core",
      version="0.1",
      author="okkevaneck",
      description="C++ core compiled into a Python module.",
      ext_modules=[Protein_module],
      py_modules=["Protein"],
      )
