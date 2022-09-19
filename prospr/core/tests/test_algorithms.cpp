/* File:            test_protein.cpp
 * Description:     Source file for testing the functionality of protein.
 *                  Expects that the following modules work:
 *                      - amino_acid.
 */

#include <iostream>
#include <string.h>


/* Test functionality of depth_first. */
int test_depth_first() {

    std::cout << "\tdepth_first ran successfully.\n";
    return 0;
}

/* Test functionality of depth_first_bnb. */
int test_depth_first_bnb() {
    std::cout << "\tdepth_first_bnb ran successfully.\n";
    return 0;
}

/* Test functionality of all algorithms. */
int run_all() {
    int retval;

    /* Test depth_first search. */
    retval = test_depth_first();
    if (retval != 0) {
        return retval;
    }

    /* Test depth_first_bnb search. */
    retval = test_depth_first_bnb();
    if (retval != 0) {
        return retval;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    /* Determine what algorithms to test. */
    if (argc == 1) {
        /* No extra arguments, so testing all algorithms. */
        std::cout << "\tTesting all algorithms..\n";
        run_all();
    } else if (strcmp(argv[1], "depth_first") == 0) {
        std::cout << "\tTesting depth_first..\n";
        return test_depth_first();
    } else if (strcmp(argv[1], "depth_first_bnb") == 0) {
        std::cout << "\tTesting depth_first_bnb..\n";
        return test_depth_first_bnb();
    } else {
        std::cout << "\tAlgorithm not detected, testing all algorithms..\n";
        run_all();
    }

    return 0;
}
