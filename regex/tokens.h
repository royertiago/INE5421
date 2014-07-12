/* tokens.h
 * Cabeçalho que define os tokens disponíveis para a análise sintática.
 */
#ifndef TOKENS_H
#define TOKENS_H

#include <vector>
#include "epsilon.h"
#include "exceptions.h"
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

    /* Operador libra: x : y
     * Equivalente a x (y . x)*
     */
    SigmaClosure,

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
 * temos de usar unqualified<Char>::type ao invés de Char; entretanto,
 * para não prejudicar a dedução de tipos, quem deve fazer a desqualificação
 * são as funções que usam TokenVector. */
template< typename Char >
using TokenVector = std::vector<Either<Char, Epsilon, Operator, Parentheses> >;

#endif // TOKENS_H
