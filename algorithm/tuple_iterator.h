/* tuple_iterator.h
 * Iterador que itera sobre tuplas homogêneas de tamanho arbitrário
 * decidido em tempo de execução, representadas por vetores.
 */
#ifndef TUPLE_ITERATOR_H
#define TUPLE_ITERATOR_H

#include <set>
#include <vector>
#include "algorithm/range.h"

template< typename T >
class tuple_iterator;

template< typename T >
bool operator==( const tuple_iterator<T>&, const tuple_iterator<T>& );

template< typename T >
class tuple_iterator {
    std::vector< T > currentTuple;
    std::set< T > base;

public:
    /* Constrói um iterador sobre tuplas homogêneas que itera sobre
     * o conjunto passado, iniciando com tuplas do tamanho passado. */
    tuple_iterator( std::set<T>, std::size_t );

    /* Retorna a tupla atual. */
    const std::vector< T >& operator*() const;

    /* Avança a iteração.
     * As tuplas estão ordenadas lexicograficamente, com o elemento
     * mais próximo do fim como sendo o mais significativo.
     * Caso a última tupla de determinado tamanho seja atingido, a
     * tupla é aumentada de tamanho. */
    tuple_iterator< T >& operator++();
    tuple_iterator< T > operator++( int );

    /* Retrocede a iteração.
     * Assume-se que seja possível; isto é, a tupla atual não é uma
     * tupla unária cujo primeiro elemento é o primeiro do domínio. */
    tuple_iterator< T >& operator--();
    tuple_iterator< T > operator--( int );

    /* Comparação.
     * O teste é feito contra a tupla atual, ignorando o conjunto
     * sobre o qual a tupla está definida. */
    friend bool operator== <>(
            const tuple_iterator<T>&, const tuple_iterator<T>& );

private:
    /* Incrementa o elemento passado, segundo o conjunto base.
     * Caso o elemento de fato seja incrementado, a função retorna
     * true; caso o fim do conjunto seja atingido, o elemento é
     * reiniciado para base.begin() e o método retorna false. */
    bool advance( T& t );
};

/* Constrói um intervalo que conterá todas as "tuplas" sobre
 * o conjunto passado, do tamanho especificado. */
template< typename T >
range< tuple_iterator<T> > tuple_range( std::set< T >, std::size_t );


// Implementação
template< typename T >
tuple_iterator<T>::tuple_iterator( std::set<T> base, std::size_t s ) :
    currentTuple( s, *base.begin() ),
    base( base )
{}

template< typename T >
const std::vector<T>& tuple_iterator<T>::operator*() const {
    return currentTuple;
}

template< typename T >
tuple_iterator<T>& tuple_iterator<T>::operator++() {
    for( auto& t : currentTuple )
        if( advance( t ) )
            return *this;
    
    /* Apenas chegamos aqui caso todos os elementos das tuplas
     * tenham produzido carry, portanto, precisamos adicionar
     * mais um elemento à tupla. */
    currentTuple.push_back( *base.begin() );
    return *this;
}

template< typename T >
tuple_iterator<T> tuple_iterator<T>::operator++( int ) {
    auto tmp = *this;
    operator++();
    return tmp;
}

template< typename T >
bool tuple_iterator<T>::advance( T& t ) {
    auto it = base.find( t );
    ++it;
    if( it == base.end() ) {
        t = *base.begin();
        return false;
    }
    t = *it;
    return true;
}

template< typename T >
range< tuple_iterator<T> > tuple_range( std::set< T > base, std::size_t n ) {
    return range< tuple_iterator<T> >(
            tuple_iterator<T>( base, n ), tuple_iterator<T>( base, n+1 )
            );
}

template< typename T >
bool operator==( const tuple_iterator<T>& lhs, const tuple_iterator<T>& rhs ) {
    return lhs.currentTuple == rhs.currentTuple;
}

template< typename T >
bool operator!=( const tuple_iterator<T>& lhs, const tuple_iterator<T>& rhs ) {
    return !( lhs == rhs );
}
#endif // TUPLE_ITERATOR_H
