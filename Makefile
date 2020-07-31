# File:       Makefile
# Author:     Okke van Eck
#
# Description:		This Makefile supports the commands:
#						- "Deploy"	for updating the live version.
#						- "Build"	for building the Python interfaces for the
#										C++ code.
#						- "Clean"	for removing the Python interfaces for the
#										C++ code as well as the prerequisite
# 										.cxx files. Also removes all compiled
#										Python caches.
# 					Add module names to the MODULES list for automatic
#					compilation.

COREDIR = prospr/core
CXX_FILES = $(shell find $(COREDIR)/ -type f -name "*.cxx")
SO_FILES = $(shell find $(COREDIR)/ -type f -name "*.so")
PY_FILES = $(shell find $(COREDIR)/ -type f -name "*.py" ! -name "__init__.py" \
			! -name "setup.py")
PYCACHES = $(shell find prospr/ -type d -name "__pycache__")

.PHONY: build clean deploy
all: build


build:
	@echo "\n~ Creating all .so and .py interfaces.."
	$(eval $@_SPATHS := $(shell find $(COREDIR)/ -type f -name "*setup.py"))
	$(eval $@_MPATHS := $(foreach PATH, $($@_SPATHS), $(dir $(PATH))))

	# Compile and build all core modules.
	for MPATH in $($@_MPATHS); do \
		MODULE=$$(basename $${MPATH%/}); \
		echo "\n~ SWIG "; \
		swig -python -c++ $$MPATH/$$MODULE.i; \
		echo "\n~ build"; \
		python $$MPATH/setup.py build_ext --build-lib=$$MPATH; \
	done

	@echo "\n~ Done building!"

clean:
	@echo "~ Removing all built .cxx, .so and .py files.."
	rm -f $(CXX_FILES) $(SO_FILES) $(PY_FILES)
	@echo "\n~ Removing all __pycache__ directories.."
	rm -rf $(PYCACHES)
	@echo "\n~ Done cleaning!"

deploy:
	@echo "~ Removing old files.."
	rm -rf build dist prospr.egg-info
	@echo "\n~ Creating new package.."
	python3 setup.py sdist bdist_wheel
	@echo "\n~ Uploading package.."
	twine upload dist/*
	@echo "\n~ Done deploying!"
