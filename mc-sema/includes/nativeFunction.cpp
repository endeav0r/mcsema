#include "nativeBlock.h"


void NativeFunction::add_block (NativeBlockPtr b) {
    uint64_t blockBase = b->get_base();
    uint64_t curBlockID = this->nextBlockID;

    this->nextBlockID++;

    //check and make sure that we haven't added this block before
    std::map<VA, uint64_t>::iterator it = this->baseToID.find(blockBase);
    LASSERT(it == this->baseToID.end(), "Added duplicate block!");

    this->baseToID[blockBase] = curBlockID;
    this->IDtoBlock[curBlockID] = b;

    return;
}


std::string NativeFunction::get_name (void) {
    return string("sub_"+to_string<VA>(this->funcEntryVA, hex));
}


NativeBlockPtr NativeFunction::block_from_id (uint64_t id) {
    NativeBlockPtr                          b;
    map<uint64_t, NativeBlockPtr>::iterator it;

    it = this->IDtoBlock.find(id);
    if (it != this->IDtoBlock.end()) {
        b = (*it).second;
    }

    return b;
}


NativeBlockPtr NativeFunction::block_from_base (VA base) {
    NativeBlockPtr b;

    std::map<VA, uint64_t>::iterator it = this->baseToID.find(base);
    LASSERT(it != this->baseToID.end(), "Block not found");

    uint64_t fBID = (*it).second;
    b = this->block_from_id(fBID);

    LASSERT(b, "");

    return b;
}


uint64_t NativeFunction::entry_block_id () const {
    std::map<VA, uint64_t>::const_iterator it = this->baseToID.find(this->funcEntryVA);
    LASSERT(it != this->baseToID.end(), "Block not found");

    uint64_t fBID = (*it).second;
    return fBID;
}


void NativeFunction::compute_graph (void) {
    //build a CFG in boost BGL from the data structures we have
    this->graph = new CFG(this->nextBlockID);

    //iterate over all of the keys in IDtoBlock
    for(std::map<uint64_t, NativeBlockPtr>::iterator it = this->IDtoBlock.begin();
        it != this->IDtoBlock.end();
        ++it)
    {   
        uint64_t       blockId = (*it).first;
        NativeBlockPtr block = (*it).second;
        std::list<VA>  &blockFollows = block->get_follows();


        for(std::list<VA>::iterator fit = blockFollows.begin();
            fit != blockFollows.end();
            ++fit)
        {   
            uint64_t fVA = *fit;
            uint64_t fBID;

            //find the block ID for this VA
            std::map<VA,uint64_t>::iterator mit = this->baseToID.find(fVA);
            LASSERT(mit != this->baseToID.end(), "");
            fBID = (*mit).second;

            //add an edge between the current block ID and the following
            //block ID
            add_edge(blockId, fBID, *(this->graph));
        }
    }
}