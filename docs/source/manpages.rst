.. Overview of all Prospr's functionality on a per module basis.

Manpages
========
The sections beneath contain all functions that can be used, ordered by module.
For each function, first the Python syntax is given, followed by the C++ syntax
if they originate from the C++ core. The modules that are part of the C++ core
are indicated in their section title.

Algorithms - core
-----------------
Prospr contains a couple of built-in search algorithms that will find the most
optimal conformation. These can all be imported directly from *prospr* without
specifying a submodule, e.g.

.. code-block:: python

    from prospr import depth_first
..

    | **depth_first**\ (*protein*)
    |     Finds the most optimal conformation using a depth-first algorithm.
    |     *Does not reset the Protein properties beforehand!*
    |         *Parameters:*
    |             * **protein** - a Protein object to fold.
    |         *Returns:*
    |             * **Protein** - the Protein object set at the found conformation
                    and with updated properties according to the performed
                    moves.

    | **depth_first_bnb**\ (*protein*)
    |     Finds the most optimal conformation using a depth-first
          branch-and-bound algorithm.
    |     *Does not reset the Protein properties beforehand!*
    |         *Parameters:*
    |             * **protein** - a Protein object to fold.
    |         *Returns:*
    |             * **Protein** - the Protein object set at the found conformation
                    and with updated properties according to the performed
                    moves.

Datasets
--------
Prospr provides many datasets. Each can be imported from the *prospr.datasets*
submodule, e.g.

.. code-block:: python

    from prospr.datasets import load_vanEck250
..

    | **load_vanEck250**\ (*length=10*)
    |     Loads the vanEck250 dataset containing 250 proteins per length, with
          lengths from 10 till 100.
    |         *Parameters:*
    |             * **length** - (optional) the length of the protein sequences
                    to load.
    |         *Returns:*
    |             * **DataFrame** - a Pandas DataFrame containing the protein
                    sequences.

    | **load_vanEck1000**\ (*length=10*)
    |     Loads the vanEck250 dataset containing 1000 proteins per length, with
          lengths from 10 till 100.
    |         *Parameters:*
    |             * **length** - (optional) the length of the protein sequences
                    to load.
    |         *Returns:*
    |             * **DataFrame** - a Pandas DataFrame containing the protein
                    sequences.

Helpers
-------
The helpers submodule contains functions used internally. These functions are
very specific for their internal use, but maybe of use to you as well. Each
can be imported from the *prospr.helpers* submodule, e.g.

.. code-block:: python

    from prospr.helpers import get_scoring_aminos
..

    | **get_scoring_aminos**\ (*protein*)
    |     Returns a dictionary with the positions of all placed amino acids
          from the given protein that may score points.
    |         *Parameters:*
    |             * **protein** - a Protein object to compute the scoring amino
                    acids from.
    |         *Returns:*
    |             * **dict** - a dictionary mapping the locations of all amino
                    acids that may score points to their previous move and next
                    move.
    |             E.g. `{[1, 0]: [-1, 2]}`

    | **get_scoring_pairs**\ (*protein*)
    |     Returns a Numpy ndarray with arrays containing tuples of the positions
          of amino acid pairs from the given protein that score points.
    |         *Parameters:*
    |             * **protein** - a Protein object to compute the scoring pairs
                    from.
    |         *Returns:*
    |             * **ndarray** - a Numpy ndarray with arrays containing tuples
                    of the positions of amino acid pairs from the given protein
                    that score points.
    |             E.g. `[[(0, 0), (0, 1)], [(0, 0), (-1, 0)]]`

    | **get_ordered_positions**\ (*protein*)
    |     Returns a Numpy ndarray with the positions of all placed amino acids
          in order of placement.
    |         *Parameters:*
    |             * **protein** - a Protein object to fetch the positions from.
    |         *Returns:*
    |             * **ndarray** - a Numpy ndarray with the positions and types
                    of all placed amino acids and in order of placement.
    |             E.g. `[[[0, 0], "H"], [[0, 1], "P"], [[[1, 1], "P"],
                  [[[1, 0], "H"]]`


Protein - core
--------------

Visualize
---------
Functions from the visualize module of Prospr can be used to illustrate your
research. Each function can be imported from the *prospr.visualize* submodule,
e.g.

.. code-block:: python

    from prospr.visualize import plot_protein
..

    | **plot_protein**\ (*protein*)
    |     Plots the current set conformation of the given Protein object.
    |         *Parameters:*
    |             * **protein** - a Protein object to plot the conformation of.
    |         *Returns:*
    |             * **None**
