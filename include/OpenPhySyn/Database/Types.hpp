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

#ifndef __PSN_TYPES__
#define __PSN_TYPES__

// Temproary fix for OpenSTA
#define THROW_DCL throw()

#include <OpenSTA/liberty/Liberty.hh>
#include <opendb/db.h>
#include <opendb/dbTypes.h>
#include <opendb/defin.h>
#include <opendb/defout.h>
#include <opendb/lefin.h>
#include <unordered_map>

namespace psn
{
class OpenDBHandler;
class OpenStaHandler;
typedef int DefDbu;
#ifdef USE_OPENDB_DB_HANDLER
typedef odb::dbDatabase     Database;
typedef odb::dbChip         Chip;
typedef odb::dbBlock        Block;
typedef odb::dbInst         Instance;
typedef odb::dbITerm        InstanceTerm; // Instance pin
typedef odb::dbMTerm        LibraryTerm;  // Library pin
typedef odb::dbBTerm        BlockTerm;
typedef odb::dbMaster       LibraryCell;
typedef odb::dbLib          Library;
typedef odb::dbTech         LibraryTechnology;
typedef odb::dbNet          Net;
typedef odb::defin          DefParser;
typedef odb::defout         DefOut;
typedef odb::lefin          LefParser;
typedef sta::LibertyLibrary Liberty;

typedef odb::dbSet<Library>      LibrarySet;
typedef odb::dbSet<Net>          NetSet;
typedef odb::dbSet<BlockTerm>    BlockTermSet;
typedef odb::dbSet<InstanceTerm> InstanceTermSet;
typedef odb::dbIoType::Value     PinDirection;
typedef odb::adsPoint            Point;
typedef OpenDBHandler            DatabaseHandler;
#else
// Default is OpenSTA handler
typedef odb::dbDatabase     Database;
typedef odb::dbChip         Chip;
typedef odb::dbBlock        Block;
typedef sta::Instance       Instance;
typedef sta::Pin            InstanceTerm; // Instance pin
typedef sta::LibertyPort    LibraryTerm;  // Library pin
typedef sta::Pin            BlockTerm;
typedef sta::LibertyCell    LibraryCell;
typedef odb::dbLib          Library;
typedef odb::dbTech         LibraryTechnology;
typedef sta::Net            Net;
typedef odb::defin          DefParser;
typedef odb::defout         DefOut;
typedef odb::lefin          LefParser;
typedef sta::LibertyLibrary Liberty;

typedef odb::dbSet<Library> LibrarySet;
typedef sta::NetSet         NetSet;
typedef sta::PinSet         BlockTermSet;
typedef sta::PinSet         InstanceTermSet;
typedef sta::PortDirection  PinDirection;
typedef odb::adsPoint       Point;
typedef OpenStaHandler      DatabaseHandler;
#endif

class PointHash
{
public:
    size_t
    operator()(const Point& pt) const
    {
        size_t h1 = std::hash<int>()(pt.x());
        size_t h2 = std::hash<int>()(pt.y());
        return h1 ^ h2;
    }
};

class PointEqual
{
public:
    bool
    operator()(const Point& pt1, const Point& pt2) const
    {
        return pt1.x() == pt2.x() && pt1.y() == pt2.y();
    }
};
} // namespace psn
#endif