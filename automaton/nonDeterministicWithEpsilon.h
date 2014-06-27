/* nonDeterministicWithEpsilon.h
 * Estrutura que representa um autômato finito não determinístico
 * com transições epsilon.
 */
#ifndef NON_DETERMINISTIC_WITH_EPSILON_H
#define NON_DETERMINISTIC_WITH_EPSILON_H

#include <set>
#include <utility>
#include "epsilon.h"
#include "math/function.h"
#include "utility/either.h"

template< typename State, typename Symbol >
struct NFAe {
    std::set< State > states;
    std::set< Symbol > alphabet;
    Math::Function< std::pair<State, Either<Symbol, Epsilon> >, 
                    std::set<State>
                  > delta;

    std::set< State > finalStates;
    State initialState;

    /* Determina o fecho-épsilon do estado passado.
     *
     * O fecho-épsilon é o conjunto de estados alcançáveis
     * a partir do estado apenas por transições-épsilon.
     * Note que um estado sempre pertencerá ao seu fecho-épsilon. */
    std::set< State > epsilonClosure( State ) const;

    /* Determina se o autômato aceita ou não a palavra delimitada
     * pelo intervalo [begin, end). */
    template< typename ForwardIterator >
    bool accepts( ForwardIterator begin, ForwardIterator end );

    /* Adiciona a transição do estado from para o estado to via s.
     * Nada é feito caso a transição já exista.
     *
     * É como se fosse adicionana uma aresta de from para to com
     * o símbolo s no grafo do autômato. */
    void addTransition( State from, Either<Symbol, Epsilon> s, State to );
};

// Implementação

template< typename State, typename Symbol >
std::set< State > NFAe<State, Symbol>::epsilonClosure( State q ) const {
    /* Retorna o conjunto de estados atingido por uma única transição-épsilon
     * a partir do estado passado. */
    auto epsilonMove = [&]( State q ) -> std::set< State > {
        if( delta.onDomain({q, epsilon}) )
            return delta({q, epsilon});
        return std::set< State >();
    };

    /* Une o segundo conjunto ao primeiro. */
    auto setUnion = []( std::set<State>& base, const std::set<State>& target ) {
        base.insert( target.begin(), target.end() );
    };

    std::set< State > old, current = {q};

    while( old != current ) {
        old = current;
        for( State s : old )
            setUnion( current, epsilonMove(s) );
    }
    return current;
}

template< typename State, typename Symbol >
void NFAe< State, Symbol >::addTransition( State from,
        Either<Symbol, Epsilon> s, State to )
{
    if( delta.onDomain({from, s}) ) {
        auto set = delta({from, s});
        set.insert( to );
        delta.insert({from, s}, set );
    } else
        delta.insert({from, s}, {to});
}
#endif // NON_DETERMINISTIC_WITH_EPSILON_H
