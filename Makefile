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

MODULES = Protein
COREDIR = prospr/core
CXX_FILES = $(shell find $(COREDIR)/ -type f -name "*.cxx")
SO_FILES = $(shell find $(COREDIR)/ -type f -name "*.so")
PY_FILES = $(shell find $(COREDIR)/ -type f -name "*.py" ! -name "__init__.py" \
			! -name "build.py")
PYCACHES = $(shell find prospr/ -type d -name "__pycache__")

.PHONY: build clean deploy
all: build


build: %.cxx
	@echo "\n~ Creating all .so and .py interfaces.."
	python $(COREDIR)/build.py build_ext --build-lib=$(COREDIR)
	@echo "\n~ Moving .so files into right directories.."

	for MODULE in $(MODULES); do \
		FPATH=`find $(COREDIR)/ -type f -name "_$$MODULE*"`; \
		FNAME=`basename "$$FPATH"`; \
		mv $$FPATH "$(COREDIR)/$$MODULE/$$FNAME"; \
	done

	@echo "~ Done building!"

%.cxx:
	@echo "~ Creating all .cxx wrappers.."
	$(foreach MODULE, $(MODULES), \
		swig -python -c++ $(COREDIR)/$(MODULE)/$(MODULE).i)

clean:
	@echo "~ Removing all built .cxx, .so and .py files.."
	@rm -f $(CXX_FILES) $(SO_FILES) $(PY_FILES)
	@echo "~ Removing all __pycache__ directories.."
	@rm -rf $(PYCACHES)
	@echo "~ Done cleaning!"

deploy:
	@echo "~ Creating package.."
	python3 setup.py sdist bdist_wheel
	@echo "\n~ Uploading package.."
	twine upload dist/*
	@echo "\n~ Done deploying!"

