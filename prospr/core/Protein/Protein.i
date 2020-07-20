/* File:       Protein.i
 * Author:     Okke van Eck
 *
 * Description:    SWIG file for a Protein object within the HP-model.
 */

%module Protein
%{
#define SWIG_FILE_WITH_INIT
#include "Protein.h"
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

namespace std {
    %template(IntVector) vector<int>;
}

%include "Protein.h"
