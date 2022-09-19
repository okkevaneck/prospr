#!/usr/bin/env bash
# File:             run_all_tests.sh
# Description:      This bash file tests the core functionalities without
#                   building the Python interfaces.

set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
CFLAGS="-O3 -Wall -std=c++11"
DEBUG=""

# Test amino_acid functionality.
test_amino_acid() {
    echo -e "\n~ Testing amino_acid.."
    # shellcheck disable=SC2086
    c++ $CFLAGS -o test_amino_acid test_amino_acid.cpp ../src/amino_acid.cpp

    echo "~ Compilation successful, running the tests.."
    $DEBUG ./test_amino_acid || true
    rm test_amino_acid
    echo -e "~ Done"
}

# Test protein functionality.
test_protein() {
    echo -e "\n~ Testing protein.."
    # shellcheck disable=SC2086
    c++ $CFLAGS -o test_protein test_protein.cpp ../src/protein.cpp ../src/amino_acid.cpp

    echo "~ Compilation successful, running the tests.."
    $DEBUG ./test_protein || true
    rm test_protein
    echo -e "~ Done"
}

# Test all modules.
test_all() {
    test_amino_acid
    test_protein
}

# Main entry point of the script.
main() {
    # Move console into test folder.
    cd "$SCRIPT_DIR" || exit 1

    # Determine if code needs to be debugged using gdb.
    if [[ $# -eq 2 ]] && [ "$2" == "debug" ]; then
        DEBUG="gdb"
    fi

    # Determine what module to test, default to all modules.
    if [[ $# -ne 1 ]]; then
        echo "~ Module:  all"
        test_all
    else
        case "$1" in
            # Only test amino_acid.
            "amino_acid")
                echo "~ Module:  amino_acid"
                test_amino_acid
                ;;
            # Only test protein.
            "protein")
                echo "~ Module:  protein"
                test_protein
                ;;
            # Only test depth_first.
            "depth_first")
                echo "~ Module:  depth_first"
                ;;
            # Only test depth_first_bnb.
            "depth_first_bnb")
                echo "~ Module:  depth_first_bnb"
                ;;
            # Default to test all.
            *)
                echo "~ Module:  all"
                test_all
                ;;
        esac
    fi
}

main "$@"
