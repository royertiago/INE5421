/* range.h
 * Classe que representa um intervalo de valores denotado
 * por iteradores [begin, end).
 *
 * Este cabeçalho também inclui as definições de std::begin
 * e std::end.
 */
#ifndef RANGE_H
#define RANGE_H

#include <iterator> // std::begin / std::end

template< typename Iterator >
class range {
    Iterator b, e; // begin/end

public:
    /* Constrói o range para atuar no intervalo [begin, end)
     * passado. Note que a classe é parametrizada por iterador,
     * não por contêiner. */
    range( Iterator begin, Iterator end );

    /* Informa se  intervalo é vazio; isto é, se os iteradores
     * são iguais. */
    bool empty() const;

    Iterator begin() const;
    Iterator end() const;
};


// Implementação
template< typename Iterator >
range< Iterator >::range( Iterator b, Iterator e ) :
    b( b ),
    e( e )
{}

template< typename Iterator >
bool range< Iterator >::empty() const {
    return b == e;
}

template< typename Iterator >
Iterator range<Iterator>::begin() const {
    return b;
}

template< typename Iterator >
Iterator range<Iterator>::end() const {
    return e;
}

#endif // RANGE_H
