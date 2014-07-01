/* decisionProcedures.h
 * Coleção de algoritmos que decidem problemas sobre linguagens
 * regulares, representadas como autômatos finitos determinísticos.
 */
#ifndef DECISION_PROCEDURES_H
#define DECISION_PROCEDURES_H

#include <map>
#include <functional>
#include "automaton/compaction.h"
#include "automaton/deterministic.h"
#include "automaton/minimization.h"
#include "automaton/closureProperties.h"

/* Determina se os dois autômatos finitos são equivalentes, complementares
 * ou disjuntos (a interseção de suas linguages é vazia), ou se o primeiro
 * está incluido no segundo. */ 
template< typename State, typename Symbol >
bool equivalent( DFA< State, Symbol >, DFA< State, Symbol > );
template< typename State, typename Symbol >
bool complementary( DFA< State, Symbol >, DFA< State, Symbol > );
template< typename State, typename Symbol >
bool disjoint( DFA< State, Symbol >, DFA< State, Symbol > );
template< typename State, typename Symbol >
bool included( DFA< State, Symbol >, DFA< State, Symbol > );

/* Determina se a linguagem do autômato é vazia, finita ou infinita. */
template< typename State, typename Symbol >
bool empty( DFA< State, Symbol > );
template< typename State, typename Symbol >
bool finite( DFA< State, Symbol > );
template< typename State, typename Symbol >
bool infinite( DFA< State, Symbol > );

// Implementação
template< typename State, typename Symbol >
bool equivalent( DFA< State, Symbol > dfa1, DFA< State, Symbol > dfa2 ) {
    return included( dfa1, dfa2 ) && included( dfa2, dfa1 );
}

template< typename State, typename Symbol >
bool complementary( DFA< State, Symbol > dfa1, DFA< State, Symbol > dfa2 ) {
    return equivalent( dfa1, complement( dfa2 ) );
}

template< typename State, typename Symbol >
bool disjoint( DFA< State, Symbol > dfa1, DFA< State, Symbol > dfa2 ) {
    return empty( automataIntersection( dfa1, dfa2 ) );
}

template< typename State, typename Symbol >
bool included( DFA< State, Symbol > dfa1, DFA< State, Symbol > dfa2 ) {
    return empty( automataSubtraction( dfa1, dfa2 ) );
}

template< typename State, typename Symbol >
bool empty( DFA< State, Symbol > dfa ) {
    return minimize( compact(dfa) ).finalStates.empty();
}

template< typename State, typename Symbol >
bool finite( DFA< State, Symbol > dfa ) {
    return !infinite( dfa );
}

template< typename State, typename Symbol >
bool infinite( DFA< State, Symbol > dfa ) {
    dfa = minimize( dfa );

    // Vamos procurar por laços no autômato mínimo.
    std::map< State, bool > analyzed;
    std::function< bool( State ) > analyze;
    analyze = [&]( State q ) {
        if( analyzed[q] ) return true;
        analyzed[q] = true;

        for( Symbol a : dfa.alphabet )
            if( dfa.delta.onDomain({q, a}) )
                if( analyze( q ) )
                    return true;
        return false;
    };

    return analyze( dfa.initialState );
}
#endif // DECISION_PROCEDURES_H
