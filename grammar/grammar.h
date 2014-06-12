/* grammar.h
 * Estrutura que representa uma gramática livre de contexto.
 *
 * O tipo dos terminais e não-terminais é parametrizado. As exigências são:
 *  - Ambos devem ser tipos POD (Plain Old Data);
 *  - operator< deve estar definido e modelar um ordenamento conexo estrito;
 *    isto é, deve ser transitivo, assimétrico e conexo/total (se definirmos
 *    a == b por !(a<b) && !(b<a), então == é uma relação de equivalência).
 *
 * Os únicos métodos desta estrutura determinam se dado objeto é terminal ou
 * não terminal; algoritmos em outros cabeçalhos operam sobre a gramática.
 */
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <set>
#include "production.h"
#include "utility/either.h"

template< typename Terminal, typename NonTerminal >
struct Grammar {
    std::set< Terminal > terminals;
    std::set< NonTerminal > nonTerminals;
    std::vector< Production<Terminal, NonTerminal> > productions;
    NonTerminal startSymbol;

    bool isTerminal( const Either<Terminal, NonTerminal>& ) const;
    bool isNonTerminal( const Either<Terminal, NonTerminal>& ) const;
};

// Implementação
template< typename Terminal, typename NonTerminal >
bool Grammar< Terminal, NonTerminal >::isTerminal(
        const Either<Terminal, NonTerminal>& a ) const
{
    return a.isFirst() && terminals.count( a ) > 0;
}

template< typename Terminal, typename NonTerminal >
bool Grammar< Terminal, NonTerminal >::isNonTerminal(
        const Either<Terminal, NonTerminal>& a ) const
{
    return a.isSecond() && nonTerminals.count( a ) > 0;
}

#endif // GRAMMAR_H
