#!/usr/bin/env bash
# File:             zip_files.sh
# Description:      This bash file compresses the .cpp core code of prospr
#                       multiple times into archives with different extensions.
#                       It also compresses the provided datasets into archives
#                       with different extensions. All archives are moved into
#                       the archives folder.

set -e

ARCHIVEDIR=archives
COREDIR=prospr/core/src
DATADIR=prospr/data


# Create .zip core archive.
zip -jqr prospr_core.zip "${COREDIR}/"
mv prospr_core.zip "${ARCHIVEDIR}/prospr_core.zip"

# Create .tar.gz core archive.
cd "${COREDIR}"
tar -czf prospr_core.tar.gz *.cpp *.hpp
cd ../../../
mv "${COREDIR}"/prospr_core.tar.gz "${ARCHIVEDIR}"/prospr_core.tar.gz

# Create .zip data archive.
cd "${DATADIR}"
zip -rq prospr_data.zip .
cd ../../
mv "${DATADIR}"/prospr_data.zip "${ARCHIVEDIR}"/prospr_data.zip

# Create .tar.gz data archive.
cd "${DATADIR}"
tar -czf prospr_data.tar.gz */*.csv
cd ../../
mv "${DATADIR}"/prospr_data.tar.gz "${ARCHIVEDIR}"/prospr_data.tar.gz

# Add archives to git stage.
git add "${ARCHIVEDIR}"/prospr_core.zip "${ARCHIVEDIR}"/prospr_core.tar.gz \
    "${ARCHIVEDIR}"/prospr_data.zip "${ARCHIVEDIR}"/prospr_data.tar.gz
