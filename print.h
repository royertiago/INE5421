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

typedef BinaryTree<Either<char, Epsilon, Operator>>::iterator EitherIterator;

void print( const DFA< int, char >& );
void print( const DFA< std::set<int>, char >& );
void print( const DFA< std::pair<int, int>, char >& );
void print( const NFA< int, char >& );
void print( const NFAe< int, char >& );
void print( const Grammar< int, char >& );
void print( const TokenVector< char >& );
void print( const BinaryTree<Either<char, Epsilon, Operator>>& );
void print( const std::set< EitherIterator >& );
void print( const std::pair<
                std::set< EitherIterator >,
                std::map< EitherIterator, std::set< EitherIterator > >
            > & );
                    

/* Converte para string o objeto passado.
 * Não delete o ponteiro retornado. */
const char * tostr( const Either<char, Epsilon, Operator, Parentheses> & );
const char * tostr( const Either<char, Epsilon, Operator>& e );
const char * tostr( const std::set<int> & );
#endif // PRINT_H
