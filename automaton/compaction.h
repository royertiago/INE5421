/* compaction.h
 * Conjunto de funções para compactação de autômatos.
 *
 * Definição: um autômato (Q, Sigma, Delta, Q0, F) é compacto
 * se Q = {0, 1, 2, ..., n-1} para algum n, e Q0 = 0.
 *
 * Teorema: todo autômato possui um autômato compacto equivalente.
 * Demonstração: veja a função compact() abaixo.
 *
 * Note que este algoritmo não preserva a disjunção entre o alfabeto
 * de entrada e o conjunto de estados, então tenha cuidado ao usar
 * esta função com autômatos cujo o tipo do símbolo é int. */
#ifndef COMPACTION_H
#define COMPACTION_H

#include <map>
#include "automaton/deterministic.h"
#include "automaton/nonDeterministic.h"

/* Automaton é o tipo do autômato a ser compactado,
 * State     é o tipo do estado do autômato,
 * Symbol    é o tipo do alfabeto deste autômato.
 *
 * Automaton e Symbol é preservado na compactação; o autômato
 * resultante é isomorfo ao fornecido.
 *
 * offset determina o elemento inicial do conjunto Q; isto é, na verdade,
 * teremos Q = {offset, offset + 1, offset + 2, ..., offset + |Q| - 1}. */
template< template< typename, typename > class Automaton,
          typename State, typename Symbol >
Automaton<int, Symbol> compact( const Automaton<State, Symbol>& input,
        int offset = 0 )
{
    Automaton< int, Symbol > output;
    Math::Function< State, int > f; // f é a função de mapeamento.

    auto remap = [&]( State q ) {
        int q_ = output.states.size() + offset; // q_ é q' - novo "nome" para q
        f.insert(q, q_);
        output.states.insert( q_ );
        if( input.finalStates.count( q ) > 0 ) // se q é final,
            output.finalStates.insert( q_ ); // q' também tem que ser.
        /* Como estamos adicionando elementos ao conjunto output.states,
         * o valor de output.states.size() corresponderá ao valor do
         * próximo elemento a ser mapeado. */
    };

    output.alphabet = input.alphabet;
    output.initialState = offset;
    remap( input.initialState );

    for( const auto& q : input.states )
        if( q != input.initialState )
            remap( q );

    for( const auto& pair : input.delta ) {
        // pair é um par (x, r), em que x é um par (q, a)
        auto q = pair.first.first;
        auto a = pair.first.second;
        auto r = pair.second;
        auto q_ = f(q);
        auto r_ = f(r);
        output.delta.insert( {q_, a}, r_ );
    }

    return output;
}

#endif // COMPACTION_H
