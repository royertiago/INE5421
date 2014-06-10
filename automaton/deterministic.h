/* deterministic.h
 * Estrutura que representa um autômato finito determinístico.
 */
#ifndef DETERMINISTIC_H
#define DETERMINISTIC_H

#include <set>
#include <utility>
#include "math/function.h"

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
    
#endif // DETERMINISTIC_H
