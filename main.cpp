#include <cstdio>
#include <set>
using std::printf;
#include "closureProperties.h"
#include "compaction.h"
#include "conversion.h"
#include "emptiness.h"
#include "print.h"
#include "automaton/deterministic.h"
#include "automaton/nonDeterministic.h"
#include "automaton/nonDeterministicWithEpsilon.h"
#include "grammar/grammar.h"
#include "regex/parsing.h"
#include "test/lib/testList.h"


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

    printf( "\nCompacting the determinized version, offset == 1:\n" );
    print( compact( dfa, 1 ) );

    printf( "\nNon-determinizing and compacting the determinized version:\n" );
    print( compact( toNFA(dfa) ) );

    printf( "\nAs grammar:\n" );
    print( toGrammar(nfa) );

    NFAe< int, char > nfae = { /*Estados */ {0, 1, 2},
                               /*Alfabeto*/ {'a', 'b', 'c'},
                               /*Delta */   { {{0, 'a'    }, {0}},
                                              {{0, epsilon}, {1}},
                                              {{1, 'b'    }, {1}},
                                              {{1, epsilon}, {2}},
                                              {{2, 'c'    }, {2}},
                                              {{2, epsilon}, {1}}
                                            },
                               /*Finais  */ {2},
                               /*Inicial */ 0
    };
    printf( "\nSample NFA with epsilon:\n" );
    print( nfae );

    printf( "\nWithout epsilon:\n" );
    print( toNFA( nfae ) );

    printf( "\nWith epsilon again:\n" );
    print( toNFAe( toNFA( nfae ) ) );

    printf( "\nDeterminized:\n" );
    print( toDFA( nfae ) );

    printf( "\nDeterminized, compacted and back with epsilon:\n" );
    print( toNFAe( compact(toDFA(nfae)) ) );

    printf( "\nAs grammar:\n" );
    print( toGrammar( nfae ) );

    printf( "\nTo NFAe again:\n" );
    print( toNFAe( toGrammar( nfae ) ) );

    nfae.addTransition( 1, epsilon, 0 );
    nfae.addTransition( 0, 'b', 2 );
    printf( "Now with e-transition from 1 to 0 and b-transition from 0 to 2:\n" );
    print( nfae );

    printf( "\nTwo simple DFA's:\n" );
    DFA< int, char > m3 = { {0, 1, 2},
                            {'0', '1'},
                            { {{0, '0'}, 0},
                              {{0, '1'}, 1},
                              {{1, '0'}, 2},
                              {{1, '1'}, 0},
                              {{2, '0'}, 1},
                              {{2, '1'}, 2}
                            },
                            {0}, 0};
    print( m3 );
    DFA< int, char > m2 = { {0, 1},
                            {'0', '1'},
                            { {{0, '0'}, 0},
                              {{0, '1'}, 1},
                              {{1, '0'}, 0},
                              {{1, '1'}, 1}
                            },
                            {0}, 0};
    print( m2 );
    printf( "\nUnion:\n" );
    print( automataUnion( m2, m3 ) );
    printf( "\nIntersection:\n" );
    print( automataIntersection( m2, m3 ) );
    printf( "\nFirst minus second:\n" );
    print( automataSubtraction( m2, m3 ) );
    printf( "\nSecond minus first:\n" );
    print( automataSubtraction( m3, m2 ) );

    std::string str = "ab*c:d";
    printf( "\nToken vector of %s :\n", str.c_str() );
    print( tokenize(str.begin(), str.end()) );
    print( explicitConcatenations( tokenize(str.begin(), str.end()) ) );
    printf( "\nSyntax tree of %s :\n", str.c_str() );
    print( parse(str) );

    str = "aa|b*:d";
    printf( "\nToken vector of %s :\n", str.c_str() );
    print( tokenize(str.begin(), str.end()) );
    print( explicitConcatenations( tokenize(str.begin(), str.end()) ) );
    printf( "\nSyntax tree of %s :\n", str.c_str() );
    print( parse(str) );

    str = "aa+?*";
    printf( "\nToken vector of %s :\n", str.c_str() );
    print( tokenize(str.begin(), str.end()) );
    print( explicitConcatenations( tokenize(str.begin(), str.end()) ) );
    printf( "\nSyntax tree of %s :\n", str.c_str() );
    print( parse(str) );
    return 0;
}
