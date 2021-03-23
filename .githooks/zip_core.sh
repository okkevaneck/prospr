#!/usr/bin/env bash

# Compress all core .cpp files into archive.
./manage.sh zip_core
git add prospr_core.zip
