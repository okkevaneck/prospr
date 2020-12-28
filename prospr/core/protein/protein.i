/* File:            protein.i
 * Description:     SWIG interface file for a protein object within the
 *                      HP-model.
 */

%module protein
%{
#include "protein.h"
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

namespace std {
    %template(IntVector) vector<int>;
}

%include "protein.h"
