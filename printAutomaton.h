/* printAutomaton.h
 * Conjunto de funções para impressão de autômatos finitos
 * na saída padrão.
 */
#ifndef PRINT_AUTOMATON_H
#define PRINT_AUTOMATON_H

#include <set>
#include "automaton/deterministic.h"
#include "automaton/nonDeterministic.h"

void printAutomaton( const DFA< int, char >& );
void printAutomaton( const DFA< std::set<int>, char >& );
void printAutomaton( const NFA< int, char >& );

#endif // PRINT_AUTOMATON_H
