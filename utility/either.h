/* either.h
 * União discriminada templatizada.
 *
 * Either é uma classe capaz de armazenar objetos de tipos diferentes,
 * desenvolvida para o uso em contêiners heterogêneos.
 *
 * Esta classe suporta objetos de quantos tipos forem necessários,
 * especifiados via templates. Objetos desta classe sempre conterão 
 * um elemento válido, de algum dos tipos especificados.
 *
 * Não há suporte para referências ou tipos const/volatile.
 */
#ifndef EITHER_H
#define EITHER_H

#include "eitherBase.h"
#include "utility/type_traits.h"

// Declaração dos operadores relacionais
template< typename ... Ts >
class Either;

/* Igualdade entre valores.
 * Os valores serão considerados iguais apenas se forem do mesmo
 * tipo e os valores internos forem iguais.
 * Exige-se que operator== esteja sobrecarregado para todos
 * os tipos armazenáveis pela classe.
 *
 * Este operador também está sobrecarregado para comparar diretamente
 * um Either com um dos tipos, exigindo, neste caso, apenas a sobrecarga
 * de operator==. */
template< typename ... Ts >
bool operator==( const Either<Ts...>&, const Either<Ts...>& );

/* Ordenamento.
 * Se os tipos internos diferirem, o tipo que vem antes na lista
 * de parâmetros para o template é considerado menor.
 * Caso contrário, a comparação é delegada para operator<, que
 * exige-se estar sobrecarregado para todos os tipos da classe. */
template< typename ... Ts >
bool operator<( const Either<Ts...>&, const Either<Ts...>& );


// Definição
template< typename ... Ts >
class Either {
    EitherBase< Ts... > value;
    unsigned int type;

public:
    /* Constrói a classe com o tipo especificado. */
    template< typename T,
        typename = typename std::enable_if<
            EitherBase<Ts...>::template typeIndex<T>() != -1
        >::type
    >
    Either( T&& t );

    /* Construtor padrão; inicializa a classe como contendo o primeiro
     * tipo da lista, com o valor nulo deste tipo. */
    Either();

    Either( const Either& );
    Either( Either&& );
    Either& operator=( const Either& );
    Either& operator=( Either&& );
    ~Either();

    /* Atribuição de qualquer dos dois tipos de objeto.
     * Caso o tipo interno do objeto mude, o objeto atual é destruído. */
    template< typename T,
        typename = typename std::enable_if<
            EitherBase<Ts...>::template typeIndex<T>() != -1
        >::type
    >
    Either& operator=( T&& );

    /* Determina se o tipo interno do objeto é o especificado.
     * Como esta função não possui parâmetros, sua chamada deve
     * ser parecida com:
     *  if( x.is<int>() )
     *      ... */
    template< typename T >
    bool is() const;

    /* Retorna o objeto armazenando internamente.
     * Não há erros caso o tipo interno seja diferente do operador
     * de conversão especificado. */
    template< typename T,
        typename = typename std::enable_if<
            EitherBase<Ts...>::template typeIndex<T>() != -1
        >::type
    >
    operator T() const;

    /* Retorna o objeto armazenado internamente.
     * Caso o tipo interno seja diferente do tipo requisitado,
     * std::bad_cast é lançado.
     *
     * Como esta função não possui parâmetro, invoque-a como:
     *  int i = x.getAs<int>(); */
    template< typename T >
    T getAs() const;

    friend bool operator== <>( const Either&, const Either& );
    friend bool operator< <> ( const Either&, const Either& );
};


// Implementação

// Construtor - a partir de elemento
template< typename ... Ts > template< typename T, typename >
Either<Ts...>::Either( T&& t ) :
    value( std::forward<T>(t) ),
    type( EitherBase<Ts...>::template typeIndex<T>() )
{}

// Construtor padrão
template< typename ... Ts >
Either<Ts...>::Either() :
    value(),
    type( 0 )
{}

// Construtor de cópia/movimento
template< typename ... Ts >
Either<Ts...>::Either( const Either<Ts...>& e ) :
    value( e.value, e.type ),
    type( e.type )
{}

template< typename ... Ts >
Either<Ts...>::Either( Either<Ts...>&& e ) :
value( std::move(e.value), e.type ),
    type( e.type )
{}

// Destrutor
template< typename ... Ts >
Either<Ts...>::~Either() {
    value.destroy( type );
}

// Atribuição de T's
template< typename ... Ts > template< typename T, typename >
Either<Ts...>& Either<Ts...>::operator=( T&& t ) {
    if( EitherBase<Ts...>::template typeIndex<T>() == type )
        value.get( identity<T>() ) = std::forward<T>(t);
    else {
        value.destroy( type );
        new (&value) EitherBase<Ts...>( std::forward<T>(t) );
        type = EitherBase<Ts...>::template typeIndex<T>();
    }
    return *this;
}

// Atribuição de Either's
template< typename ... Ts >
Either<Ts...>& Either<Ts...>::operator=( const Either<Ts...>& e ) {
    if( type == e.type )
        value.assignOnIndex( type, e.value );
    else {
        value.destroy( type );
        new (&value) EitherBase<Ts...>( e.value, e.type );
        type = e.type;
    }
    return *this;
}

template< typename ... Ts >
Either<Ts...>& Either<Ts...>::operator=( Either<Ts...>&& e ) {
    if( type == e.type )
        value.assignOnIndex( type, e.value );
    else {
        value.destroy( type );
        new (&value) EitherBase<Ts...>( std::move(e.value), e.type );
        type = e.type;
    }
    return *this;
}

// Funcionalidade básica
template< typename ... Ts > template< typename T >
bool Either<Ts...>::is() const {
    return EitherBase<Ts...>::template typeIndex<T>() == type;
}

template< typename ... Ts > template< typename T, typename >
Either<Ts...>::operator T() const {
    return value.get( identity<T>() );
}

template< typename ... Ts > template< typename T >
T Either<Ts...>::getAs() const {
    if( is<T>() )
        return operator T();
    throw std::runtime_error( "Wrong type Either cast\n" );
}

// Operadores relacionais
template< typename ... Ts >
bool operator==( const Either<Ts...>& lhs, const Either<Ts...>& rhs ) {
    return lhs.type == rhs.type && 
        lhs.value.isEqualsOnIndex( rhs.type, rhs.value );
}

template< typename T, typename ... Ts >
typename std::enable_if<
        EitherBase<Ts...>::template typeIndex<T>() != -1, bool
    >::type
operator==( const Either<Ts...>& lhs, const T& rhs ) {
    if( lhs.template is<T>() )
        return lhs.template operator T() == rhs;
    return false;
}

template< typename T, typename ... Ts >
typename std::enable_if<
        EitherBase<Ts...>::template typeIndex<T>() != -1, bool
    >::type
operator==( const T& lhs, const Either<Ts...>& rhs ) {
    return rhs == lhs;
}

template< typename ... Ts >
bool operator<( const Either<Ts...>& lhs, const Either<Ts...>& rhs ) {
    if( lhs.type < rhs.type )
        return true;
    if( rhs.type < lhs.type )
        return false;
    return lhs.value.isSmallerOnIndex( lhs.type, rhs.value );
}
#endif // EITHER_H
