#include <cstdio>
#include <set>
using std::printf;
#include "acceptanceList.h"
#include "conversion.h"
#include "emptiness.h"
#include "print.h"
#include "automaton/closureProperties.h"
#include "automaton/compaction.h"
#include "automaton/deterministic.h"
#include "automaton/minimization.h"
#include "automaton/nonDeterministic.h"
#include "automaton/nonDeterministicWithEpsilon.h"
#include "grammar/grammar.h"
#include "regex/deSimone.h"
#include "regex/parsing.h"
#include "regex/thompson.h"
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
                             /*Inicial */ 0,
                             /*Finais  */ {2}
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
                               /*Inicial */ 0,
                               /*Finais  */ {2}
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
    printf( "With e-transition from 1 to 0 and b-transition from 0 to 2:\n" );
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
                            0, {0}};
    print( m3 );
    DFA< int, char > m2 = { {0, 1},
                            {'0', '1'},
                            { {{0, '0'}, 0},
                              {{0, '1'}, 1},
                              {{1, '0'}, 0},
                              {{1, '1'}, 1}
                            },
                            0, {0}};
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

    DFA< int, char > test = { {0, 1, 2, 3, 4, 5, 6, 7},
                              {'0', '1'},
                              { { {0, '1'}, 5},
                                { {0, '0'}, 1},
                                { {1, '1'}, 2},
                                { {1, '0'}, 6},
                                { {2, '1'}, 2},
                                { {2, '0'}, 0},
                                { {3, '1'}, 6},
                                { {3, '0'}, 2},
                                { {4, '1'}, 5},
                                { {4, '0'}, 7},
                                { {5, '1'}, 6},
                                { {5, '0'}, 2},
                                { {6, '1'}, 4},
                                { {6, '0'}, 6},
                                { {7, '1'}, 2},
                                { {7, '0'}, 6}
                              },
                              0,
                              { 2 }
    };
    printf( "\nAutomaton from Hopcroft, pg 68, and minimized:\n" );
    print( test );
    printf( "\n" );
    print( minimize( test ) );

    str = "ab";
    printf( "\nMinimum automaton for %s:\n", str.c_str() );
    print( minimize( compact( toDFA( thompson( parse( str ) )))));

    str = "01*|1";
    printf( "\nMinimum automaton for %s:\n", str.c_str() );
    print( minimize( compact( toDFA( thompson( parse( str ) )))));
    
    str = "a:b?+";
    printf( "\nMinimum automaton for %s:\n", str.c_str() );
    print( minimize( compact( toDFA( thompson( parse( str ) )))));

    str = "ab*c";
    printf( "\nMinimum automaton for %s:\n", str.c_str() );
    print( minimize( compact( toDFA( thompson( parse( str ) )))));

    printf( "\nReverse of that automaton:\n" );
    print( 
            minimize(
                compact(
                    toDFA(
                        automataReversion(
                            thompson(
                                parse(
                                    str
                                    )
                                )
                            )
                        )
                    )
                )
         );

    str = "(0|1)+:\\*:\\+";
    printf( "\nMinimum automaton for %s:\n", str.c_str() );
    print( parse( str ) );
    auto arith = minimize( compact( toDFA( thompson( parse( str ) ))));
    print( arith );

    printf( "All 5-char words accepted by this automaton:\n" );
    for( const std::vector< char >& v : acceptanceList( arith, 5 ) ) {
        for( char c : v )
            printf( "%c", c );
        printf( "\n" );
    }

    str = "(01):(ab)";
    auto tree = parse( str );
    print( tree );
    removeSigmaClosure( tree );
    print( tree );

    str = "&(ab)|&*";
    printf( "Tree for %s - and without epsilon:\n\n", str.c_str() );
    tree = parse( str );
    print( tree );
    removeEpsilon( tree );
    print( tree );

    str = "(a&b&)+";
    printf( "Tree for %s - and without epsilon:\n\n", str.c_str() );
    tree = parse( str );
    print( tree );
    removeEpsilon( tree );
    print( tree );

    str = "a|b|&";
    printf( "Tree for %s - and without epsilon:\n\n", str.c_str() );
    tree = parse( str );
    print( tree );
    removeEpsilon( tree );
    print( tree );

    str = "(&:(ab)):&";
    printf( "Tree for %s - and without epsilon:\n\n", str.c_str() );
    tree = parse( str );
    print( tree );
    removeEpsilon( tree );
    print( tree );

    str = "(ab)?(a|b)";
    printf( "Tree for %s - and right-threaded:\n\n", str.c_str() );
    tree = parse( str );
    print( tree );
    addRightThreads( tree.root() );
    print( tree );

    str = "abc";
    tree = parse( str );
    removeSigmaClosure( tree );
    removeEpsilon( tree );
    print( tree );
    addRightThreads( tree.root() );
    auto pair = buildComposition( tree.root() );
    print( pair );

    str = "(a|b)c*";
    tree = parse( str );
    removeSigmaClosure( tree );
    removeEpsilon( tree );
    print( tree );
    addRightThreads( tree.root() );
    pair = buildComposition( tree.root() );
    print( pair );

    str = "(a|bc)+d?";
    tree = parse( str );
    removeSigmaClosure( tree );
    removeEpsilon( tree );
    print( tree );
    addRightThreads( tree.root() );
    pair = buildComposition( tree.root() );
    print( pair );

    str = "ab*c:d";
    tree = parse( str );
    removeSigmaClosure( tree );
    removeEpsilon( tree );
    print( tree );
    addRightThreads( tree.root() );
    pair = buildComposition( tree.root() );
    print( pair );

    str = "ab*c";
    printf( "\nAutomaton for %s (via De Simone Algorithm)\n", str.c_str() );
    print( deSimone( parse(str) ) );

    str = "(a|b)c*";
    printf( "\nAutomaton for %s (via De Simone Algorithm)\n", str.c_str() );
    print( deSimone( parse(str) ) );

    str = "ab*c:d";
    printf( "\nAutomaton for %s (via De Simone Algorithm)\n", str.c_str() );
    print( deSimone( parse( str ) ) );

    str = "ab|ac";
    printf( "\nAutomaton for %s (via De Simone Algorithm)\n", str.c_str() );
    print( deSimone( parse( str ) ) );

    Grammar< char, char > g1 = { {'S', 'A', 'B', 'C', 'D'},
                                 {'a', 'b', 'c', 'd'},
                                 { {'S', {'a', 'S', 'a'} },
                                   {'S', {'d', 'D', 'd'} },
                                   {'A', {'a', 'B'} },
                                   {'A', {'C', 'c'} },
                                   {'A', {'a'} },
                                   {'B', {'d', 'D'} },
                                   {'B', {'b', 'B'} },
                                   {'B', {'b'} },
                                   {'C', {'A', 'a'} },
                                   {'C', {'d', 'D'} },
                                   {'C', {'c'} },
                                   {'D', {'b', 'b', 'B'} },
                                   {'D', {'d'} }
                                 },
                                 'S'
    };
    /* Objetivo:
       GrammarParser p( "->", "|", isSpace, isSpace );
       g1 = parse({ "S -> aSa | dDd",
                    "A->aB|Cc|a",
                    "B->dD|bB|b",
                    "  C -> A a | d D | c  ",
                    "  D -> b b B | d "
                 });
     */
    printf( "Sample grammar:\n" );
    print( g1 );

    printf( "Without unreachable symbols:\n" );
    print( removeUnreachable( g1 ) );


    return 0;
}
