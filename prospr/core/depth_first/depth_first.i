/* File:            depth_first.i
 * Description:     SWIG interface file for a depth-first search function.
 */

%module depth_first
%{
#include "depth_first.hpp"
%}

%include "std_vector.i"
%include "std_string.i"

namespace std {
    %template(IntVector) vector<int>;
}

%include "../protein/protein.hpp"
%include "depth_first.hpp"
