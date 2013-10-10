// $Id: trace.cpp,v 1.1 2013-07-07 19:47:49-07 - - $

#include <climits>
#include <iostream>
#include <vector>

using namespace std;

#include "trace.h"
#include "util.h"

typedef vector<char> boolvec;
boolvec traceflags::flags (UCHAR_MAX + 1, false);
const boolvec trueflags (UCHAR_MAX + 1, true);

void traceflags::setflags (const string &flags) {
   for (string::const_iterator itor = flags.begin();
        itor != flags.end(); ++itor) {
      if (*itor == '@') {
         traceflags::flags = trueflags;
      }else {
         traceflags::flags[*itor] = true;
      }
   }
   // Note that TRACE can trace setflags.
   TRACE ('t', "flags = " << flags);
}

//
// getflag -
//    Check to see if a certain flag is on.
//

bool traceflags::getflag (char flag) {
   // WARNING: Don't TRACE this function or the stack will blow up.
   bool result = flags[flag];
   return result;
}

