/* File:            depth_first_bnb.i
 * Description:     SWIG interface file for a depth-first branch-and-bound
 *                      search function.
 */

%module depth_first_bnb
%{
#include "depth_first_bnb.hpp"
%}

%include "std_vector.i"
%include "std_string.i"

namespace std {
    %template(IntVector) vector<int>;
}

%include "../protein/protein.hpp"
%include "depth_first_bnb.hpp"
