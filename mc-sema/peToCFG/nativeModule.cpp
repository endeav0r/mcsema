#include "nativeModule.h"


NativeModule::NativeModule (std::string modName,
                            std::list<NativeFunctionPtr> f,
                            llvm::MCInstPrinter *p)
    : funcs(f), callGraph(f.size()), nextID(0), nameStr(modName), MyPrinter(p)
{}


string NativeModule::printModule() {
    string  s = "";
    // ?
    return s;
}


void NativeModule::addDataSection (VA base, std::vector<uint8_t> &bytes)
{
  DataSection      ds;
  DataSectionEntry dse(base, bytes);

  ds.addEntry(dse);
    
  this->dataSecs.push_back(ds);
}


void NativeModule::addDataSection (const DataSection &d)
{ 
    this->dataSecs.push_back(d);
}