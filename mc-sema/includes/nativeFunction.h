#ifndef nativeFunction_HEADER
#define nativeFunction_HEADER


class NativeFunction {
    private:
        CFG *    graph; //a graph of blocks
        VA       funcEntryVA; //addr of function entry point
        uint64_t nextBlockID; //next available block ID
        
        std::map<VA, uint64_t>             baseToID; //a map of block bases to block IDs
        std::map<uint64_t, NativeBlockPtr> IDtoBlock; //a map of block IDs to blocks

    public:
        NativeFunction (VA b)
            : funcEntryVA (b), nextBlockID (0) {}

        void add_block (NativeBlockPtr);

        VA          get_start() { return this->funcEntryVA; }
        CFG         get_cfg  () { return *this->graph; }
        std::string get_name ();

        uint64_t       num_blocks      () { return this->IDtoBlock.size(); }

        NativeBlockPtr block_from_id   (uint64_t);
        NativeBlockPtr block_from_base (VA);
        uint64_t       entry_block_id  () const;
        void           compute_graph   ();
};

typedef boost::shared_ptr<NativeFunction>   NativeFunctionPtr;

#endif