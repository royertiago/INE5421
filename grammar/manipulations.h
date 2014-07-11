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

/* Remove os não-terminais inalcançáveis da gramática, a partir
 * do símbolo inicial. */
template< typename NonTerminal, typename Terminal >
Grammar<NonTerminal, Terminal> removeUnreachable(
        Grammar<NonTerminal, Terminal> );


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

template< typename NonTerminal, typename Terminal >
Grammar<NonTerminal, Terminal> removeUnreachable(
        Grammar<NonTerminal, Terminal> g )
{
    std::map< NonTerminal, bool> reachable;
    for( auto t : g.nonTerminals )
        reachable[t] = false;

    bool updatedLastIteration;

    /* markReachable( t ) marcará como alcançável o objeto passado,
     * caso seja um não-terminal da gramática. */
    auto markReachable = [&]( Either<NonTerminal, Terminal> t ) {
        if( g.isNonTerminal( t ) && !reachable[t] ) {
            reachable[t] = true;
            updatedLastIteration = true;
        }
    };

    markReachable( g.startSymbol );
    while( updatedLastIteration ) {
        updatedLastIteration = false;
        for( const auto& production : g.productions )
            if( reachable[production.left] )
                for( auto t : production.right )
                    markReachable( t );
    }

    /* Agora removamos os inalcançáveis. */
    for( auto it = g.nonTerminals.begin(); it != g.nonTerminals.end(); ) {
        if( !reachable[*it] )
            g.erase( *it++ );
        else
            ++it;
    }

    return g;
}


#endif // MANIPULATIONS_H
