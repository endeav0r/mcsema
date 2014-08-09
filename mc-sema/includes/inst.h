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
    public:
    enum Prefix {
        NoPrefix,
        RepPrefix,
        RepNePrefix,
        FSPrefix,
        GSPrefix
    };

    private:
    std::vector<VA>   targets;
    std::vector<boost::uint8_t> instBytes;
    VA              tgtIfTrue;
    VA              tgtIfFalse;
    VA              loc;
    llvm::MCInst    NativeInst;
    std::string     instRep;
    ExternalCodeRefPtr   extCallTgt;
    ExternalDataRefPtr   extDataRef;

    JumpTablePtr        jumpTable;
    bool                jump_table;
    JumpIndexTablePtr   jumpIndexTable;
    bool                jump_index_table;

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
    std::vector<boost::uint8_t> get_bytes(void) { return this->instBytes; }
    std::string printInst(void) { 
        return this->instRep;
    }

    bool terminator(void) { return this->is_terminator; }
    void set_terminator(void) { this->is_terminator = true; }

    uint8_t get_reloc_offset() {
        return this->reloc_offset;
    }

    void set_reloc_offset(uint8_t ro) {
        this->reloc_offset = ro;
    }

    void set_data_offset(boost::uint32_t d) { 
        this->has_data_offset = true;
        this->data_offset = d;
        return;
    }

    bool is_data_offset(void) { return this->has_data_offset; }

    boost::uint32_t get_data_offset(void) { return this->data_offset; }

    bool get_is_call_external(void) { return this->is_call_external; }
    void set_is_call_external(void) { this->is_call_external = true; }

    llvm::MCInst get_inst(void) { return this->NativeInst; }
    void set_inst(llvm::MCInst i) { this->NativeInst = i; }
    void set_inst_rep(std::string s) { this->instRep = s; }

    VA get_loc(void) { return this->loc; }

    void set_tr(VA a) { this->tgtIfTrue = a; }
    void set_fa(VA a) { this->tgtIfFalse = a; }

    VA get_tr(void) { return this->tgtIfTrue; }
    VA get_fa(void) { return this->tgtIfFalse; }

    uint8_t get_len(void) { return this->len; }

    void set_call_tgt(VA addr) { this->targets.push_back(addr); return; }
    bool has_call_tgt() { return !this->targets.empty(); }
    VA get_call_tgt(int index) { return this->targets.at(index); }

    void set_ext_call_target(ExternalCodeRefPtr t) { 
        this->extCallTgt = t; 
        this->ext_call_target = true;
        return; 
    }

    void set_ext_data_ref(ExternalDataRefPtr t) { 
        this->extDataRef = t;
        this->ext_data_ref = true;
        return; 
    }

    bool has_ext_data_ref(void) {
        return this->ext_data_ref;
    }

    bool has_ext_call_target(void) {
        return this->ext_call_target;
    }

    bool has_external_ref(void) {
        return this->has_ext_call_target() || this->has_ext_data_ref();
    }

    // accessors for JumpTable
    void set_jump_table(JumpTablePtr p) {
        this->jump_table = true;
        this->jumpTable = p;
    }
    JumpTablePtr get_jump_table(void) {
        return this->jumpTable;
    }
    bool has_jump_table(void) {
        return this->jump_table;
    }

    // accessors for JumpIndexTable
    void set_jump_index_table(JumpIndexTablePtr p) {
        this->jump_index_table = true;
        this->jumpIndexTable = p;
    }
    JumpIndexTablePtr get_jump_index_table(void) {
        return this->jumpIndexTable;
    }
    bool has_jump_index_table(void) {
        return this->jump_index_table;
    }
    
    Prefix get_prefix(void) { return this->pfx; }
    unsigned int get_addr_space(void) { 

        switch(this->pfx) {
            case GSPrefix:
                return 256;
            case FSPrefix:
                return 257;
            default:
                return 0;
        }
    }

    ExternalCodeRefPtr get_ext_call_target(void) { return this->extCallTgt; }
    ExternalDataRefPtr get_ext_data_ref(void) { return this->extDataRef; }

    Inst( VA      v, 
          uint8_t l, 
          llvm::MCInst inst, 
          std::string instR, 
          Prefix k,
          std::vector<boost::uint8_t> bytes) : 
        instBytes(bytes),
        tgtIfTrue(0),
        tgtIfFalse(0),
        loc(v), 
        NativeInst(inst),
        instRep(instR),
        pfx(k),
        ext_call_target(false),
        is_call_external(false),
        has_data_offset(false),
        is_terminator(false),
        len(l),
        reloc_offset(0),
        jump_table(false),
        jump_index_table(false),
        ext_data_ref(false)
       { }
};

#endif