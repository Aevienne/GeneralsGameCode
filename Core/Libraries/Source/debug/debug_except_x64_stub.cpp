#include "debug.h"
#include "internal_except.h"
#include <windows.h>
void DebugExceptionhandler::LogExceptionLocation(Debug &, struct _EXCEPTION_POINTERS *) {}
void DebugExceptionhandler::LogRegisters(Debug &, struct _EXCEPTION_POINTERS *) {}
void DebugExceptionhandler::LogFPURegisters(Debug &, struct _EXCEPTION_POINTERS *) {}
LONG __stdcall DebugExceptionhandler::ExceptionFilter(struct _EXCEPTION_POINTERS*) { return EXCEPTION_EXECUTE_HANDLER; }
