#!/usr/bin/env python3
"""
File:           context.py
Description:    This file can be used by the tests to use the compiled version
                    of the code. Usage in test file:
                    `from .context import prospr`
"""
import os
import sys
sys.path.insert(0,
                os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import prospr
