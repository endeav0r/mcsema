#ifndef dataSection_HEADER
#define dataSection_HEADER

#include "includes/dataSectionEntry.h"

class DataSection 
{
    protected:
        std::list<DataSectionEntry> entries;
        uint64_t base;
        bool     read_only;
        
    public:
        static const uint64_t NO_BASE = (uint64_t)(-1);

        DataSection ()
            : base (NO_BASE), read_only(false) {}

        virtual ~DataSection() {};

        void     setReadOnly(bool isro) { this->read_only = isro; }
        bool     isReadOnly () const    { return this->read_only; }
        uint64_t getBase    () const    { return this->base; }

        void addEntry (const DataSectionEntry &dse) 
        {
            this->entries.push_back(dse);
            if (this->base == NO_BASE || this->base > dse.getBase()) 
                this->base = dse.getBase();
        }

        std::list<DataSectionEntry> & getEntries ()
        {
            return this->entries;
        }

        uint64_t getSize() const
        {
            uint64_t size_sum=0;
            for(std::list<DataSectionEntry>::const_iterator itr = entries.begin();
                    itr != entries.end();
                    itr++) 
            {
               size_sum += itr->getSize(); 
            }

            return size_sum;
        }
        std::vector<uint8_t> getBytes() const
        {
            std::vector<uint8_t> all_bytes;
            for(std::list<DataSectionEntry>::const_iterator itr = entries.begin();
                    itr != entries.end();
                    itr++) 
            {
               std::vector<uint8_t> vec = itr->getBytes();
               all_bytes.insert(all_bytes.end(), vec.begin(), vec.end());
            }

            return all_bytes;
        }
};

#endif