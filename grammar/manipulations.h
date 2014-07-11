/* mainpulations.h
 * Funções que fazem manipulações em gramáticas livres de contexto.
 */
#ifndef MANIPULATIONS_H
#define MANIPULATIONS_H

#include "grammar/grammar.h"

/* Remove os não-terminais mortos; isto é, aqueles incapazes de
 * derivar em um ou mais passos uma sequência contendo apenas
 * símbolos terminais. */
template< typename NonTerminal, typename Terminal >
Grammar<NonTerminal, Terminal> removeDead( Grammar<NonTerminal, Terminal> );


// Implementação
template< typename NonTerminal, typename Terminal >
Grammar<NonTerminal, Terminal> removeDead( Grammar<NonTerminal, Terminal> g )
{
    /* Marcaremos como bons (true) todos os símbolos que podem
     * derivar palavras, e símbolos maus (false) os símbolos que 
     * não podem. Inicialmente, assumiremos que apenas os símbolos
     * terminais são bons. */
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

    /* Agora, temos de eliminar os símbolos mortos e as produções.
     * Este malabarismo é necessário para evitar problemas com
     * invalidação de iteradores: ao apagar um elemento, o iterador
     * que apontava para ele passa a ser inválido - então nenhuma
     * outra operação pode ser feita com ele.
     *
     * Aqui, o pós-incremento evita o uso de uma nova variável.
     * Incrementando o iterador atual antes de usar o valor apontado
     * por ele garante que teremos um iterador válido na próxima iteração.
     */
    for( auto it = g.nonTerminals.begin(); it != g.nonTerminals.end(); ) {
        if( isBad( *it ) )
            g.erase( *it++ );
        else
            ++it;
    }

    return g;
}

#endif // MANIPULATIONS_H
