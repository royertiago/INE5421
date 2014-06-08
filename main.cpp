#include <cstdio>
#include "emptiness.h"
#include "grammar/grammar.h"
#include "test/lib/testList.h"

using std::printf;

int main () {
    if( !Test::run() )
        return 1;

    return 0;
}
