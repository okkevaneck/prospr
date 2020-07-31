/* File:       depth_first.i
 * Author:     Okke van Eck
 *
 * Description:    SWIG interface file for a depth_first search function.
 */

%module depth_first
%{
#include "depth_first.h"
%}

%include "std_vector.i"
%include "std_string.i"

namespace std {
    %template(IntVector) vector<int>;
}

%include "../../Protein/Protein.h"
%include "depth_first.h"
