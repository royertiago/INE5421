#include <cstdio>
#include <set>
#include "conversion.h"
#include "emptiness.h"
#include "printAutomaton.h"
#include "automaton/deterministic.h"
#include "automaton/nonDeterministic.h"
#include "grammar/grammar.h"
#include "test/lib/testList.h"

using std::printf;

int main () {
    if( !Test::run() )
        return 1;

    NFA< int, char > nfa = { /*Estados */ {0, 1, 2},
                             /*Alfabeto*/ {'0', '1'},
                             /*Delta */   { {{0, '0'}, {0, 1}},
                                            {{0, '1'}, {0}   },
                                            {{1, '0'}, {2}   },
                                            {{2, '0'}, {2}   },
                                            {{2, '1'}, {2}   }
                                          },
                             /*Finais  */ {2},
                             /*Inicial */ 0
    };
    printAutomaton( nfa );

    DFA< std::set<int>, char > dfa = toDFA( nfa );
    printAutomaton( dfa );
    return 0;
}
