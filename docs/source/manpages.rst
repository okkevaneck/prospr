.. Overview of all Prospr's functionality on a per module basis.

Manpages
========
The sections beneath contain all functions that can be used, ordered by module.
For each function, first the Python syntax is given, followed by the C++ syntax
if they originate from the C++ core. The modules that are part of the C++ core
are indicated in their section title.

Algorithms (core)
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

    | **load_vanEck250**\ (*len=10*)
    |     Loads the vanEck250 dataset containing 250 proteins per length, with
          lengths from 10 till 100.
    |         *Parameters:*
    |             * **len** - (optional) the length of the protein sequences to
                    load.
    |         *Returns:*
    |             * **DataFrame** - a Pandas DataFrame containing the protein
                    sequences.

    | **load_vanEck1000**\ (*len=10*)
    |     Loads the vanEck250 dataset containing 1000 proteins per length, with
          lengths from 10 till 100.
    |         *Parameters:*
    |             * **len** - (optional) the length of the protein sequences to
                    load.
    |         *Returns:*
    |             * **DataFrame** - a Pandas DataFrame containing the protein
                    sequences.

Helpers
-------

Protein (core)
--------------

Visualize
---------
