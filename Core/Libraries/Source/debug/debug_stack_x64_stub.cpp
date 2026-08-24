#include "debug.h"
#include "debug_stack.h"
int DebugStackwalk::StackWalk(Signature &sig, struct _CONTEXT *) { sig.m_numAddr = 0; return 0; }
void DebugStackwalk::Signature::GetSymbol(unsigned, char *buf, unsigned bufSize) { if (buf && bufSize) buf[0]=0; }
void DebugStackwalk::Signature::GetSymbol(unsigned, char*, unsigned, unsigned*, char*, unsigned, unsigned*, char*, unsigned, unsigned*, unsigned*) {}
DebugStackwalk::Signature::Signature(const Signature &src) { *this = src; }
DebugStackwalk::Signature& DebugStackwalk::Signature::operator=(const Signature& src) { m_numAddr = src.m_numAddr; for(unsigned i=0;i<m_numAddr;++i) m_addr[i]=src.m_addr[i]; return *this; }
unsigned DebugStackwalk::Signature::GetAddress(int n) const { return m_addr[n]; }
Debug& operator<<(Debug &dbg, const DebugStackwalk::Signature &) { return dbg; }
DebugStackwalk::DebugStackwalk() {}
DebugStackwalk::~DebugStackwalk() {}
void *DebugStackwalk::GetDbghelpHandle() { return nullptr; }
bool DebugStackwalk::IsOldDbghelp() { return false; }
