#include <cstdio>
#include <set>
#include "compaction.h"
#include "conversion.h"
#include "emptiness.h"
#include "print.h"
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
    printf( "Sample automaton:\n" );
    print( nfa );

    printf( "\nDeterminizing:\n" );
    DFA< std::set<int>, char > dfa = toDFA( nfa );
    print( dfa );

    printf( "\nCompacting the determinized version:\n" );
    print( compact( dfa ) );

    printf( "\nNon-determinizing and compacting the determinized version:\n" );
    print( compact( toNFA(dfa) ) );

    printf( "\nAs grammar:\n" );
    print( toGrammar(nfa) );

    return 0;
}
