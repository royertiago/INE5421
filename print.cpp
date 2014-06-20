/* print.cpp
 * Implementação de print.h
 */

#include <cstdio>
#include "print.h"

using std::printf;
using std::set;

void print( const DFA< int, char >& dfa ) {
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

void print( const DFA< set<int>, char >& dfa ) {
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

void print( const NFA< int, char >& nfa ) {
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

void printRightSide( const Production< int, char >& p ) {
    bool firstElement = true;
    for( auto& either : p.right ) {
        if( firstElement )
            firstElement = false;
        else
            printf( ", " );

        if( either.is<int>() )
            printf( "%d", either.getAs<int>() );
        else
            printf( "'%c'", either.getAs<char>() );
    }
}

void print( const Grammar< int, char >& g ) {
    printf( "Start symbol: %d\n", g.startSymbol );
    printf( "P = {" );
    bool firstPrint = true;
    int currentSymbol;
    for( const Production< int, char >& p : g.productions ) {
        if( firstPrint ) {
            firstPrint = false;
            currentSymbol = p.left;
            printf( "%d -> ", p.left );
            printRightSide( p );
        }
        else if( currentSymbol == p.left ) {
            printf( " | " );
            printRightSide( p );
        }
        else {
            currentSymbol = p.left;
            printf( "\n     %d -> ", p.left );
            printRightSide( p );
        }
    }
    printf( "}\n" );
}

const char * etostr( const Either<char, Epsilon, Operator, Parentheses>& e ) {
    if( e.is<Epsilon>() )
        return "Epsilon";
    if( e.is<Operator>() )
        switch( e.operator Operator() ) {
            case Operator::SigmaClosure:      return "SigmaClosure";
            case Operator::KleneeClosure:     return "KleneeClosure";
            case Operator::PositiveClosure:   return "PositiveClosure";
            case Operator::Optional:          return "Optional";
            case Operator::Concatenation:     return "Concatenation";
            case Operator::VerticalBar:       return "VerticalBar";
        }
    if( e == Parentheses::Left )
        return "LeftParentheses";
    if( e == Parentheses::Right )
        return "RightParentheses";

    static char v[] = "'x'";
    v[1] = e;
    return v;
}
