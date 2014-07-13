/* eitherBase.h
 * União templatizada não-discriminada, usada internamente
 * pela classe Either.
 */
#ifndef EITHER_BASE_H
#define EITHER_BASE_H

#include <new>
#include <stdexcept>
#include <utility>
#include "utility/identity.h"
#include "utility/type_traits.h"

template< typename ... Ts >
union EitherBase;

template< typename Head, typename ... Tail >
union EitherBase< Head, Tail... > {
    Head head;
    EitherBase< Tail... > tail;

    static_assert( !std::is_reference<Head>::value,
            "There is no support to references." );
    static_assert( !std::is_const<Head>::value,
            "There is no support to const types." );
    static_assert( !std::is_volatile<Head>::value,
            "There is no support to volatile types." );

    /* Construtor padrão. Inicializado com o valor nulo
     * de Head. */
    EitherBase() :
        EitherBase( Head() )
    {}

    /* Constrói o primeiro elemento da união que possui
     * o mesmo tipo do objeto especificado.
     *
     * O primeiro construtor delega a construção para o próximo 
     * eitherBase na sequência; os dois últimos efetuam a 
     * construção de fato, quando o tipo correto chegar. 
     *
     * O segundo parâmetro do template abusa do SFINAE para garantir 
     * que apenas tipos que aparecem em Tail... estejam qualificados
     * para o primeiro construtor. */
    template< typename T,
        typename = typename std::enable_if<
            EitherBase<Tail...>::typeIndex( 0, 
                identity<
                    typename unqualified< T >::type
                >()
            ) != -1
        >::type
    >
    EitherBase( T&& obj ) {
        new (&tail) EitherBase<Tail...>( std::move(obj) );
    }
    EitherBase( const Head& obj ) {
        new (&head) Head( obj );
    }
    EitherBase( Head&& obj ) {
        new (&head) Head( std::move(obj) );
    }

    /* Construtores "de cópia/movimento" - copia os dados do elemento
     * especificado, no índice especificado.
     * index deve ser o índice do tipo ativo no momento. */
    EitherBase( const EitherBase& e, unsigned index ) {
        if( index == 0 )
            new (&head) Head( e.head );
        else
            new (&tail) EitherBase<Tail...>( e.tail, index - 1 );
    }
    EitherBase( EitherBase&& e, unsigned index ) {
        if( index == 0 )
            new (&head) Head( std::move(e.head) );
        else
            new (&tail) EitherBase<Tail...>( std::move(e.tail), index - 1 );
    }

    /* O destrutor desta classe faz nada.
     *
     * Ele existe apenas para que a classe seja utilizável se um dos tipos
     * não possuir construtor trivial. */
    ~EitherBase() {}

    /* Retorna o índice do tipo especificado.
     * 
     * A primeira função apenas redireciona para as outras duas, mas
     * descartando referências e const/volatile do tipo passado; ela
     * pode ser usada como
     *  eitherBase<Ts...>::template typeIndex<T>()
     *
     * A segunda e terceira funções fazem o "trabalho sujo", sem descartar
     * const/volatile. Caso os tipos desta classe sejam
     *  T0, T1, T2, ..., TN
     * typeIndex( j, identity<Ti> ) retorna j + i.
     *
     * Caso um tipo ocorra mais de uma vez na lista de tipos,
     * a primeira aparição é considerada.
     * Caso o tipo não ocorra na lista de tipos, -1 é retornado. */
    template< typename T >
    static constexpr unsigned typeIndex() {
        return typeIndex( 0, identity< typename unqualified<T>::type >() );
    }
    template< typename T >
    static constexpr unsigned typeIndex( unsigned current, identity<T> ) {
        return EitherBase<Tail...>::typeIndex( current + 1, identity<T>() );
    }
    static constexpr unsigned typeIndex( unsigned current, identity<Head> ) {
        return current;
    }


    /* Retorna o elemento do tipo especificado na união.
     * Nenhuma checagem é feita para garantir que o tipo especificado
     * é o tipo ativo no momento. */
    template< typename T >
    T& get( identity<T> ) {
        return tail.get( identity<T>() );
    }
    template< typename T >
    const T& get( identity<T> ) const {
        return tail.get( identity<T>() );
    }
    Head& get( identity<Head> ) {
        return head;
    }
    const Head& get( identity<Head> ) const {
        return head;
    }

    /* Destrói a variável do índice especificado.
     * index deve ser o índice da variável ativa no momento. */
    void destroy( unsigned int index ) {
        if( index == 0 )
            head.~Head();
        else
            tail.destroy( index - 1 );
    }

    /* Efetua atribuição de valores no índice especificado.
     * index deve ser o índice da variável ativa no momento. */
    void assignOnIndex( unsigned index, const EitherBase& e ) {
        if( index == 0 )
            head = e.head;
        else
            tail.assignOnIndex( index - 1, e.tail );
    }
    void assignOnIndex( unsigned index, EitherBase&& e ) {
        if( index == 0 )
            head = std::move(e.head);
        else
            tail.assignOnIndex( index - 1, std::move(e.tail) );
    }

    /* Operadores "relacionais": teste de ordem (operator<)
     * e igualdade (operator==) feitos pelos métodos
     * isSmallerOnIndex e isEqualsOnIndex.
     *
     * O teste será feito no índice especificado. */
    bool isSmallerOnIndex( unsigned index, const EitherBase& e ) const {
        if( index == 0 )
            return head < e.head;
        else
            return tail.isSmallerOnIndex( index - 1, e.tail );
    }
    bool isEqualsOnIndex( unsigned index, const EitherBase& e ) const {
        if( index == 0 )
            return head == e.head;
        else
            return tail.isEqualsOnIndex( index - 1, e.tail );
    }
};

// Especialização para EitherBase<>
template<>
union EitherBase<> {
    template< typename T >
    EitherBase( T&& ) {
        static_assert( dependant_false<T>::value,
                "No such type in EitherBase" );
    }

    // Construtor padrão
    EitherBase() = default;

    /* Construtores de cópia/movimento */
    EitherBase( const EitherBase&, unsigned ) {
        throw std::runtime_error( "Bad const Either&& construction." );
    }
    EitherBase( EitherBase&&, unsigned ) {
        throw std::runtime_error( "Bad Either&& construction." );
    }


    template< typename T >
    static constexpr unsigned typeIndex( unsigned, identity<T> ) {
        return -1;
    }

    template< typename T >
    const T& get( identity<T> ) const {
        static_assert( dependant_false<T>::value,
                "No such type in EitherBase" );
    }

    void destroy( unsigned ) {
        throw std::runtime_error( "Empty Either destruction\n" );
    }

    void assignOnIndex( unsigned, const EitherBase<>& ) {
        throw std::runtime_error( "Bad const Either& assign." );
    }
    void assignOnIndex( unsigned, EitherBase<>&& ) {
        throw std::runtime_error( "Bad Either&& assign." );
    }

    bool isSmallerOnIndex( unsigned, const EitherBase& ) const {
        throw std::runtime_error( "Empty Either on operator<" );
    }
    bool isEqualsOnIndex( unsigned, const EitherBase& ) const {
        throw std::runtime_error( "Empty Either on operator==" );
    }
};
#endif // EITHER_BASE_H
