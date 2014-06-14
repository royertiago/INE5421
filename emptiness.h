/* emptiness.h
 * Função que tesa vacuidade de linguagens livres de contexto,
 * baseadas em suas representações.
 */
#ifndef EMPTINESS_H
#define EMPTINESS_H

#include <map>
#include "grammar/grammar.h"

template< typename Representation >
bool isEmpty( Representation );

template< typename NonTerminal, typename Terminal >
bool isEmpty( const Grammar<NonTerminal, Terminal> & g ) {
    /* Marcaremos como bons (true) todos os símbolos que podem
     * derivar palavras, e símbolos maus (false) os símbolos
     * que jamais derivam uma forma sentencial sem símbolos não-terminais.
     * Inicialmente, assumiremos que apenas os símbolos terminais
     * são bons. */
    std::map< Either<NonTerminal, Terminal>, bool> mark;
    for( const auto& t : g.nonTerminals )
        mark[t] = false;
    for( const auto& t : g.terminals )
        mark[t] = true;

    /* Será true enquanto houverem alterações no conjunto de símbolos
     * bons; começa em true pois marcamos os terminais como bons. */
    bool markedLastIteration = true;

    auto isBad = [&]( const Either<NonTerminal, Terminal> & t ) {
        return !mark[t];
    };
    auto markGood = [&]( const Either<NonTerminal, Terminal> & t ) {
        if( isBad(t) ) {
            markedLastIteration = true;
            mark[t] = true;
        }
    };

    /* Uma produção boa tem todos os seus símbolos à direita bons. */
    auto goodProduction = [&]( const Production<NonTerminal, Terminal>& p ) {
        for( const auto & t : p.right )
            if( isBad( t ) )
                return false;
        return true;
    };

    while( markedLastIteration ) {
        markedLastIteration = false;
        for( auto& p : g.productions )
            if( goodProduction( p ) )
                markGood( p.left );
    }

    return isBad( g.startSymbol );
}

#endif // EMPTINESS_H
