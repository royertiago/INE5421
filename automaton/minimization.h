/* minimization.h
 * Minimização de autômatos finitos determinísticos.
 */
#ifndef MINIMIZATION_H
#define MINIMIZATION_H

#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include "automaton/deterministic.h"

/* Constrói o autômato mínimo equivalente ao autômato passado.
 * O autômato retornado será mínimo com relação ao número de
 * estados.
 *
 * Esta função coordena as demais funções deste cabeçalho. */
template< typename State, typename Symbol >
DFA< State, Symbol > minimize( DFA< State, Symbol > );

/* Remove os estados inalcançáveis a partir do estado inicial. */
template< typename State, typename Symbol >
DFA< State, Symbol > removeUnreachable( DFA< State, Symbol > );

/* Remove os estados mortos (que não alcançam estados finais).
 *
 * As transições para estes estados também serão removidas,
 * então talvez seja necessário completar o autômato depois. */
template< typename State, typename Symbol >
DFA< State, Symbol > removeDead( DFA< State, Symbol > );

/* Remove estados redundantes.
 * Dois estados são equivalentes se, trocar o estado inicial para
 * um deles resulta na mesma linguagem obtida ao trocar para o outro.
 * Esta função manterá apenas um destes estados no autômato retornado.
 */
template< typename State, typename Symbol >
DFA< State, Symbol > removeRedundant( DFA< State, Symbol > );

// Implementação
template< typename State, typename Symbol >
DFA< State, Symbol > minimize( DFA< State, Symbol > dfa ) {
    return removeRedundant( 
            completeTransitions( 
                removeDead( 
                    removeUnreachable( dfa )
                    )
                )
            );
}

template< typename State, typename Symbol >
DFA< State, Symbol > removeUnreachable( DFA< State, Symbol > dfa ) {
    std::map< State, bool > reachable;
    for( State q : dfa.states )
        reachable[q] = false;

    /* Usaremos o mapeamento reachable para dois propósitos:
     * indicar quais são os estados alcançáveis a partir do
     * estado inicial e indicar quais estados já foram "visitados"
     * pela função analyze.
     *
     * analyze irá percorrer os descendentes do estado passado,
     * marcando todos como alcançáveis; como é uma função lambda
     * recursiva (que não é diretamente suportada pelo C++), temos
     * de recorrer a std::function. */
    std::function< void(State) > analyze;
    analyze = [&]( State q ) {
        if( reachable[q] )
            return;

        reachable[q] = true;
        for( Symbol a : dfa.alphabet )
            if( dfa.delta.onDomain( {q, a} ) )
                analyze( dfa.delta({q, a}) );
    };

    analyze( dfa.initialState );

    for( auto pair : reachable )
        if( !pair.second )
            dfa.removeState( pair.first );

    return dfa;
}

template< typename State, typename Symbol >
DFA< State, Symbol > removeDead( DFA< State, Symbol > dfa ) {
    std::map< State, bool > alive;
    for( State q : dfa.states )
        alive[q] = dfa.finalStates.count( q ) > 0;

    bool changed = true;

    auto markAlive = [&]( State q ) {
        if( alive[q] ) return;
        alive[q] = true;
        changed = true;
    };

    /* true se a transição do estado pelo símbolo for para um
     * estado vivo, false caso contrário. */
    auto aliveTransition = [&]( std::pair< State, Symbol > p ) {
        if( dfa.delta.onDomain( p ) )
            return alive[ dfa.delta(p) ];
        return false;
    };

    /* true se existe alguma transição partindo deste estado
     * para algum estado vivo, false caso contrário. */
    auto hasAliveSuccessor = [&]( State q ) {
        for( Symbol a : dfa.alphabet )
            if( aliveTransition({ q, a }) )
                return true;
        return false;
    };

    /* true se a transição é para um estado que não está no
     * conjunto de estados, falso caso a transição seja indefinida
     * ou ela seja para um estado que está no conjunto de estados. */
    auto danglingTransition = [&]( State q, Symbol a ) {
        if( dfa.delta.onDomain({q, a}) )
            return dfa.states.count( dfa.delta({q, a}) ) == 0;
        return false;
    };

    while( changed ) {
        changed = false;
        for( State q : dfa.states )
            if( !alive[q] && hasAliveSuccessor(q) )
                markAlive( q );
    }

    for( auto pair : alive )
        if( !pair.second )
            dfa.removeState( pair.first );

    /* Removemos todos os estados mortos, agora falta remover
     * as transições para estes estados. */
    for( State q : dfa.states )
        for( Symbol a : dfa.alphabet )
            if( danglingTransition(q, a) )
                dfa.delta.erase({q, a});

    return dfa;
}

template< typename State, typename Symbol >
DFA< State, Symbol > removeRedundant( DFA< State, Symbol > dfa ) {
    using std::set;

    set< set< State > > equivalenceClasses;
    /* Se o conjunto de estados finais for vazio, o conjunto das
     * classes de equivalência conterá apenas o conjunto dos estados.
     * Caso contrário, começaremos com duas classes: finais e não-finais. */
    if( dfa.finalStates.empty() )
        equivalenceClasses.insert( dfa.states );
    else {
        set< State > alce;
        std::set_difference( dfa.states.begin(), dfa.states.end(),
                dfa.finalStates.begin(), dfa.finalStates.end(),
                std::inserter( alce, alce.end() ) );
        equivalenceClasses.insert( alce );
        equivalenceClasses.insert( dfa.finalStates );
    }

    /* true se os dois estados estão na mesma classe de equivalência,
     * false caso contrário. */
    auto sameEquivalenceClass = [&]( State q, State r ) {
        for( set< State > s : equivalenceClasses )
            if( s.count(q) > 0 ) {
                if( s.count(r) > 0 )
                    return true;
                else
                    return false;
            }
        return false;
    };

    /* Dois estados são equivalentes se eles pertencerem
     * às mesmas classes de equivalência e todas as transições
     * partindo deles terminam em estados nas mesmas classes
     * de equivalência. */
    auto equivalent = [&]( State q, State r ) {
        if( !sameEquivalenceClass( q, r ) )
            return false;
        for( Symbol a : dfa.alphabet )
            if( !sameEquivalenceClass( dfa.delta({q, a}), dfa.delta({r, a}) ) )
                return false;
        return true;
    };

    /* Quebra a classe de equivalência s em duas: uma contendo os
     * estados equivalentes a q, e outra contendo os demais estados.
     * 
     * A classe original é removida do conjunto de classes de equivalência
     * e as duas novas classes são inseridas. */
    auto split = [&]( set< State > s, State q ) {
        set< State > c1, c2;
        for( State r : s )
            if( equivalent( r, q ) )
                c1.insert( r );
            else
                c2.insert( r );
        equivalenceClasses.erase( s );
        equivalenceClasses.insert( c1 );
        equivalenceClasses.insert( c2 );
    };

    bool changed = true;
    while( changed ) { // Montar as classes de equivalência:
        changed = false;
        for( const set< State > s : equivalenceClasses ) {
            if( s.size() == 1 )
                continue;
            else {
                for( auto it = s.begin(); it != s.end(); ++it )
                    for( auto jt = it; jt != s.end(); ++jt )
                        if( !equivalent( *it, *jt ) ) {
                            split( s, *it );
                            changed = true;
                            goto continue_external_loop; // ineficiente...
                        }
            }
        }
continue_external_loop:
        ;
    }

    // Remontar o autômato

    /* Retorna o elemento que representará o conjunto passado. */
    auto representative = [&]( set< State > s ) {
        return *s.begin();
    };
    /* Retorna a classe de equivalência do estado passado. */
    auto equivalenceClass = [&]( State q ) {
        for( set< State > s : equivalenceClasses )
            if( s.count( q ) > 0 )
                return s;
        return set< State >();
    };
    /* Retorna o representante da classe de equivalência do estado. */
    auto classRepresentative = [&]( State q ) {
        return representative( equivalenceClass( q ) );
    };
    
    DFA< State, Symbol > r;
    r.alphabet = dfa.alphabet;
    r.initialState = classRepresentative( dfa.initialState );

    for( State q : dfa.states )
        r.states.insert( classRepresentative( q ) );

    for( State q : dfa.finalStates )
        r.finalStates.insert( classRepresentative( q ) );

    for( State q : r.states )
        for( Symbol a : r.alphabet )
            r.delta.insert( {q, a}, classRepresentative( dfa.delta({q, a}) ) );

    return r;
}
#endif // MINIMIZATION_H
