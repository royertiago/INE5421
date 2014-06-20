/* tokens.h
 * Cabeçalho que define os tokens disponíveis para a análise sintática.
 */
#ifndef TOKENS_H
#define TOKENS_H

#include <vector>
#include "utility/either.h"
#include "utility/type_traits.h"

/* Operadores lidos pelo programa.
 * Eles estão descritos na ordem de precedência. 
 *
 * Os operadores associam-se da esquerda para a direita; isto é,
 * a|b|c significa (a|b)|c. 
 *
 * O único operador para o qual isto é significativo é o operador libra.
 */
enum class Operator {
    /* Operador libra: x l y
     * Equivalente a x (y . x)*
     */
    SigmaClosure,

    /* Fecho de Klenee: x* */
    KleneeClosure,

    /* Fecho positivo: x+
     * Equivalente a x . x*
     */
    PositiveClosure,

    /* Opcional: x?
     * Equivalente a x | [epsilon]
     */
    Optional,

    /* Concatenação: x . y
     * É o único operador implícito; a justaposição de dois conjuntos
     * (xy) denota concatenação entre eles (x.y). */
    Concatenation,

    /* Barra vertical: x | y
     * Denota união de conjuntos. */
    VerticalBar,
};

enum class Parentheses {
    Left,
    Right
};

/* Um vetor de tokens é um vetor que contém caracteres, épsilon,
 * operadores e parênteses.
 *
 * Note que, como Either não suporta referências/const/volatile,
 * usamos unqualified<Char>::type ao invés de Char. */
template< typename Char >
using TokenVector = std::vector<Either<
        typename unqualified<Char>::type, Epsilon, Operator, Parentheses
    > >;

#endif // TOKENS_H
