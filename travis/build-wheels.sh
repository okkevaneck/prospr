#!/bin/bash
set -e -u -x

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        auditwheel repair "$wheel" --plat "$PLAT" -w /io/wheelhouse/
    fi
}

# Install required system packages.
/opt/python/cp37-cp37m/bin/pip install twine

# Remove old Python versions.
ls -al /opt/python/
rm -rf /opt/python/cp26-*
rm -rf /opt/python/cp33-*
rm -rf /opt/python/cp34-*
rm -rf /opt/python/cp35-*
ls -al /opt/python/

# Compile wheels.
for PYBIN in /opt/python/cp3*/bin; do
    "${PYBIN}/pip" install -r /io/requirements.txt
    "${PYBIN}/pip" wheel /io/ --no-deps -w wheelhouse/
    # TODO: Run setup or not?
#    "${PYBIN}/python" /io/setup.py sdist -d  /io/wheelhouse/
done

# Bundle external shared libraries into the wheels.
for whl in wheelhouse/*.whl; do
    repair_wheel "$whl"
done

# Install packages and test.
for PYBIN in /opt/python/*/bin/; do
    "${PYBIN}/pip" install prospr --no-index -f /io/wheelhouse
    (cd "$HOME"; "${PYBIN}/nosetests" prospr)
done
