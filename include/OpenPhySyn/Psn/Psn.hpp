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

#ifndef __PSN_PSN__
#define __PSN_PSN__
#include <OpenPhySyn/Database/DatabaseHandler.hpp>
#include <OpenPhySyn/Database/Types.hpp>
#include <OpenPhySyn/Psn/DesignSettings.hpp>
#include <OpenPhySyn/Psn/ProgramOptions.hpp>
#include <OpenPhySyn/PsnLogger/LogLevel.hpp>
#include <OpenPhySyn/Sta/DatabaseSta.hpp>
#include <OpenPhySyn/Transform/PsnTransform.hpp>
#include <OpenPhySyn/Transform/TransformInfo.hpp>
#include <OpenSTA/network/ConcreteNetwork.hh>

#include <unordered_map>

namespace psn
{
class Psn
{
public:
    static Psn& instance();
    static Psn* instancePtr();

    virtual ProgramOptions& programOptions();

    int setLogLevel(const char* level);
    int setLogPattern(const char* pattern);
    int setLogLevel(LogLevel level);

    virtual int readDef(const char* path);
    virtual int readLef(const char* path, bool import_library = true,
                        bool import_tech = true);
    virtual int readLib(const char* path);

    virtual int writeDef(const char* path);

    int         loadTransforms();
    bool        hasTransform(std::string transform_name);
    virtual int runTransform(std::string              transform_name,
                             std::vector<std::string> args);

    int  setupInterpreter(Tcl_Interp* interp, bool import_psn_namespace = true,
                          bool print_psn_version = true, bool setup_sta = true);
    int  setupInterpreterReadline();
    void setProgramOptions(int argc, char* argv[]);
    void processStartupProgramOptions();
    int  sourceTclScript(const char* script_path);
    virtual void setWireRC(float res_per_micon, float cap_per_micron);
    virtual int  linkDesign(const char* design_name);

    virtual DatabaseHandler* handler() const;
    virtual DesignSettings*  settings() const;

    virtual void printVersion(bool raw_str = false);
    virtual void printUsage(bool raw_str = false, bool print_transforms = true,
                            bool print_commands = true);
    virtual void printTransforms(bool raw_str = false);
    virtual void printCommands(bool raw_str = false);

    virtual Database*          database() const;
    virtual Liberty*           liberty() const;
    virtual LibraryTechnology* tech() const;

    virtual void clearDatabase();

    virtual int initializeFlute(const char* flue_init_dir = nullptr);

#ifndef OPENROAD_BUILD
    static void initialize(Database* db = nullptr, bool load_transforms = true,
                           Tcl_Interp* interp     = nullptr,
                           bool        init_flute = true);
    static void initialize(sta::DatabaseSta* sta, bool load_transforms = true,
                           Tcl_Interp* interp     = nullptr,
                           bool        init_flute = true);
#else
    static void initialize(sta::DatabaseSta* sta             = nullptr,
                           bool              load_transforms = true,
                           Tcl_Interp*       interp          = nullptr,
                           bool              init_flute      = true);
#endif
    virtual ~Psn();

private:
#ifndef OPENROAD_BUILD
    Psn(Database* db = nullptr);
    Psn(sta::DatabaseSta* sta);
#else
    Psn(sta::DatabaseSta* sta = nullptr);
#endif
    DesignSettings*   settings_;
    Liberty*          liberty_;
    sta::DatabaseSta* sta_;
    Database*         db_;
    DatabaseHandler*  db_handler_;

    int initializeDatabase();
    int initializeSta(Tcl_Interp* interp = nullptr);

    std::unordered_map<std::string, std::shared_ptr<PsnTransform>> transforms_;
    std::unordered_map<std::string, TransformInfo> transforms_info_;
    Tcl_Interp*                                    interp_;
    ProgramOptions                                 program_options_;
    static Psn*                                    psn_instance_;
    static bool                                    is_initialized_;
};
} // namespace psn
#endif