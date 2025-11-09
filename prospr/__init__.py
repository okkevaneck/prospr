from pathlib import Path

from prospr_core import (
    AminoAcid,
    Protein,
    depth_first,
    depth_first_bnb,
    depth_first_bnb_parallel,
    beam_search,
)
from .datasets import load_vanEck250, load_vanEck1000, load_vanEck_hratio
from .helpers import export_protein
from .visualize import plot_protein

# Import __version__ from _version.py during compile time.
exec(open(Path(__file__).parent.absolute() / "_version.py").read())

__all__ = [
    "__version__",
    "AminoAcid",
    "Protein",
    "depth_first",
    "depth_first_bnb",
    "depth_first_bnb_parallel",
    "beam_search",
    "load_vanEck250",
    "load_vanEck1000",
    "load_vanEck_hratio",
    "export_protein",
    "plot_protein",
]
