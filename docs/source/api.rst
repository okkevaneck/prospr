.. Overview of all Prospr's functionality on a per module basis.

API Reference
=============
The sections beneath contain all functions that can be used, ordered by module.
The modules that are part of the C++ core are indicated by their section title.
All functions have the same signature in the Python package and the C++ core.
Only the properties of the Protein class are called differently. All API
references are given in alphabetical order.

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
    |             * **protein** - *Protein*: the Protein object to fold.
    |         *Returns:*
    |             * **Protein** - the Protein object set at the found
                    conformation and with updated properties according to the
                    performed moves.

    | **depth_first_bnb**\ (*protein*)
    |     Finds the most optimal conformation using a depth-first
          branch-and-bound algorithm.
    |     *Does not reset the Protein properties beforehand!*
    |         *Parameters:*
    |             * **protein** - *Protein*: the Protein object to fold.
    |         *Returns:*
    |             * **Protein** - the Protein object set at the found
                    conformation and with updated properties according to the
                    performed moves.

    | **beam_search**\ (*protein, beam_width=-1*)
    |     Finds a best-effort conformation using a beam search algorithm.
    |     *Does not reset the Protein properties beforehand!*
    |         *Parameters:*
    |             * **protein** - *Protein*: the Protein object to fold.
    |             * **beam_width** - *int (optional)*: the beam width to use,
            where -1 indicates traversal of the entire search space.
    |         *Returns:*
    |             * **Protein** - the Protein object set at the found
                    conformation and with updated properties according to the
                    performed moves.

AminoAcid - core
----------------
Protein objects use AminoAcid objects internally to keep track of the chain on
the grid. AminoAcid objects have read-only properties, which are consulted
internally by the Protein object. The AminoAcid class can be imported directly
from *prospr* without specifying a submodule, e.g.

.. code-block:: python

    from prospr import AminoAcid

AminoAcid Properties
~~~~~~~~~~~~~~~~~~~~
When using the Python package, each property can be directly called as an
attribute. If the C++ core is used, the property can be accessed using a method.
Each property is described below with the Python and C++ syntax for accessing
them.

    | **type**
    |     Type of the AminoAcid.

             +-----------------+---------------+
             | **Python**      | *.type*       |
             +-----------------+---------------+
             | **C++**         | *.get_type()* |
             +-----------------+---------------+
             | **Return type** | *str*         |
             +-----------------+---------------+

    | **index**
    |     Index of the AminoAcid within the Protein's sequence.

             +-----------------+----------------+
             | **Python**      | *.index*       |
             +-----------------+----------------+
             | **C++**         | *.get_index()* |
             +-----------------+----------------+
             | **Return type** | *int*          |
             +-----------------+----------------+

    | **prev_move**
    |     Move to perform in order to get to the previous AminoAcid in the
            chain.

             +-----------------+--------------------+
             | **Python**      | *.prev_move*       |
             +-----------------+--------------------+
             | **C++**         | *.get_prev_move()* |
             +-----------------+--------------------+
             | **Return type** | *int*              |
             +-----------------+--------------------+

    | **next_move**
    |     Move to perform in order to get to the next AminoAcid in the chain.

             +-----------------+--------------------+
             | **Python**      | *.next_move*       |
             +-----------------+--------------------+
             | **C++**         | *.get_next_move()* |
             +-----------------+--------------------+
             | **Return type** | *int*              |
             +-----------------+--------------------+


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
    |             * **length** - *int (optional)*: the length of the protein
                    sequences to load.
    |         *Returns:*
    |             * **DataFrame** - a Pandas DataFrame containing the protein
                    sequences.

    | **load_vanEck1000**\ (*length=10*)
    |     Loads the vanEck1000 dataset containing 1000 proteins per length,
          with lengths from 10 till 100.
    |         *Parameters:*
    |             * **length** - *int (optional)*: the length of the protein
                    sequences to load.
    |         *Returns:*
    |             * **DataFrame** - a Pandas DataFrame containing the protein
                    sequences.

    | **load_vanEck_hratio**\ (*length=25, hratio=0.1*)
    |     Loads the vanEck_hratio dataset containing proteins per length and
          hratio combination.
    |         *Parameters:*
    |             * **length** - *int (optional)*: the length of the protein
                    sequences to load.
    |             * **hratio** - *float (optional)*: the hratio upperbound of
                    the hratio interval to use.
    |         *Returns:*
    |             * **DataFrame** - a Pandas DataFrame containing the protein
                    sequences.

Helpers
-------
The helpers submodule contains functions used internally. These functions are
very specific for their internal use, but maybe of use to you as well. Each
can be imported from the *prospr.helpers* submodule, e.g.

.. code-block:: python

    from prospr.helpers import get_ordered_positions
..

    | **get_ordered_positions**\ (*protein*)
    |     Returns a Numpy ndarray with the positions of all placed amino acids
          in order of placement.
    |         *Parameters:*
    |             * **protein** - *Protein*: the Protein object to fetch the
                    positions from.
    |         *Returns:*
    |             * **ndarray** - a Numpy ndarray with the positions and types
                    of all placed amino acids and in order of placement.
    |             E.g. `[[[0, 0], "H"], [[0, 1], "P"], [[[1, 1], "P"],
                  [[[1, 0], "H"]]`

    | **get_scoring_aminos**\ (*protein*)
    |     Returns a dictionary with the positions of all placed amino acids
          from the given protein that may score points.
    |         *Parameters:*
    |             * **protein** - *Protein*: the Protein object to compute the
                    scoring amino acids from.
    |         *Returns:*
    |             * **dict** - a dictionary mapping the locations of all amino
                    acids that may score points to their previous move and next
                    move.
    |             E.g. `{[1, 0]: [-1, 2]}`

    | **get_scoring_pairs**\ (*protein*)
    |     Returns a Numpy ndarray with arrays containing tuples of the positions
          of amino acid pairs from the given protein that score points.
    |         *Parameters:*
    |             * **protein** - *Protein*: the Protein object to compute the
                    scoring pairs from.
    |         *Returns:*
    |             * **ndarray** - a Numpy ndarray with arrays containing tuples
                    of the positions of amino acid pairs from the given protein
                    that score points.
    |             E.g. `[[(0, 0), (0, 1)], [(0, 0), (-1, 0)]]`

    | **export_protein**\ (*protein, path*)
    |     Save conformation of a protein in Protein Data Bank (PDB) file format
          for processing or visualization with external software such as `Mol* <https://molstar.org/>`. 
    |         *Parameters:*
    |             * **protein** - *Protein*: Protein object to save the hash of.
    |             * **path** - *os.PathLike or str*: The path of the output file.
    |         *Returns:*
    |             * **None**

Protein - core
--------------
The Protein class is the cornerstone of Prospr. An instance tracks many
properties and has methods to alter the Protein's state. First, an overview
will be made of all the properties and how to reach them. Second, all the
non-property bounded methods will be given. The Protein class can be imported
directly from *prospr* without specifying a submodule, e.g.

.. code-block:: python

    from prospr import Protein
..

Protein Properties
~~~~~~~~~~~~~~~~~~
When using the Python package, each property can be directly called as an
attribute. If the C++ core is used, the property can be accessed using a method.
Each property is described below with the Python and C++ syntax for accessing
them.

    | **bond_values**
    |     The ways to form bonds and their stability.

             +-----------------+----------------------+
             | **Python**      | *.bond_values*       |
             +-----------------+----------------------+
             | **C++**         | *.get_bond_values()* |
             +-----------------+----------------------+
             | **Return type** | *Dict[str, int]*     |
             +-----------------+----------------------+

    | **changes**
    |     The number of amino acids placed so far.

             +-----------------+------------------+
             | **Python**      | *.changes*       |
             +-----------------+------------------+
             | **C++**         | *.get_changes()* |
             +-----------------+------------------+
             | **Return type** | *int*            |
             +-----------------+------------------+

    | **cur_len**
    |     The length of the current conformation.

             +-----------------+------------------+
             | **Python**      | *.cur_len*       |
             +-----------------+------------------+
             | **C++**         | *.get_cur_len()* |
             +-----------------+------------------+
             | **Return type** | *int*            |
             +-----------------+------------------+

    | **dim**
    |     The maximum dimension in which the Protein can fold.

             +-----------------+--------------+
             | **Python**      | *.dim*       |
             +-----------------+--------------+
             | **C++**         | *.get_dim()* |
             +-----------------+--------------+
             | **Return type** | *int*        |
             +-----------------+--------------+

    | **h_idxs**
    |     The indexes of the "H" amino acids in the sequence.

             +-----------------+-----------------+
             | **Python**      | *.h_idxs*       |
             +-----------------+-----------------+
             | **C++**         | *.get_h_idxs()* |
             +-----------------+-----------------+
             | **Return type** | *List[int]*     |
             +-----------------+-----------------+

    | **last_move**
    |     The last performed move.

             +-----------------+--------------------+
             | **Python**      | *.last_move*       |
             +-----------------+--------------------+
             | **C++**         | *.get_last_move()* |
             +-----------------+--------------------+
             | **Return type** | *List[int]*        |
             +-----------------+--------------------+

    | **last_pos**
    |     The position of the amimo acid at the end of the current conformation.

             +-----------------+-------------------+
             | **Python**      | *.last_pos*       |
             +-----------------+-------------------+
             | **C++**         | *.get_last_pos()* |
             +-----------------+-------------------+
             | **Return type** | *List[int]*       |
             +-----------------+-------------------+

    | **max_weigths**
    |     For each amino acid, the maximum value a bond can make.

             +-----------------+----------------------+
             | **Python**      | *.max_weigths*       |
             +-----------------+----------------------+
             | **C++**         | *.get_max_weigths()* |
             +-----------------+----------------------+
             | **Return type** | *List[int]*          |
             +-----------------+----------------------+

    | **score**
    |     The score of the current conformation.

             +-----------------+----------------+
             | **Python**      | *.score*       |
             +-----------------+----------------+
             | **C++**         | *.get_score()* |
             +-----------------+----------------+
             | **Return type** | *int*          |
             +-----------------+----------------+

    | **sequence**
    |     The amino acid sequence of the Protein.

         +-----------------+-------------------+
         | **Python**      | *.sequence*       |
         +-----------------+-------------------+
         | **C++**         | *.get_sequence()* |
         +-----------------+-------------------+
         | **Return type** | *str*             |
         +-----------------+-------------------+

Methods
~~~~~~~
The Protein class knows many methods to interact with a protein. The Python
package and C++ core use the same signatures, so no distinction is made in the
references below.

    | **.get_amino**\ (*position*)
    |     Returns a list with the amino acid index and next move of the amino
          acid placed at the given position.
    |         *Parameters:*
    |             * **position** - *List[int]*: position of the amino acid.
    |         *Returns:*
    |             * **List[int]** - a list with the amino acid's index and next
                    move.
    |             E.g. `[0, 1]`

    | **.get_bonds**\ ()
    |     Returns a list of amino acid index pairs that are bonding.
    |         *Parameters:*
    |             * **None**
    |         *Returns:*
    |             * **List[Tuple[int,int]]** - a list of tuples with two amino
                    acid indexes that bond.
    |             E.g. `[(0, 9), (2, 9), (9, 2), (9, 0)]`

    | **.hash_fold**\ ()
    |     Returns a list of moves representing the current conformation.
    |         *Parameters:*
    |             * **None**
    |         *Returns:*
    |             * **List[int]** - a list of moves.
    |             E.g. `[1, 2, -1]`

    | **.is_hydro**\ (*index*)
    |     Returns if the amino acid at the given index is an H.
    |         *Parameters:*
    |             * **index** - *int*: index of the amino acid.
    |         *Returns:*
    |             * **bool** - A boolean indicating if the amino acid is an H.

    | **.is_valid**\ (*move*)
    |     Returns if the given move is a valid next move.
    |         *Parameters:*
    |             * **move** - *int*: possible next move to perform.
    |         *Returns:*
    |             * **bool** - A boolean indicating if the given move is valid.

    | **.place_amino**\ (*move, track=True*)
    |     Places the next amino acid in the given direction.
    |         *Parameters:*
    |             * **move** - *int*: direction to place the next amino acid.
    |             * **track** - *bool (optional)*: set to True if the move
                    should be tracked as a change.
    |         *Returns:*
    |             * **None**

    | **.remove_amino**\ ()
    |     Removes the previously placed amino acid.
    |         *Parameters:*
    |             * **None**
    |         *Returns:*
    |             * **None**

    | **.reset**\ ()
    |     Reset the whole Protein as if it was just created.
    |         *Parameters:*
    |             * **None**
    |         *Returns:*
    |             * **None**

    | **.reset_conformation**\ ()
    |     Reset the placement of amino acids for the given Protein. Also sets
          the *.score* property to 0.
    |         *Parameters:*
    |             * **None**
    |         *Returns:*
    |             * **None**

    | **.set_hash**\ (*fold_hash*)
    |     Set the conformation to the given sequence of moves.
    |         *Parameters:*
    |             * **fold_hash** - *List[int]*: a list of moves as provided by
                    **.hash_fold()**.
    |         *Returns:*
    |             * **None**

Visualize
---------
Functions from the visualize module of Prospr can be used to illustrate your
research. Each function can be imported from the *prospr.visualize* submodule,
e.g.

.. code-block:: python

    from prospr.visualize import plot_protein
..

    | **plot_protein**\ (*protein, style="basic", ax=None, legend=True,
                            legend_style="inner", show=True,*
    |                    *linewidth=2.5, markersize=210, annotate_first=False*)
    |     Plots the current set conformation of the given Protein object.
    |         *Parameters:*
    |             * **protein** - *Protein*: a Protein object to plot the
                    conformation of.
    |             * **style** - *str (optional)*: The figure style to use,
                    either 'basic' or 'paper'.
    |             * **ax** - *Axes (optional)*: If given, plot the conformation
                    on the given Matplotlib Axes.
    |             * **legend** - *bool (optional)*: Set to False to disable the
                    legend.
    |             * **legend_style** - *str (optional)*: The legend style to
                    use, either 'inner' or 'outer'.
    |             * **show** - *bool (optional)*: Set to False to disable
                    plt.show() call.
    |             * **linewidth** - *float (optional)*: Line width of the chain.
    |             * **markersize** - *float (optional)*: Size of the amino
                    acids.
    |             * **annotate_first** - *float (optional)*: Set to True to
                    highlight first amino acid with a color.
    |         *Returns:*
    |             * **None**
