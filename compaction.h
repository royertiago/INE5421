/* compaction.h
 * Conjunto de funções para compactação de autômatos.
 *
 * Definição: um autômato (Q, Sigma, Delta, Q0, F) é dito ser compacto
 * se Q = {0, 1, 2, ..., n-1} para algum n, e Q0 = 0.
 *
 * Teorema: todo autômato possui um autômato compacto equivalente.
 * Demonstração: veja as funções abaixo. */
#ifndef COMPACTION_H
#define COMPACTION_H

#include <map>
#include "automaton/deterministic.h"
#include "automaton/nonDeterministic.h"

/* Compacta o autômato passado.
 * O alfabeto é preservado e o autômato resultante será equivalente ao
 * (e do mesmo tipo do) autômato de entrada. */
template< typename State, typename Symbol >
DFA<int, Symbol> compact( DFA<State, Symbol> );
template< typename State, typename Symbol >
NFA<int, Symbol> compact( NFA<State, Symbol> );

// Implementação

// Autômatos finitos determinísticos
template< typename State, typename Symbol >
DFA<int, Symbol> compact( DFA<State, Symbol> input ) {
    DFA< int, Symbol > output;
    Math::Function< State, int > f; // f é a função de mapeamento.

    auto remap = [&]( State q ) {
        int q_ = output.states.size(); // q_ == q' -> novo "nome" para q
        f.insert(q, q_);
        output.states.insert( q_ );
        if( input.finalStates.count( q ) > 0 ) // se q é final,
            output.finalStates.insert( q_ ); // q' também tem que ser.
        /* Como estamos adicionando elementos ao conjunto output.states,
         * o valor de output.states.size() corresponderá ao valor do
         * próximo elemento a ser mapeado. */
    };

    output.alphabet = input.alphabet;
    output.initialState = 0;
    remap( input.initialState );

    for( State q : input.states )
        if( q != input.initialState )
            remap( q );

    for( const auto& pair : input.delta ) {
        // pair é um par (x, r), em que x é um par (q, a)
        State q = pair.first.first;
        Symbol a = pair.first.second;
        State r = pair.second;
        int q_ = f(q);
        int r_ = f(r);
        output.delta.insert( {q_, a}, r_ );
    }

    return output;
}

// Autômatos finitos não-determinísticos
template< typename State, typename Symbol >
NFA<int, Symbol> compact( NFA<State, Symbol> input ) {
    NFA< int, Symbol > output;
    Math::Function< State, int > f; // f é a função de mapeamento.

    using std::set;

    auto remap = [&]( State q ) {
        int q_ = output.states.size(); // q_ == q' -> novo "nome" para q
        f.insert(q, q_);
        output.states.insert( q_ );
        if( input.finalStates.count( q ) > 0 ) // se q é final,
            output.finalStates.insert( q_ ); // q' também tem que ser.
    };

    output.alphabet = input.alphabet;
    output.initialState = 0;
    remap( input.initialState );

    for( State q : input.states )
        if( q != input.initialState )
            remap( q );

    for( const auto& pair : input.delta ) {
        // pair é um par (x, r), em que x é um par (q, a)
        State q = pair.first.first;
        Symbol a = pair.first.second;
        set<State> r = pair.second;
        int q_ = f(q);
        set<int> r_ = f(r);
        output.delta.insert( {q_, a}, r_ );
    }

    return output;
}


#endif // COMPACTION_H
