/* print.h
 * Conjunto de funções para impressão de autômatos finitos,
 * gramáticas livres de contexto e expressões regulares na saída padrão.
 */
#ifndef PRINT_H
#define PRINT_H

#include <set>
#include <utility>
#include "epsilon.h"
#include "automaton/deterministic.h"
#include "automaton/nonDeterministic.h"
#include "automaton/nonDeterministicWithEpsilon.h"
#include "grammar/grammar.h"
#include "regex/tokens.h"
#include "utility/binaryTree.h"
#include "utility/either.h"

void print( const DFA< int, char >& );
void print( const DFA< std::set<int>, char >& );
void print( const DFA< std::pair<int, int>, char >& );
void print( const NFA< int, char >& );
void print( const NFAe< int, char >& );
void print( const Grammar< int, char >& );
void print( const TokenVector< char >& );
void print( const BinaryTree<Either<char, Epsilon, Operator>>& );

/* Converte para string o objeto passado.
 * Não delete o ponteiro retornado. */
const char * etostr( const Either<char, Epsilon, Operator, Parentheses> & );
#endif // PRINT_H
