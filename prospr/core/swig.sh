#!/usr/bin/env bash
swig -python -c++ Protein/Protein.i
python setup.py build_ext --inplace
