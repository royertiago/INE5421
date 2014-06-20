/* print.h
 * Conjunto de funções para impressão de autômatos finitos,
 * gramáticas livres de contexto e expressões regulares na saída padrão.
 */
#ifndef PRINT_H
#define PRINT_H

#include <set>
#include "automaton/deterministic.h"
#include "automaton/nonDeterministic.h"
#include "grammar/grammar.h"
#include "utility/either.h"
#include "regex/tokens.h"

void print( const DFA< int, char >& );
void print( const DFA< std::set<int>, char >& );
void print( const NFA< int, char >& );
void print( const Grammar< int, char >& );

/* Converte para string o objeto passado.
 * Não delete o ponteiro retornado. */
const char * etostr( const Either<char, Epsilon, Operator, Parentheses> & );
#endif // PRINT_H
