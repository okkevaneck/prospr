#!/usr/bin/env bash
# File:             zip_core.sh
# Description:      This bash file compresses the .cpp core code of prospr
#                       multiple times into archives with different extensions.

set -e

COREDIR=prospr/core/src
ARCHIVEDIR=core_archives

## Create .zip archive.
#zip -jrq prospr_core.zip "${COREDIR}/"
#mv prospr_core.zip "${ARCHIVEDIR}/prospr_core.zip"
#
## Create .tar.gz archive.
#cd "${COREDIR}"
#tar -czf prospr_core.tar.gz *.cpp *.hpp
#cd ../../../
#mv "${COREDIR}"/prospr_core.tar.gz "${ARCHIVEDIR}"/prospr_core.tar.gz
#
## Add archives to git stage.
#git add "${ARCHIVEDIR}"/prospr_core.zip "${ARCHIVEDIR}"/prospr_core.tar.gz
