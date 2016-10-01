#ifndef GCI_H
#define GCI_H

#include "ISOhandler.h"
#include "MemoryConfig.h"
#include "CodeAssembler.h"

void CreateISO(ISOhandler&, MemoryConfig&, CodeAssembler&, bool=false);
void SaveISO(ISOhandler&, MemoryConfig&, std::string);
void LoadISO(ISOhandler&, std::string);
std::string ReadAddr(ISOhandler&, uint32_t);
std::string ReadAddr(ISOhandler&, std::string);

#endif
