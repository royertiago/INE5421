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

template< typename NonTerminal, typename Terminal >
struct Grammar {
    std::set< NonTerminal > nonTerminals;
    std::set< Terminal > terminals;
    std::set< Production<NonTerminal, Terminal> > productions;
    NonTerminal startSymbol;

    bool isNonTerminal( const Either<NonTerminal, Terminal>& ) const;
    bool isTerminal( const Either<NonTerminal, Terminal>& ) const;
};

// Implementação
template< typename NonTerminal, typename Terminal >
bool Grammar< NonTerminal, Terminal >::isNonTerminal(
        const Either<NonTerminal, Terminal>& a ) const
{
    return a.template is<NonTerminal>() && nonTerminals.count( a ) > 0;
}

template< typename NonTerminal, typename Terminal >
bool Grammar< NonTerminal, Terminal >::isTerminal(
        const Either<NonTerminal, Terminal>& a ) const
{
    return a.template is<Terminal>() && terminals.count( a ) > 0;
}

#endif // GRAMMAR_H
