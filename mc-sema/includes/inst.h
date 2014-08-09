#ifndef inst_HEADER
#define inst_HEADER



/* we're going to make some assumptions about external calls:
 *  - they have some sane calling convention
 *  - they take a defined number of arguments
 *  - they either return no values and have no effects on local
      registers, or, they return a single integer value and 
      assign that value to the EAX register
 */

class Inst {
    private:
        std::vector<VA>             targets;
        std::vector<boost::uint8_t> instBytes;
        VA                 tgtIfTrue;
        VA                 tgtIfFalse;
        VA                 loc;
        llvm::MCInst       NativeInst;
        std::string        instRep;
        ExternalCodeRefPtr extCallTgt;
        ExternalDataRefPtr extDataRef;

        JumpTablePtr      jumpTable;
        bool              jump_table;
        JumpIndexTablePtr jumpIndexTable;
        bool              jump_index_table;

        Prefix          pfx;
        bool            ext_call_target;
        bool            ext_data_ref;
        bool            is_call_external;
        boost::uint32_t data_offset;
        bool            has_data_offset;
        uint8_t         len;
        bool            is_terminator;
        // relocation offset: the number of bytes from the start of the instruction
        // that there is a relocation.
        // Zero if there is no relocation that occurs in the bytes of this
        // instruction
        uint8_t         reloc_offset;

    public:
        enum Prefix {
            NoPrefix,
            RepPrefix,
            RepNePrefix,
            FSPrefix,
            GSPrefix
        };

    bool terminator     () { return this->is_terminator; }
    bool is_data_offset () { return this->has_data_offset; }

    std::string printInst(void) { 
        return this->instRep;
    }

    unsigned int       get_addr_space       ()          { return this->pfx == GSPrefix ? 256 : (this->pfx == FSPrefix ? 257 : 0); }
    std::vector<boost::uint8_t> get_bytes   ()          { return this->instBytes; }
    VA                 get_call_tgt         (int index) { return this->targets.at(index); }
    boost::uint32_t    get_data_offset      ()          { return this->data_offset; }
    ExternalCodeRefPtr get_ext_call_target  ()          { return this->extCallTgt; }
    ExternalDataRefPtr get_ext_data_ref     ()          { return this->extDataRef; }
    llvm::MCInst       get_inst             ()          { return this->NativeInst; }
    bool               get_is_call_external ()          { return this->is_call_external; }
    JumpIndexTablePtr  get_jump_index_table ()          { return this->jumpIndexTable; }
    JumpTablePtr       get_jump_table       ()          { return this->jumpTable; }
    uint8_t            get_len              ()          { return this->len; }
    VA                 get_loc              ()          { return this->loc; }
    Prefix             get_prefix           ()          { return this->pfx; }
    uint8_t            get_reloc_offset     ()          { return this->reloc_offset; }

    VA                 get_tr               ()          { return this->tgtIfTrue; }
    VA                 get_fa               ()          { return this->tgtIfFalse; }

    bool has_call_tgt         () { return !this->targets.empty(); }
    bool has_ext_data_ref     () { return this->ext_data_ref; }
    bool has_ext_call_target  () { return this->ext_call_target; }
    bool has_external_ref     () { return this->has_ext_call_target() || this->has_ext_data_ref(); }
    bool has_jump_index_table () { return this->jump_index_table; }
    bool has_jump_table       () { return this->jump_table; }

    void set_data_offset (boost::uint32_t d) { 
        this->has_data_offset = true;
        this->data_offset = d;
        return;
    }

    void set_ext_call_target (ExternalCodeRefPtr t) { 
        this->extCallTgt = t; 
        this->ext_call_target = true;
        return; 
    }

    void set_ext_data_ref (ExternalDataRefPtr t) { 
        this->extDataRef = t;
        this->ext_data_ref = true;
        return; 
    }

    void set_jump_index_table (JumpIndexTablePtr p) {
        this->jump_index_table = true;
        this->jumpIndexTable = p;
    }

    void set_jump_table (JumpTablePtr p) {
        this->jump_table = true;
        this->jumpTable = p;
    }

    void set_call_tgt         (VA addr)        { this->targets.push_back(addr); }
    void set_inst             (llvm::MCInst i) { this->NativeInst = i; }
    void set_inst_rep         (std::string s)  { this->instRep = s; }
    void set_is_call_external ()               { this->is_call_external = true; }
    void set_terminator       ()               { this->is_terminator = true; }
    void set_reloc_offset     (uint8_t ro)     { this->reloc_offset = ro; }

    void set_tr(VA a) { this->tgtIfTrue = a; }
    void set_fa(VA a) { this->tgtIfFalse = a; }

    // accessors for JumpIndexTable

    Inst (VA virtualAddress, 
          uint8_t length, 
          llvm::MCInst inst, 
          std::string instR, 
          Prefix k,
          std::vector<boost::uint8_t> bytes)
            : instBytes(bytes),       tgtIfTrue(0),           tgtIfFalse(0),
              loc(virtualAddress),    NativeInst(inst),       instRep(instR),
              pfx(k),                 ext_call_target(false), is_call_external(false),
              has_data_offset(false), is_terminator(false),   len(length),
              reloc_offset(0),        jump_table(false),      jump_index_table(false),
              ext_data_ref(false)
        {}
};

#endif