// BSD 3-Clause License

// Copyright (c) 2019, SCALE Lab, Brown University
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
%module psn
%{
#include "Psn/Exports.hpp"
#include <OpenPhySyn/Psn/Psn.hpp>

#ifndef OPENROAD_BUILD
#include "db.h"
#include "dbShape.h"
#include "geom.h"
using namespace odb;
#endif

#include <memory>
#include <OpenPhySyn/SteinerTree/SteinerTree.hpp>
#include <OpenPhySyn/Database/DatabaseHandler.hpp>
using namespace psn;
DatabaseHandler *handler() {
   return Psn::instance().handler();
}
%}

DatabaseHandler *handler();




%include <typemaps.i>
%include <std_string.i>
%include <std_pair.i>
%include <std_except.i>
#ifndef OPENROAD_BUILD
%include <stl.i>
//%include <src/Tcl/Swig/std_unique_ptr.i>
%include <std_vector.i>
#else
%include <src/Tcl/Swig/std_vector.i>
#endif
%template(vector_str) std::vector<std::string>;
%typemap(in) char ** {
     Tcl_Obj **listobjv;
     int       nitems;
     int       i;
     if (Tcl_ListObjGetElements(interp, $input, &nitems, &listobjv) == TCL_ERROR) {
        return TCL_ERROR;
     }
     $1 = (char **) malloc((nitems+1)*sizeof(char *));
     for (i = 0; i < nitems; i++) {
        $1[i] = Tcl_GetStringFromObj(listobjv[i],0);
     }
     $1[i] = 0;
}

// This gives SWIG some cleanup code that will get called after the function call
%typemap(freearg) char ** {
     if ($1) {
        free($1);
     }
}

%typemap(in) (uint) = (int);
%typemap(in) (unsigned int) = (int);
%typemap(in) (unsigned long) = (int);
%typemap(in) (ulong) = (int);

// wrap_unique_ptr(SteinerTreePtr, SteinerTree);
// wrap_unique_ptr(SteinerTreePtr, psn::SteinerTree);
// %include "include/OpenPhySyn/SteinerTree/SteinerTree.hpp"

#ifdef OPENROAD_BUILD
%ignore     import_def;
%ignore     import_lef;
%ignore     import_lib;
%ignore     import_liberty;
%ignore     export_def;
%ignore     print_liberty_cells;
%ignore psn::Psn::initialize;
#endif

#ifndef OPENROAD_BUILD
%include "external/OpenDB/src/swig/tcl/dbtypes.i"
%include "external/OpenDB/include/opendb/geom.h"
%include "external/OpenDB/include/opendb/db.h"
#endif
%include "include/OpenPhySyn/Database/Types.hpp"
%rename(pt_eq) psn::PointEqual::operator()(const Point& pt1, const Point& pt2);
%rename(pt_hash) psn::PointHash::operator()(const Point& pt);
%include "include/OpenPhySyn/Database/DatabaseHandler.hpp"
#ifndef OPENROAD_BUILD //Temp. removed until SWIG is upgraded in the Dockerfile
%include "include/OpenPhySyn/Psn/Psn.hpp"
#endif

%include "Psn/Exports.hpp"
