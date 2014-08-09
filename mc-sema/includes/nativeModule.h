#ifndef nativeModule_HEADER
#define nativeModule_HEADER

class NativeModule {

    private:
        std::list<NativeFunctionPtr>        funcs;
        std::map<FuncID, NativeFunctionPtr> IDtoFunc;
        CFG                           callGraph;
        FuncID                        nextID;
        std::string                   nameStr;
        const llvm::Target *          MyTarget;
        const llvm::MCAsmInfo *       MyAsmInfo;
        const llvm::MCSubtargetInfo * MySubtargetInfo;
        llvm::MCInstPrinter *         MyPrinter;

        std::list<DataSection>        dataSecs;
        std::list<ExternalCodeRefPtr> extCalls;
        std::list<ExternalDataRefPtr> extData;


    public:
        class EntrySymbol {
            private:
                std::string name;
                VA          addr;
                bool        has_extra;
                int         argc;
                bool        does_return;

                ExternalCodeRef::CallingConvention cconv;
                
            public:
                EntrySymbol (const std::string &name, 
                             VA                addr)
                    : name(name), addr(addr), has_extra(false) {}

                EntrySymbol (VA addr)
                    : addr(addr), has_extra(false) {
                        this->name = "sub_"+to_string<VA>(this->addr, std::hex);
                    }

                VA                  getAddr    () const { return this->addr; }
                int                 getArgc    () const { return this->argc; }
                const std::string&  getName    () const { return this->name; }
                bool                hasExtra   () const { return this->has_extra; }
                bool                doesReturn () const { return this->does_return; }

                ExternalCodeRef::CallingConvention getConv() const { return this->cconv; }

                void setExtra(int argc, 
                              bool does_ret, 
                              ExternalCodeRef::CallingConvention conv) 
                {
                    this->argc        = argc;
                    this->does_return = does_ret;
                    this->cconv       = conv;
                    this->has_extra   = true;
                }
        };

    std::vector<EntrySymbol> entries;

    NativeModule (std::string, std::list<NativeFunctionPtr>, llvm::MCInstPrinter *);

    void add_func (NativeFunctionPtr f) { this->funcs.push_back(f); }

    std::list<NativeFunctionPtr>    get_funcs      () { return this->funcs; }
    get_cfg                         get_cfg        () { return this->callGraph; }
    llvm::MCInstPrinter *           get_printer    () { return this->MyPrinter; }
    std::list<DataSection> &        getData        () { return this->dataSecs; }
    std::list<ExternalCodeRefPtr>   getExtCalls    () { return this->extCalls; }
    std::list<ExternalDataRefPtr>   getExtDataRefs () {return this->extData; }
    const std::vector<EntrySymbol>& getEntryPoints () const { return this->entries; }

    std::string name(void) { return this->nameStr; }

    std::string printModule(void);

    //add a data section from a COFF object
    void addDataSection (VA, std::vector<uint8_t> &);
    void addDataSection (const DataSection &d);
    void addExtCall     (ExternalCodeRefPtr p) { this->extCalls.push_back(p); }
    void addExtDataRef  (ExternalDataRefPtr p) { this->extData.push_back(p); return; }
    void addEntryPoint  (const EntrySymbol& ep) {
        this->entries.push_back(ep);
    }
};

typedef boost::shared_ptr<NativeModule>      NativeModulePtr;

#endif