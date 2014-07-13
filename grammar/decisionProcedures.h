/* decisionProcedures.h
 * Funções que informam se dada gramática possui uma linguagem 
 * vazia, finita ou infinita. 
 */
#ifndef GRAMMAR_DECISION_PROCEDURES_H
#define GRAMMAR_DECISION_PROCEDURES_H

#include <functional>
#include <map>
#include "grammar/grammar.h"
#include "grammar/manipulations.h"

/* Determina se a linguagem da gramática é vazia, finita ou infinita. */
template< typename NonTerminal, typename Terminal >
bool empty( Grammar<NonTerminal, Terminal> );
template< typename NonTerminal, typename Terminal >
bool finite( Grammar<NonTerminal, Terminal> );
template< typename NonTerminal, typename Terminal >
bool infinite( Grammar<NonTerminal, Terminal> );

// Implementação
template< typename NonTerminal, typename Terminal >
bool empty( Grammar<NonTerminal, Terminal> g ) {
    return removeDead( g ).productionsFrom( g.startSymbol ).empty();
}

template< typename NonTerminal, typename Terminal >
bool finite( Grammar<NonTerminal, Terminal> g ) {
    return !infinite( g );
}

template< typename NonTerminal, typename Terminal >
bool infinite( Grammar<NonTerminal, Terminal> g ) {
    g = removeUnreachable( removeDead( g ) );
    std::map< NonTerminal, bool > analyzed;
    for( auto n : g.nonTerminals )
        analyzed[n] = false;

    std::function< bool( NonTerminal ) > analyze;
    /* analyzed[n] será verdadeiro caso n esteja sendo analisada na pilha
     * de recursão atual; antes de sair da função, analyzed[n] é alterada
     * para falso novamente.
     *
     * A exceção é quando um ciclo na pilha é detectado; neste caso,
     * todas as funções retornam verdadeiro, ser atualizar analyze[n]. */
    analyze = [&]( NonTerminal n ) {
        if( analyzed[n] ) return true;
        analyzed[n] = true;
        for( auto p : g.productionsFrom( n ) )
            for( auto c : p.right )
                if( g.isNonTerminal( c ) && analyze(c) )
                    return true;
        return analyzed[n] = false;
    };

    return analyze( g.startSymbol );
}
#endif // GRAMMAR_DECISION_PROCEDURES_H
