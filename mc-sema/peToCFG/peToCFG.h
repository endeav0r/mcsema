/*
Copyright (c) 2013, Trail of Bits
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  Neither the name of the {organization} nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef _PETOCFG_H
#define _PETOCFG_H
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/ADT/Triple.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/MemoryObject.h"
#include "llvm/Object/COFF.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Module.h"
#include "llvm/Function.h"
#include "llvm/LLVMContext.h"
#include "llvm/Type.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/system_error.h"

#include <stack>
#include <set>
#include <map>
#include <list>

#include <stdio.h>
#include <assert.h>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <boost/program_options/config.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_matrix.hpp>

typedef boost::adjacency_matrix<boost::directedS> CFG;
typedef boost::uint64_t FuncID;
typedef boost::uint64_t VA;

class Inst;
typedef boost::shared_ptr<Inst> InstPtr;

#include <BaseBMO.h>

#include "inst_decoder_fe.h"
#include "../common/to_string.h"

#include "../cfgToLLVM/Externals.h"

class JumpTable;
class JumpIndexTable;
typedef boost::shared_ptr<ExternalCodeRef> ExternalCodeRefPtr;
typedef boost::shared_ptr<ExternalDataRef> ExternalDataRefPtr;
typedef boost::shared_ptr<JumpTable>       JumpTablePtr;
typedef boost::shared_ptr<JumpIndexTable>  JumpIndexTablePtr;

class BufferMemoryObject : public llvm::MemoryObject {
    private:
        std::vector<uint8_t> Bytes;
    public:
        BufferMemoryObject (const uint8_t * bytes, uint64_t length) {
            for (unsigned int i = 0; i < length; i++) {
                this->Bytes.push_back(bytes[i]);
            }
        }

        uint64_t getBase   () const { return 0; }
        uint64_t getExtent () const { return this->Bytes.size(); }

        int readByte (uint64_t addr, uint8_t * byte) const {
            if (addr > this->getExtent())
                return -1;
            *byte = this->Bytes[addr];
            return 0;
        }
};


enum ModuleInputFormat {
    COFFObject,
    PEFile,
    ASMText,
    ProtoBuff
};


const llvm::Target *findDisTarget (std::string arch);

NativeModulePtr readModule (std::string, ModuleInputFormat, std::list<VA>);

// used in testSemantics.cpp via funcFromBuff
NativeBlockPtr blockFromBuff(VA, 
                             BufferMemoryObject &, 
                             const llvm::MCDisassembler *, 
                             llvm::MCInstPrinter *);

// used in testSemantics.cpp
NativeFunctionPtr funcFromBuff(VA, 
                               BufferMemoryObject &, 
                               const llvm::MCDisassembler *, 
                               llvm::MCInstPrinter *);

void addExterns (std::list<NativeFunctionPtr>, NativeModulePtr); 

std::string dumpProtoBuf(NativeModulePtr);

#endif
