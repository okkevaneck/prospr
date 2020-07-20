#!/usr/bin/env bash
# Script for updating the live version of the package.

source venv/bin/activate
python3 setup.py sdist bdist_wheel
twine upload dist/*
deactivate .
