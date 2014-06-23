/* closureProperties.h
 * Conjunto de funções que efetua operações entre autômatos,
 * como união, interseção e subtração.
 */
#ifndef CLOSURE_PROPERTIES_H
#define CLOSURE_PROPERTIES_H

#include "automaton/deterministic.h"
#include "automaton/nonDeterministic.h"
#include "automaton/nonDeterministicWithEpsilon.h"

/* Constrói um autômato finito determinístico cuja linguagem reconhecida
 * é a união, interseção ou subtração do primeiro pelo segundo autômato,
 * respectivamente.
 *
 * Assumiremos que os alfabetos dos dois autômatos são idênticos. */
template< typename State1, typename State2, typename Symbol >
DFA< std::pair<State1, State2>, Symbol > automataUnion( 
        DFA< State1, Symbol >, DFA< State2, Symbol > );
template< typename State1, typename State2, typename Symbol >
DFA< std::pair<State1, State2>, Symbol > automataIntersection( 
        DFA< State1, Symbol >, DFA< State2, Symbol > );
template< typename State1, typename State2, typename Symbol >
DFA< std::pair<State1, State2>, Symbol > automataSubtraction( 
        DFA< State1, Symbol >, DFA< State2, Symbol > );

/* Constrói um autômato que executa os dois autômatos simultaneamente,
 * e aceita uma palavra w, e somente se, pred( M1 aceita w, M2 aceita w )
 * for verdadeiro.
 * Por exemplo, se pred(x, y) ==  x && y, temos a operação de interseção.
 *
 * Assumiremos que os alfabetos dos dois autômatos são idênticos. */
template< typename State1, typename State2, typename Symbol >
DFA< std::pair<State1, State2>, Symbol > simultaneousRun( 
        DFA< State1, Symbol > M1, 
        DFA< State2, Symbol > M2, 
        bool (* pred)(bool, bool) );


// Implementação
template< typename State1, typename State2, typename Symbol >
DFA< std::pair<State1, State2>, Symbol > automataUnion( 
        DFA< State1, Symbol > M1, DFA< State2, Symbol > M2 )
{
    return simultaneousRun( M1, M2, [](bool x, bool y){ return x || y; } );
}

template< typename State1, typename State2, typename Symbol >
DFA< std::pair<State1, State2>, Symbol > automataIntersection( 
        DFA< State1, Symbol > M1, DFA< State2, Symbol > M2 )
{
    return simultaneousRun( M1, M2, [](bool x, bool y){ return x && y; } );
}
template< typename State1, typename State2, typename Symbol >
DFA< std::pair<State1, State2>, Symbol > automataSubtraction( 
        DFA< State1, Symbol > M1, DFA< State2, Symbol > M2 )
{
    return simultaneousRun( M1, M2, [](bool x, bool y){ return x && !y; } );
}

template< typename State1, typename State2, typename Symbol >
DFA< std::pair<State1, State2>, Symbol > simultaneousRun( 
        DFA< State1, Symbol > M1, 
        DFA< State2, Symbol > M2, 
        bool (* pred)(bool, bool) )
{
    using std::pair;
    DFA< State1, Symbol > dfa1 = completeTransitions( M1 );
    DFA< State2, Symbol > dfa2 = completeTransitions( M2 );
    DFA< pair< State1, State2 >, Symbol > dfa;

    for( State1 q1 : dfa1.states )
        for( State2 q2 : dfa2.states )
            dfa.states.insert({q1, q2});

    dfa.alphabet = dfa1.alphabet;

    for( State1 q1 : dfa1.states )
        for( State2 q2 : dfa2.states )
            for( Symbol a : dfa.alphabet )
                dfa.delta.insert({{q1, q2}, a}, {dfa1.delta({q1, a}), 
                                                 dfa2.delta({q2, a})});

    dfa.initialState = {dfa1.initialState, dfa2.initialState};
    
    auto isFinal1 = [&]( State1 q ){ return dfa1.finalStates.count(q) > 0; };
    auto isFinal2 = [&]( State2 q ){ return dfa2.finalStates.count(q) > 0; };
    for( State1 q1 : dfa1.states )
        for( State2 q2 : dfa2.states )
            if( pred( isFinal1(q1), isFinal2(q2) ) )
                dfa.finalStates.insert({q1, q2});

    return dfa;
}

#endif // CLOSURE_PROPERTIES_H
