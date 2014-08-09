#ifndef nativeBlock_HEADER
#define nativeBlock_HEADER

class NativeBlock {
    private:
        //a list of instructions
        VA                      baseAddr;
        std::list<InstPtr>      instructions;
        std::list<VA>           follows;
        llvm::MCInstPrinter     *MyPrinter;
    public:
        NativeBlock (VA, llvm::MCInstPrinter *);

        void add_inst   (InstPtr);
        void add_follow (VA f)    { this->follows.push_back(f); }

        std::string print_block ();

        VA                    get_base    () { return this->baseAddr; }
        std::list<VA> &       get_follows () { return this->follows; }
        std::list<InstPtr>    get_insts   () { return this->instructions; }
        llvm::MCInstPrinter * get_printer () { return this->MyPrinter; }
        std::string           get_name    ();
        uint32_t              get_size    ();
};

typedef boost::shared_ptr<NativeBlock> NativeBlockPtr;

#endif