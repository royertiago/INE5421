/* nonDeterministic.h
 * Estrutura que representa um autômato finito não-determinístico.
 */
#ifndef NON_DETERMINISTIC_H
#define NON_DETERMINISTIC_H

#include <set>
#include <utility>
#include "math/function.h"

template< typename State, typename Symbol >
struct NFA {
    std::set< State > states;
    std::set< Symbol > alphabet;
    // Função de transição
    Math::Function< std::pair<State, Symbol>, std::set<State> > delta;
    State initialState;
    std::set< State > finalStates;
};
#endif // NON_DETERMINISTIC_H
