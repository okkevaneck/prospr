#!/usr/bin/env python3
"""
File:           test_helpers.py
Description:    This file contains the pytest tests for helper functions.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

from pathlib import Path
from tempfile import NamedTemporaryFile

import pytest
from prospr import Protein, export_protein, depth_first


class TestHelpers:
    @classmethod
    def setup_class(cls):
        with NamedTemporaryFile(suffix=".pdb") as tmp:
            cls.tmp_path = Path(tmp.name)

    @classmethod
    def teardown_class(cls):
        cls.tmp_path.unlink(missing_ok=True)

    def test_export_suffix(self):
        """
        Test if exporting a protein enforces file suffix.
        """
        with pytest.raises(ValueError):
            export_protein(Protein("HHPHH"), self.tmp_path.with_suffix(".txt"))

    def test_export_HHPHH(self):
        """
        Test if exporting a protein as a PDB file works.
        """
        p = Protein("HHPHH")
        depth_first(p)
        export_protein(p, self.tmp_path)
        pdb_lines = self.tmp_path.read_text().splitlines()
        n_aminos = len(p.sequence)
        # Expect preamble, atoms, connections, end
        assert len(pdb_lines) == 3 + n_aminos + (n_aminos - 1) + 1
        # Expect preamble (header, title, single comment)
        assert pdb_lines[0].startswith("HEADER    ")
        assert pdb_lines[1].startswith("TITLE     ")
        assert pdb_lines[2].startswith("REMARK    ")

        # Expect valid atoms section (Carbon with ALA/SER residue names for
        # amino acids)
        assert all(
            pdb_lines[3 + i].startswith("ATOM ") for i in range(n_aminos)
        )
        assert all(
            len(pdb_lines[3 + i].split()) == 12 for i in range(n_aminos)
        )
        assert all(
            pdb_lines[3 + i].split()[1:3] == [str(i + 1), "CA"]
            for i in range(n_aminos)
        )
        assert all(
            (
                pdb_lines[3 + i].split()[3] == "ALA"
                if p.sequence[i] == "H"
                else "SER"
            )
            for i in range(n_aminos)
        )
        assert all(
            pdb_lines[3 + i].split()[4:6] == ["A", str(i + 1)]
            for i in range(n_aminos)
        )

        # No negative positions
        assert all(
            float(s) >= 0
            for i in range(n_aminos)
            for s in pdb_lines[3 + i].split()[6:9]
        )

        # Grid size of 3.8 Angstrom
        assert all(
            int(float(s) * 100) % 38 == 0
            for i in range(n_aminos)
            for s in pdb_lines[3 + i].split()[6:9]
        )
        assert all(
            pdb_lines[3 + i].split()[-3:] == ["1.00", "0.00", "C"]
            for i in range(n_aminos)
        )

        # Expect valid connections section
        assert all(
            pdb_lines[3 + n_aminos + i].startswith("CONECT ")
            for i in range(n_aminos - 1)
        )
        assert pdb_lines[3 + n_aminos].split()[1:] == ["1", "2"]

        # Remaining connections follow the pattern: <current> <last> <next>
        assert all(
            pdb_lines[3 + n_aminos + i].split()[1:]
            == [str(j) for j in [i + 1, i, i + 2]]
            for i in range(1, n_aminos - 1)
        )

        # End
        assert pdb_lines[-1] == "END"
