## Prospr: The Protein Structure Prediction Toolbox
<p align="center">
    <img src="docs/source/_static/prospr_logo.png" alt="Prospr's logo" width="496" height="308">  
</p>

![GitHub](https://img.shields.io/github/license/OkkeVanEck/prospr)
![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/OkkeVanEck/prospr?include_prereleases)
![GitHub Workflow Status (branch)](https://img.shields.io/github/actions/workflow/status/OkkeVanEck/prospr/.github/workflows/build_deploy.yml?branch=master)
[![Documentation Status](https://readthedocs.org/projects/prospr/badge/?version=latest)](https://prospr.readthedocs.io/en/latest/?badge=latest)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)  
**Creator:** Okke van Eck

Prospr is a universal toolbox for protein structure prediction within the
HP-model.
At the core, Prospr offers an easy-to-use Protein data structure, which can be
used to simulate protein folding.
It also offers algorithms, datasets and visualization functions.
The Protein data structure tracks many properties when folding the protein.
This includes tracking the number of conformation changes, which makes it
possible to determine the relative hardness of a protein for a specific 
algorithm.
This allows for a fair comparison between different algorithms.

So far, only square lattices are supported in n-dimensions.
The amino acids can only be placed in the corners of the squares and have to be
one unit distance away from the previously placed amino acid.

The Python package is based on a C++ core, which gives Prospr its high
performance.
The C++ core is made available as a separate zip file to facilitate
high-performance computing applications.
See the C++ core section below for direct links to the core.

## Installation and documentation
This package can simply be installed via pip by running:
```bash
pip install prospr
```
A quickstart and reference documentation can be found at
[prospr.readthedocs.io](https://prospr.readthedocs.io).
The PDF version of the complete documentation can be found
[here](https://prospr.readthedocs.io/_/downloads/en/latest/pdf/).

## Archives
All the C++ core files and datasets are also available as compressed archives.
See the subsections below for direct links.

### C++ core
All the core code which prospr runs on, is available as a compressed archive.
The folder `archives` contains a [.zip](archives/prospr_core.zip) and
a [.tar.gz](archives/prospr_core.tar.gz) archive.

### Datasets
The complete collection of datasets is available as a compressed archive in the
`archives` folder. It is available as a [.zip](archives/prospr_data.zip) and a
[.tar.tz](archives/prospr_data.tar.gz) archive.

## Future work
This toolbox could be used for other protein folding problems within discrete
models.
It would be a great extension to support different models by creating a modular
amino acid.

## License
The used license is the GNU LESSER GENERAL PUBLIC LICENSE.
A copy can be found in the [LICENSE](LICENSE) file on GitHub.
