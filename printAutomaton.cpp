/* printAutomaton.cpp
 * Implementação de printAutomaton.h
 */

#include <cstdio>
#include "printAutomaton.h"

using std::printf;
using std::set;

void printAutomaton( const DFA< int, char >& dfa ) {
    printf( "       " );
    for( char c : dfa.alphabet )
        printf( "%4c", c );
    printf( "\n" );
    for( int q : dfa.states ) {
        printf( "%s%s%4d", q == dfa.initialState ? "->" : "  ",
                           dfa.finalStates.count(q) == 0 ? " ":"*",
                           q );
        for( char c : dfa.alphabet )
            if( !dfa.delta.onDomain({q, c}) )
                printf( "   -" );
            else
                printf( "%4d", dfa.delta({q, c}) );
        printf( "\n" );
    }
}

static const char * toString( const set< int >& state ) {
    using std::sprintf;
    static char str[1024];

    str[0] = '\0';
    char * ptr = str;
    ptr += sprintf( ptr, "{" );

    bool firstPrint = true;
    for( int q : state )
        if( firstPrint ) {
            ptr += sprintf( ptr, "%d", q );
            firstPrint = false;
        } else
            ptr += sprintf( ptr, ", %d", q );

    sprintf( ptr, "}" );
    return str;
}

void printAutomaton( const DFA< set<int>, char >& dfa ) {
    printf( "             " );
    for( char c : dfa.alphabet )
        printf( "%10c", c );
    printf( "\n" );
    for( set<int> q : dfa.states ) {
        printf( "%s%s%10s", q == dfa.initialState ? "->" : "  ",
                            dfa.finalStates.count(q) == 0 ? " ":"*",
                            toString( q ) );
        for( char c : dfa.alphabet )
            if( !dfa.delta.onDomain({q, c}) )
                printf( "         -" );
            else
                printf( "%10s", toString( dfa.delta({q, c}) ) );
        printf( "\n" );
    }
}

void printAutomaton( const NFA< int, char >& nfa ) {
    printf( "       " );
    for( char c : nfa.alphabet )
        printf( "%10c", c );
    printf( "\n" );
    for( int q : nfa.states ) {
        printf( "%s%s%4d", q == nfa.initialState ? "->" : "  ",
                           nfa.finalStates.count(q) == 0 ? " ":"*",
                           q );
        for( char c : nfa.alphabet )
            if( !nfa.delta.onDomain({q, c}) )
                printf( "         -" );
            else
                printf( "%10s", toString( nfa.delta({q, c}) ) );
        printf( "\n" );
    }
}
