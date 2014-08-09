#include "nativeblock.h"

NativeBlock::NativeBlock (VA b, MCInstPrinter *p)
    : baseAddr(b), MyPrinter(p) {}



void NativeBlock::add_inst (InstPtr p) {
    this->instructions.push_back(p);
    return;
}


std::string NativeBlock::print_block (void) {
    std::string                  s;
    std::list<InstPtr>::iterator it;

    s.append(to_string<uint64_t>(this->get_base(), hex)+"\\n ");

    for (it = this->instructions.begin();
         it != this->instructions.end();
         ++it)
    {
        InstPtr ip = *it;
        string st = ip->printInst();
        s.append(st+"\\n ");
    }

    return s;
}


std::string NativeBlock::get_name (void) {
    return std::string("block_0x"+to_string<VA>(this->baseAddr, hex));
}


uint32_t NativeBlock :: get_size () {
    uint32_t blockLen = 0;

    for (std::list<InstPtr>::iterator i = this->instructions.begin();
         i != this->instructions.end();
         ++i)
    {
        InstPtr inst = *i;
        blockLen += inst->get_len();
    }

    return blockLen;
}