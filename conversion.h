/* conversion.h
 * Algoritmos de conversão entre as representações de linguagens
 * regulares suportados pelo programa:
 *  - Autômato finito determinístico;
 *  - Autômato finito não-determinístico;
 *  - Autômato finito não-determinístico com transições-épsilon;
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
#include "automaton/nonDeterministicWithEpsilon.h"
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
DFA< State, Symbol >                   toDFA( DFA< State, Symbol > );
template< typename State, typename Symbol >
DFA< std::set<State>, Symbol >         toDFA( NFA< State, Symbol > );
template< typename State, typename Symbol >
DFA< std::set<State>, Symbol >         toDFA( NFAe< State, Symbol > );
template< typename NonTerminal, typename Terminal >
DFA< std::set<NonTerminal>, Terminal > toDFA( Grammar<NonTerminal, Terminal> );

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
template< typename State, typename Symbol >
NFA< State, Symbol >         toNFA( NFAe< State, Symbol > );
template< typename NonTerminal, typename Terminal >
NFA< NonTerminal, Terminal > toNFA( Grammar< NonTerminal, Terminal > );

/* Converte representações de linguagens regulares para autômatos
 * finitos não deterministicos com transições-épsilon equivalentes.
 *
 * O valor de retorno é NFAe<N, T>, em que N é um tipo que varia
 * de acordo com a representação de entrada e T é o tipo do alfabeto
 * usado pela representação.
 */
template< typename State, typename Symbol >
NFAe< State, Symbol >         toNFAe( DFA< State, Symbol > );
template< typename State, typename Symbol >
NFAe< State, Symbol >         toNFAe( NFA< State, Symbol > );
template< typename State, typename Symbol >
NFAe< State, Symbol >         toNFAe( NFAe< State, Symbol > );
template< typename NonTerminal, typename Terminal >
NFAe< NonTerminal, Terminal > toNFAe( Grammar< NonTerminal, Terminal > );

/* Converte objetos para gramáticas livre de contexto equivalentes.
 * O valor de retorno é uma Grammar< N, T >, em que N é um tipo que
 * varia conforme o valor de retorno. T, os símbolos terminais,
 * são compatíveis com o alfabeto do elemento de entrada. 
 *  - Para NFAs e DFAs, T é o símbolo de entrada. 
 *
 * A gramática gerada é, garantidamente, regular. */
template< typename State, typename Symbol >
Grammar< State, Symbol >         toGrammar( DFA< State, Symbol > );
template< typename State, typename Symbol >
Grammar< State, Symbol >         toGrammar( NFA< State, Symbol > );
template< typename State, typename Symbol >
Grammar< State, Symbol >         toGrammar( NFAe< State, Symbol > );
template< typename NonTerminal, typename Terminal >
Grammar< NonTerminal, Terminal > toGrammar( Grammar< NonTerminal, Terminal > );


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

// NFAe para DFA (determinização)
template< typename State, typename Symbol >
DFA< std::set<State>, Symbol > toDFA( NFAe< State, Symbol > nfae ) {
    return toDFA( toNFA( nfae ) );
}

// Gramática para DFA
template< typename NonTerminal, typename Terminal >
DFA<std::set<NonTerminal>, Terminal> toDFA( Grammar<NonTerminal, Terminal> g ) {
    return toDFA( toNFA( g ) );
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

// NFA para NFA
template< typename State, typename Symbol >
NFA< State, Symbol > toNFA( NFA< State, Symbol > nfa ) {
    return nfa;
}

// NFAe para NFA
template< typename State, typename Symbol >
NFA< State, Symbol > toNFA( NFAe< State, Symbol > nfae ) {
    using std::pair;
    using std::set;
    NFA< State, Symbol > nfa;

    /* Une o segundo conjunto ao primeiro. */
    auto setUnion = []( set<State>& base, const set<State>& target ) {
        base.insert( target.begin(), target.end() );
    };
    /* epsilonClosureTo( q, a ) é a união de todos os delta( p, a ),
     * para p pertencente a epsilonClosure( q ).
     *
     * Isto é, é o conjunto de estados que pode ser alcançado a partir
     * de q, fazendo quantas transições-épsilon forem desejadas, e, por
     * fim, uma transição por a. */
    auto epsilonClosureTo = [&]( State q, Symbol a ) {
        set< State > r;
        for( State p : nfae.epsilonClosure( q ) )
            if( nfae.delta.onDomain({p, a}) )
                setUnion( r, nfae.delta({p, a}) );
        return r;
    };
    /* epsilonClosureOfSet( s ) é a união dos fechamentos-épsilon
     * dos estados do conjunto s. */
    auto epsilonClosureOfSet = [&]( set<State> s ) {
        set< State > r;
        for( State q : s )
            setUnion( r, nfae.epsilonClosure( q ) );
        return r;
    };
    /* reachableStatesWith( q, a ) é a união dos fechos-épsilon
     * dos conjuntos delta( p, a ) para p pertencente ao fecho-épsilon
     * de q.
     *
     * Isto é, o conjunto dos conjuntos alcançáveis a partir de q,
     * fazendo zero ou mais transições-épsilon, então uma transição
     * por a, e então zero ou mais transições-épsilon. */
    auto reachableStatesWith = [&]( State q, Symbol a ) {
        return epsilonClosureOfSet( epsilonClosureTo( q, a ) );
    };
    // true se a interseção dos dois conjuntos for vazia.
    auto emptyIntersection = []( set< State > a, set< State > b ) {
        for( State q : a )
            if( b.count( q ) > 0 )
                return false;
        return true;
    };

    nfa.states = nfae.states;
    nfa.alphabet = nfae.alphabet;

    for( State q : nfae.states )
        for( Symbol a : nfae.alphabet )
            nfa.delta.insert( {q, a}, reachableStatesWith( q, a ) );

    nfa.initialState = nfae.initialState;

    nfa.finalStates = nfae.finalStates;
    if( !emptyIntersection( 
                nfae.finalStates, 
                nfae.epsilonClosure( nfae.initialState )
                ) )
        nfa.finalStates.insert( nfa.initialState );

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

// DFA para NFAe
template< typename State, typename Symbol >
NFAe< State, Symbol > toNFAe( DFA< State, Symbol > automaton ) {
    return toNFAe( toNFA( automaton ) );
}

// NFA para NFAe
template< typename State, typename Symbol >
NFAe< State, Symbol > toNFAe( NFA< State, Symbol > nfa ) {
    NFAe< State, Symbol > nfae;
    nfae.states = nfa.states;
    nfae.alphabet = nfa.alphabet;
    for( const auto& pair : nfa.delta )
        nfae.delta.insert( pair.first, pair.second );
    nfae.initialState = nfa.initialState;
    nfae.finalStates = nfa.finalStates;
    return nfae;
}

// NFAe para NFAe
template< typename State, typename Symbol >
NFAe< State, Symbol > toNFAe( NFAe< State, Symbol > automaton ) {
    return automaton;
}

// Gramática para NFAe
template< typename NonTerminal, typename Terminal >
NFAe< NonTerminal, Terminal > toNFAe( Grammar< NonTerminal, Terminal > g ) {
    return toNFAe( toNFA( g ) );
}

// DFA para gramática
template< typename State, typename Symbol >
Grammar<State, Symbol> toGrammar( DFA<State, Symbol> dfa ) {
    return toGrammar( toNFA( dfa ) );
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

// NFAe para gramática
template< typename State, typename Symbol >
Grammar<State, Symbol> toGrammar( NFAe<State, Symbol> nfae ) {
    return toGrammar( toNFA( nfae ) );
}

// Gramática para gramática
template< typename NonTerminal, typename Terminal >
Grammar<NonTerminal, Terminal> toGrammar( Grammar<NonTerminal, Terminal> g ) {
    return g;
}

#endif // ALGORITHM_H
