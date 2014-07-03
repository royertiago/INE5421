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
bool operator< ( const tuple_iterator<T>&, const tuple_iterator<T>& );

template< typename T >
class tuple_iterator {
    std::vector< T > currentTuple;
    std::set< T > base;

public:
    /* Constrói um iterador sobre tuplas homogêneas que itera sobre
     * o conjunto passado, com tuplas fixas com o tamanho especificado. */
    tuple_iterator( std::set<T>, std::size_t );

    /* Retorna a tupla atual.
     * Note que o retorno é constante. */
    const std::vector< T >& operator*() const;

    /* Avança a iteração.
     * As tuplas estão ordenadas lexicograficamente, com o elemento
     * mais significativo sendo o mais próximo do fim.
     * Caso a última tupla de determinado tamanho seja atingido, a
     * tupla é aumentada de tamanho. */
    tuple_iterator< T >& operator++();
    tuple_iterator< T > operator++( int );

    /* Retrocede a iteração.
     * Assume-se que seja possível; isto é, não */
    tuple_iterator< T >& operator--();
    tuple_iterator< T > operator--( int );

    friend bool operator== <>(
            const tuple_iterator<T>&, const tuple_iterator<T>& );
    friend bool operator< <> (
            const tuple_iterator<T>&, const tuple_iterator<T>& );

private:
    bool advance( T& t ) {
        auto it = base.find( t );
        ++it;
        if( it == base.end() ) {
            t = *base.begin();
            return false;
        }
        t = *it;
        return true;
    }
};

/* Constrói um intervalo que conterá todas as "tuplas" sobre
 * o conjunto passado, do tamanho especificado. */
template< typename T >
range< tuple_iterator<T> > tuple_range( std::set< T >, std::size_t );

template< typename T >
bool operator<( const tuple_iterator<T>&, const tuple_iterator<T>& );
template< typename T >
bool operator>( const tuple_iterator<T>&, const tuple_iterator<T>& );
template< typename T >
bool operator<=( const tuple_iterator<T>&, const tuple_iterator<T>& );
template< typename T >
bool operator>=( const tuple_iterator<T>&, const tuple_iterator<T>& );
template< typename T >
bool operator==( const tuple_iterator<T>&, const tuple_iterator<T>& );
template< typename T >
bool operator!=( const tuple_iterator<T>&, const tuple_iterator<T>& );

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
tuple_iterator<T>& tuple_iterator<T>::operator--() {
    for( auto& t : currentTuple )
        if( retrocede( t ) )
            return *this;

    currentTuple.pop_back();
    return *this;
}

template< typename T >
tuple_iterator<T> tuple_iterator<T>::operator--( int ) {
    auto tmp = *this;
    operator--();
    return tmp;
}

template< typename T >
bool operator<( const tuple_iterator<T>& lhs, const tuple_iterator<T>& rhs ) {
    if( lhs.currentTuple.size() < rhs.currentTuple.size() )
        return true;
    if( lhs.currentTuple.size() > rhs.currentTuple.size() )
        return false;
    for( std::size_t i = 0; i < lhs.currentTuple.size(); ++i )
        if( lhs.currentTuple[i] < rhs.currentTuple[i] )
            return true;
        else if( lhs.currentTuple[i] > rhs.currentTuple[i] )
            return false;
    return false;
}

template< typename T >
range< tuple_iterator<T> > tuple_range( std::set< T > base, std::size_t n ) {
    return range< tuple_iterator<T> >(
            tuple_iterator<T>( base, n ), tuple_iterator<T>( base, n+1 )
            );
}

template< typename T >
bool operator>( const tuple_iterator<T>& lhs, const tuple_iterator<T>& rhs ) {
    return rhs < lhs;
}

template< typename T >
bool operator<=( const tuple_iterator<T>& lhs, const tuple_iterator<T>& rhs ) {
    return !( lhs > rhs );
}

template< typename T >
bool operator>=( const tuple_iterator<T>& lhs, const tuple_iterator<T>& rhs ) {
    return !( lhs < rhs );
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
