#!/usr/bin/env bash
# File:             test_core_locally.sh
# Description:      This file builds a new core and checks basic functionality.

cd ..
./manage.sh build
cd prospr

python3 <<HEREDOC
from prospr_core import Protein, depth_first
p = Protein("PHPHPHPPH")
p2 = depth_first(p)
print(f"PHPHPHPPH score: {p2.score}")
HEREDOC
