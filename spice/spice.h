#pragma once

#include "../ngspice/sharedspice.h"

#include <tchar.h>
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <windows.h>
#endif

extern bool no_bg;
extern int vecgetnumber;
extern double v2dat;
extern bool has_break;
extern bool errorflag;

int ng_getchar(char* outputreturn, int ident, void* userdata);

int ng_getstat(char* outputreturn, int ident, void* userdata);

int ng_exit(int, bool, bool, int ident, void*);

int ng_thread_runs(bool noruns, int ident, void* userdata);

int ng_initdata(pvecinfoall intdata, int ident, void* userdata);

int ng_data(pvecvaluesall vdata, int numvecs, int ident, void* userdata);

int cieq(char* p, char* s);

int ciprefix(const char* p, const char* s);

bool waitSimulationEnd();

#define spice(X) (ngSpice_Command(const_cast <char *>(X)))
