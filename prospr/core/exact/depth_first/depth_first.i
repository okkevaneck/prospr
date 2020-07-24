/* File:       depth_first.i
 * Author:     Okke van Eck
 *
 * Description:    SWIG file for a depth_first search function.
 */

%module depth_first
%{
#define SWIG_FILE_WITH_INIT
#include "depth_first.h"
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

namespace std {
    %template(IntVector) vector<int>;
}

%include "depth_first.h"
