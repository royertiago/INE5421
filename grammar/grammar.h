/* grammar.h
 * Estrutura que representa uma gramática livre de contexto.
 *
 * O tipo dos terminais e não-terminais é parametrizado, mas deve
 * ser o mesmo; as exigências são:
 *  - operator< deve estar definido e modelar um ordenamento conexo estrito;
 *    isto é, deve ser transitivo, assimétrico e conexo/total (se definirmos
 *    a == b por !(a<b) && !(b<a), então == é uma relação de equivalência).
 *  - Construtores de cópia e movimento, operadores de atribuição de cópia
 *    e de movimento e destrutores não devem lançar exceções.
 */
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <set>
#include "production.h"

template< typename T >
struct Grammar {
    std::set< T > terminals;
    std::set< T > nonTerminals;
    std::vector< Production<T> > productions;
    T startSymbol;
};

#endif // GRAMMAR_H
