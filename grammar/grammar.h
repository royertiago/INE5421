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
#include "algorithm/range.h"
#include "utility/either.h"

template< typename NonTerminal, typename Terminal >
struct Grammar {
    std::set< NonTerminal > nonTerminals;
    std::set< Terminal > terminals;
    std::set< Production<NonTerminal, Terminal> > productions;
    NonTerminal startSymbol;

    bool isNonTerminal( const Either<NonTerminal, Terminal>& ) const;
    bool isTerminal( const Either<NonTerminal, Terminal>& ) const;

    /* Retorna o intervalo que contém todas as produções
     * cujo lado esquerdo é o não-terminal passado. */
    range< typename std::set< 
                Production<NonTerminal, Terminal>
            >::const_iterator >
        productionsFrom( NonTerminal ) const;

    /* Apaga todas as produções cujo lado esquerdo seja o não-terminal
     * passado. Caso este não-terminal não seja o símbolo inicial, ele
     * também é removido do conjunto de não-terminais.
     *
     * Este método não organiza o conjunto de símbolos terminais. */
    void erase( NonTerminal );
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

template< typename NonTerminal, typename Terminal >
range< typename std::set< 
            Production<NonTerminal, Terminal>
        >::const_iterator >
Grammar< NonTerminal, Terminal >::productionsFrom( NonTerminal n ) const
{
    // TODO: ineficiente...
    typedef typename std::set< 
                Production<NonTerminal, Terminal>
            >::const_iterator iterator;

    iterator begin = productions.end();
    iterator end = productions.end();
    iterator i = productions.begin();
    for( ; i != end; ++i )
        if( i->left == n ) {
            begin = i;
            break;
        }
    for( ; i != end; ++i )
        if( i->left != n ) {
            end = i;
            break;
        }

    return range<iterator>( begin, end );
}

template< typename NonTerminal, typename Terminal >
void Grammar< NonTerminal, Terminal >::erase( NonTerminal n ) {
    auto range = productionsFrom( n );
    productions.erase( range.begin(), range.end() );
    if( n != startSymbol )
        nonTerminals.erase( n );
}
#endif // GRAMMAR_H
