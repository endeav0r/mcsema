#ifndef dataSectionEntry_HEADER
#define dataSectionEntry_HEADER

class DataSectionEntry {
    protected:
        uint64_t             base;
        std::vector<uint8_t> bytes;
        bool                 is_symbol;
        std::string          sym_name;

    public:
        DataSectionEntry(uint64_t base, const std::vector<uint8_t> & b)
            : base(base), bytes(b), is_symbol(false) {}

        DataSectionEntry(uint64_t base, const std::string & sname)
            : base(base), sym_name(sname), is_symbol(true) 
        {
            // initialize bytes to null
            this->bytes.push_back(0x0);
            this->bytes.push_back(0x0);
            this->bytes.push_back(0x0);
            this->bytes.push_back(0x0);
        }

        uint64_t             getBase()  const { return this->base; }
        uint64_t             getSize()  const { return this->bytes.size(); }
        std::vector<uint8_t> getBytes() const { return this->bytes; }

        bool getSymbol(std::string &sname) const
        { 
            if(this->is_symbol) {
                sname = this->sym_name;
                return true;
            } else {
                return false;
            }
        }

        virtual ~DataSectionEntry() {};
};

#endif