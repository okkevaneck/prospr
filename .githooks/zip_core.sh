#!/usr/bin/env bash
# File:             zip_core.sh
# Description:      This bash file compresses the .cpp core code of prospr
#                       multiple times into archives with different extensions.

set -e

COREDIR=prospr/core
ARCHIVEDIR=core_archives

# Create .zip archive.
zip -jrq prospr_core.zip "${COREDIR}/src/"
mv prospr_core.zip "${ARCHIVEDIR}/prospr_core.zip"
#zip -jrq "${ARCHIVEDIR}/prospr_core.zip" . -i "${COREDIR}/src/"

# Create .tar.gz archive.
#cd "${COREDIR}/src"
#tar -czf "../../../${ARCHIVEDIR}/prospr_core.tar.gz" *.cpp *.hpp

# Add archives to git stage.
#git add "${ARCHIVEDIR}"/prospr_core.zip "${ARCHIVEDIR}"/prospr_core.tar.gz
#git add "${ARCHIVEDIR}"/prospr_core.tar.gz
git add "${ARCHIVEDIR}"/prospr_core.zip
