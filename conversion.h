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
#include <utility>
#include "automaton/deterministic.h"
#include "automaton/nonDeterministic.h"
#include "automaton/newState.h"
#include "grammar/grammar.h"

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

/* Converte representações de linguagens formais para autômatos
 * finitos não determinísticos, sem transições-épsilon.
 *
 * O valor de retorno é NFA< N, T >, em que N é um tipo que varia
 * de acordo com a representação sendo convertida e T é o tipo
 * do alfabeto usado pela representação. */
template< typename State, typename Symbol >
NFA< State, Symbol >         toNFA( DFA< State, Symbol > );
template< typename State, typename Symbol >
NFA< State, Symbol >         toNFA( NFA< State, Symbol > );
template< typename NonTerminal, typename Terminal >
NFA< NonTerminal, Terminal > toNFA( Grammar< NonTerminal, Terminal > );

/* Converte objetos para gramáticas livre de contexto equivalentes.
 * O valor de retorno é uma Grammar< N, T >, em que N é um tipo que
 * varia conforme o valor de retorno. T, os símbolos terminais,
 * são compatíveis com o alfabeto do elemento de entrada. 
 *  - Para NFAs e DFAs, T é o símbolo de entrada. 
 *
 * A gramática gerada é, garantidamente, regular. */
template< typename NonTerminal, typename Terminal >
Grammar< NonTerminal, Terminal > toGrammar( Grammar< NonTerminal, Terminal > );
template< typename State, typename Symbol >
Grammar< State, Symbol >         toGrammar( NFA< State, Symbol > );

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

// DFA para DFA
template< typename State, typename Symbol >
NFA< State, Symbol > toNFA( NFA< State, Symbol > nfa ) {
    return nfa;
}

// DFA para NFA
template< typename State, typename Symbol >
NFA< State, Symbol > toNFA( DFA< State, Symbol > dfa ) {
    NFA< State, Symbol > nfa;
    nfa.states = dfa.states;
    nfa.alphabet = dfa.alphabet;
    for( auto& pair : dfa.delta )
        nfa.delta.insert( pair.first, {pair.second} );
    nfa.initialState = dfa.initialState;
    nfa.finalStates = dfa.finalStates;
    return nfa;
}

// Gramática para NFA
template< typename NonTerminal, typename Terminal >
NFA< NonTerminal, Terminal > toNFA( Grammar<NonTerminal, Terminal> g ) {
    NFA< NonTerminal, Terminal > nfa;

    auto addTransition = [&]( NonTerminal q, Terminal a, NonTerminal n ) {
        if( !nfa.delta.onDomain( {q, a} ) )
            nfa.delta.insert( {q, a}, {n} );
        auto set = nfa.delta({q, a});
        set.insert( n );
        nfa.delta.insert( {q, a}, set );
    };

    nfa.states = g.nonTerminals;
    nfa.alphabet = g.terminals;

    NonTerminal finalState = generateNewState( nfa );
    nfa.states.insert( finalState );
    nfa.finalStates = {finalState};
    nfa.initialState = g.startSymbol;

    for( Production<NonTerminal, Terminal> p : g.productions )
        if( p.right.size() == 1 )
            addTransition( p.left, p.right[0], finalState );
        else
            addTransition( p.left, p.right[0], p.right[1] );

    return nfa;
}

// Gramática para gramática
template< typename NonTerminal, typename Terminal >
Grammar<NonTerminal, Terminal> toGrammar( Grammar<NonTerminal, Terminal> g ) {
    return g;
}

// NFA para gramática
template< typename State, typename Symbol >
Grammar<State, Symbol> toGrammar( NFA<State, Symbol> nfa ) {
    using std::set;
    Grammar< State, Symbol > g; // Gramática a ser retornada

    auto isFinalState = [&nfa]( const State& q ) -> bool {
        return nfa.finalStates.count( q ) > 0;
    };
    auto nextStates = [&nfa]( const State& q, const Symbol& s ) -> set<State> {
        if( nfa.delta.onDomain({ q, s }) )
            return nfa.delta({q, s});
        return set<State>();
    };
    auto addProduction = [&g]( const Production<State, Symbol>& p ) -> void {
        g.productions.insert( p );
    };

    g.nonTerminals = nfa.states;
    g.terminals = nfa.alphabet;
    g.startSymbol = nfa.initialState;

    for( const State & q : nfa.states )
        for( const Symbol & s : nfa.alphabet )
            for( const State& next : nextStates( q, s ) ) {
                addProduction( {q, {s, next} } );
                if( isFinalState( next ) )
                    addProduction( {q, {s} } );
            }

    return g;
}
#endif // ALGORITHM_H
