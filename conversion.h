/* conversion.h
 * Algoritmos de conversão entre as representações de linguagens
 * regulares suportados pelo programa:
 *  - Autômato finito determinístico;
 *  - Autômato finito não-determinístico;
 *  - Gramática regular.
 *
 * Conversão de um tipo para ele próprio sempre é incluído, por completude.
 */
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <set>
#include <map>
#include <queue>
#include "automaton/deterministic.h"
#include "automaton/nonDeterministic.h"

/* Converte objetos para autômatos finitos determinísticos equivalentes.
 *
 * O valor de retorno é um DFA< N, T >, em que N é um tipo que varia
 * de acordo com a função de conversão. T, os símbolos de entrada,
 * é compatível com o alfabeto do elemento de entrada:
 *  - Para NFAs e DFAs, T é o símbolo de entrada.
 *
 * O autômato gerado não necessariamente estará minimizado. */
template< typename State, typename Symbol >
    DFA< State, Symbol >           toDFA( DFA< State, Symbol > );
template< typename State, typename Symbol >
    DFA< std::set<State>, Symbol > toDFA( NFA< State, Symbol > );

// Implementação

// DFA para DFA
template< typename State, typename Symbol >
DFA< State, Symbol > toDFA( DFA< State, Symbol > automaton ) {
    return automaton;
}

// NFA para DFA (determinização)
template< typename State, typename Symbol >
DFA< std::set<State>, Symbol > toDFA( NFA< State, Symbol > nfa ) {
    using std::set;

    DFA< set<State>, Symbol > dfa;

    // Incluiremos apenas os estados necessários pelo autômato:
    std::queue< set<State> > statesToBeIncluded;

    /* unite( A, B ) substitui A por A unido com B. */
    auto unite = []( set<State>& A, const set<State>& B ) {
        A.insert( B.begin(), B.end() );
    };
    /* delta( q, a ) retorna o conjunto de estados do NFA que
     * são alcançados ao transitar por 'a' a partir de 'q'.
     * Caso não haja tais transições, o conjunto vazio é retornado. */
    auto delta = [&]( State q, Symbol a ) -> set< State > {
        if( nfa.delta.onDomain({ q, a }) )
            return nfa.delta({ q, a });
        return set< State >();
    };
    /* Marca o estado para ser incluso no autômato determinístico.
     * Caso o estado já tenha sido incluído, nada é feito. */
    auto markToInclusion = [&]( const set<State>& q ) {
        if( dfa.states.count( q ) == 0 )
            statesToBeIncluded.push( q );
    };

    dfa.alphabet = nfa.alphabet;
    /* Primeiro, incluiremos o estado inicial: */
    dfa.initialState = set<State>{ nfa.initialState };
    markToInclusion( dfa.initialState );

    while( !statesToBeIncluded.empty() ) {
        set< State > current = statesToBeIncluded.front();
        statesToBeIncluded.pop();
        for( Symbol a : nfa.alphabet ) {
            // Construiremos a transição de current com o símbolo a.
            set<State> next;
            for( State q : current )
                unite( next, delta( q, a ) );

            dfa.states.insert( current );
            dfa.delta.insert( {current, a}, next );
            markToInclusion( next );
        }
    }
    //Agora, definir os estados finais:
    for( const set<State>& DFAstate : dfa.states )
        for( const State& NFAstate : DFAstate )
            if( nfa.finalStates.count( NFAstate ) != 0 ) {
                dfa.finalStates.insert( DFAstate );
                continue;
                /* Os estados finais são aqueles que tem algum elemento
                 * como estado final do NFA. */
            }

    return dfa;
}

#endif // ALGORITHM_H
