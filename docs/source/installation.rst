.. How to install Prospr.

Installation
============
One can choose to use the Python package, or only the C++ core. The C++ core is
handy for minimizing the execution speed of your experiments, while the Python
package may lower development time and offers more functionality. Installation
instructions are provided for both scenarios.

Python package
--------------

Python version
~~~~~~~~~~~~~~
Prospr offers support for Python 3.6 and newer. However, using the latest Python
version is always recommended.

Virtual environments
~~~~~~~~~~~~~~~~~~~~
It is recommended to use a new virtual environment for each Python project. An
easy way to setup a new virtual environment is via the **venv** module that
comes with Python 3. After creation, one must activate the virtual environment
before installing any packages. Examples for Linux/MacOS and Windows are given
below.

For Linux/MacOS:

.. code-block:: text

    $ python3 -m venv venv
    $ source venv/bin/activate

For Windows:

.. code-block:: text

    > py -3 -m venv venv
    > venv/Scripts/activate

Installing Prospr
~~~~~~~~~~~~~~~~~
After activating the environment, use pip to install Prospr:

.. code-block:: text

    $ pip install prospr

Congratulations! Prospr is now installed. Check out the :doc:`quickstart`
to see how to use the basics, or read the :doc:`manpages` for all functionality.


C++ core
--------
test
