/* either.h
 * Classe capaz de armazenar objetos de tipos diferentes.
 * Útil para o uso em contêiners heterogêneos.
 *
 * Esta classe suporta objetos de dois tipos apenas, especifiados
 * via templates. Objetos desta classe sempre conterão um elemento
 * válido, de algum dos dois tipos.
 *
 * A classe está especializada para o caso de os dois tipos
 * especificados serem o mesmo; para informações sobre semântica,
 * consulte cada método individualmente.
 *
 * Por enquanto, há suporte apenas a tipos POD.
 */
#ifndef EITHER_H
#define EITHER_H

#include <typeinfo> // std::bad_alloc

template< typename T, typename U >
class Either {
    union {
        T t;
        U u;
    };
    enum Type {firstType, secondType};
    Type type;

public:
    /* Constrói a classe com o tipo especificado. */
    Either( const T& t );
    Either( const U& u );

    /* Construtor padrão; inicializa a classe como contendo o tipo T,
     * com o valor nulo deste tipo.
     *
     * Equivalente a Either( T() ). */
    Either();

    /* Atribuição de qualquer dos dois tipos de objeto.
     * Não há invocação de destrutores ou qualquer coisa do tipo
     * caso o tipo interno do objeto mude. */
    Either& operator=( const T& );
    Either& operator=( const U& );

    /*  isFirst retorna true caso o tipo interno seja T,
     * isSecond retorna true caso o tipo interno seja U.
     * Se T == U, ambos os membros retornam true. */
    bool isFirst() const;
    bool isSecond() const;

    /* Retorna o objeto armazenando internamente.
     * Não há erros caso o tipo interno seja diferente do operador
     * de conversão especificado. */
    operator T() const;
    operator U() const;

    /* Retorna o objeto armazenado internamente.
     * Caso o tipo interno seja diferente do tipo requisitado,
     * std::bad_cast é lançado.
     * Esta função nunca lança exceções caso T == U. */
    T first() const;
    U second() const;
};

// Especialização para o caso T == U
template< typename T >
class Either<T, T> {
    T t;
public:
    Either( const T& t );
    Either() = default;
    Either& operator=( const T& );
    bool isFirst() const;
    bool isSecond() const;
    operator T() const;
    T first() const;
    T second() const;
};

// Operadores relacionais

/* Igualdade entre valores.
 * Os valores serão considerados iguais apenas se forem do mesmo
 * tipo e os valores internos forem iguais.
 * Exige-se que operator== esteja sobrecarregado para comparar dois
 * T e dois U. */
template< typename T, typename U >
bool operator==( const Either<T, U>&, const Either<T, U>& );

/* Ordenamento.
 * Valores T serão considerados ser sempre menores do que valores U;
 * caso sejam do mesmo tipo, a comparação será deferida para operator<,
 * que deve estar sobrecarregado para T e U. */
template< typename T, typename U >
bool operator<( const Either<T, U>&, const Either<T, U>& );


// Implementação

// Construtores
template< typename T, typename U >
Either<T, U>::Either( const T& t ) :
    t( t ),
    type( firstType )
{}

template< typename T, typename U >
Either<T, U>::Either( const U& u ) :
    u( u ),
    type( secondType )
{}

template< typename T >
Either<T, T>::Either( const T& t ) :
    t( t )
{}

template< typename T, typename U >
Either<T, U>::Either() :
    Either( T() )
{}

// Operador de atribuição
template< typename T, typename U >
Either<T, U>& Either<T, U>::operator=( const T& rhs ) {
    type = firstType;
    t = rhs;
    return *this;
}

template< typename T, typename U >
Either<T, U>& Either<T, U>::operator=( const U& rhs ) {
    type = secondType;
    t = rhs;
    return *this;
}

template< typename T >
Either<T, T>& Either<T, T>::operator=( const T& rhs ) {
    t = rhs;
    return *this;
}

// Teste de tipo
template< typename T, typename U >
bool Either<T, U>::isFirst() const {
    return type == firstType;
}

template< typename T, typename U >
bool Either<T, U>::isSecond() const {
    return type == secondType;
}

template< typename T >
bool Either<T, T>::isFirst() const {
    return true;
}

template< typename T >
bool Either<T, T>::isSecond() const {
    return true;
}

// Conversão implícita
template< typename T, typename U >
Either<T, U>::operator T() const {
    return t;
}

template< typename T, typename U >
Either<T, U>::operator U() const {
    return u;
}

template< typename T >
Either<T, T>::operator T() const {
    return t;
}

// Conversão explícita
template< typename T, typename U >
T Either<T, U>::first() const {
    if( type == firstType )
        return operator T();
    throw std::bad_cast();
}

template< typename T, typename U >
U Either<T, U>::second() const {
    if( type == secondType )
        return operator U();
    throw std::bad_cast();
}

template< typename T >
T Either<T, T>::first() const {
    return operator T();
}

template< typename T >
T Either<T, T>::second() const {
    return operator T();
}

// Operadores relacionais
template< typename T, typename U >
bool operator==( const Either<T, U>& lhs, const Either<T, U>& rhs ) {
    if( lhs.isFirst() )
        if( rhs.isFirst() )
            return lhs.operator T() == rhs.operator T();
        else
            return false;
    else
        if( rhs.isSecond() )
            return lhs.operator U() == rhs.operator U();
        else
            return false;
}

template< typename T, typename U >
bool operator<( const Either<T, U>& lhs, const Either<T, U>& rhs ) {
    if( lhs.isFirst() )
        if( rhs.isFirst() )
            return lhs.operator T() < rhs.operator T();
        else
            return true; // T < U
    else
        if( rhs.isSecond() )
            return lhs.operator U() < rhs.operator U();
        else
            return false; // U < T
}

#endif // EITHER_H
