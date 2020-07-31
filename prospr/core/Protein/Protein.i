/* File:       Protein.i
 * Author:     Okke van Eck
 *
 * Description:    SWIG interface file for a Protein object within the HP-model.
 */

%module Protein
%{
#include "Protein.h"
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

namespace std {
    %template(IntVector) vector<int>;
}

%include "Protein.h"
