/* type_traits.h
 * Cabeçalho que inclui o cabeçalho padrão type_traits,
 * além de definir algumas outras propriedades,
 * construídas sobre o cabeçalho padrão.
 */
#ifndef UTILITY_TYPE_TRAITS_H
#define UTILITY_TYPE_TRAITS_H

#include <type_traits>

/* Remove referências e const/volatile do tipo passado.
 * Assim, 
 *  const int&
 *  int&
 *  volatile int&&
 * viram todos <int>.
 * Note que ponteiros não são alterados. */
template< typename T >
struct unqualified {
    typedef typename std::remove_cv<
        typename std::remove_reference<T>::type
    >::type type;
};

/* dependant_false é uma estrutura que se comporta
 * de maneira idêntica a std::false_type, entretanto,
 * ela depende de um parâmetro do template.
 *
 * Isso permite formalizar 
 *  static_assert( false, "Não insancie com T genérico!" ); */
template< typename T >
struct dependant_false : std::false_type {};

/* Extrai o primeiro tipo de um template variádico. */
template< typename T >
struct extract_head_type;

template< template< typename... > class V, typename Head, typename ... Tail >
struct extract_head_type< V<Head, Tail...> > {
    typedef Head type;
};

#endif // UTILITY_TYPE_TRAITS_H
