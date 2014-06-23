/* deterministic.h
 * Estrutura que representa um autômato finito determinístico.
 *
 * Contém uma função auxiliar, que completa um autômato caso
 * existam transições faltando.
 */
#ifndef DETERMINISTIC_H
#define DETERMINISTIC_H

#include <set>
#include <utility>
#include "math/function.h"
#include "automaton/newState.h"

template< typename State, typename Symbol >
struct DFA {
    std::set< State > states;
    std::set< Symbol > alphabet;
    // Função de transição
    Math::Function< std::pair<State, Symbol>, State > delta;
    std::set< State > finalStates;
    State initialState;

    /* Determina se o autômato aceita ou não a palavra delimitada
     * pelo intervalo [begin, end). */
    template< typename ForwardIterator >
    bool accepts( ForwardIterator begin, ForwardIterator end );
};

/* Completa as transições do autômato, adicionando um novo estado 
 * não-terminal se necessário. */
template< typename State, typename Symbol >
DFA< State, Symbol > completeTransitions( DFA<State, Symbol> dfa ) {
    bool needNewState = false;
    State newState; // Será criado se necessário
    auto getNewState = [&](){
        if( needNewState )
            return newState;
        needNewState = true;
        return newState = generateNewState( dfa );
    };

    for( State q : dfa.states )
        for( Symbol a : dfa.alphabet )
            if( !dfa.delta.onDomain({q, a}) )
                dfa.delta.insert({q, a}, getNewState() );

    if( needNewState ) {
        dfa.states.insert( getNewState() );
        for( Symbol a : dfa.alphabet )
            dfa.delta.insert({getNewState(), a}, getNewState() );
    }
    return dfa;
}
    
#endif // DETERMINISTIC_H
