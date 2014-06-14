/* print.h
 * Conjunto de funções para impressão de autômatos finitos
 * e gramáticas livres de contexto na saída padrão.
 */
#ifndef PRINT_H
#define PRINT_H

#include <set>
#include "automaton/deterministic.h"
#include "automaton/nonDeterministic.h"
#include "grammar/grammar.h"

void print( const DFA< int, char >& );
void print( const DFA< std::set<int>, char >& );
void print( const NFA< int, char >& );
void print( const Grammar< int, char >& );

#endif // PRINT_H
