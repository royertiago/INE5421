/* parseGrammar.h
 * Conjunto de funções que efetua o parsing de uma gramática livre
 * de contexto.
 *
 * A notação usada pelo parser é
 *
 *  Start -> a c Start | a
 *  c -> Start B c | B a
 *  c -> Start Start
 *
 * Os símbolos não-terminais são aqueles que aparecem à direita das
 * produções, e os demais são terminais. O primeiro não-terminal da
 * lista é o símbolo inicial da gramática.
 *
 * Na gramática de exemplo, temos
 * Não-terminais: "Start" e "c"
 * Terminais: "a" e "B"
 * Produções:
 * {"Start" -> "a", "c", "Start",
 *  "Start" -> "a",
 *  "c"     -> "Start", "B", "c",
 *  "c"     -> "B", "a",
 *  "c"     -> "Start", "Start" }
 * Símbolo inicial: "Start".
 *
 * Note que não há exigência de letras maiúsculas e minúsculas.
 * Espaços em branco (de acordo com std::isspace) são ignorados.
 *
 * O símbolo '&' será tratado como épsilon; além disso, uma produção
 * vazia também é considerada épsilon. Ou seja, são equivalentes
 *
 *  A -> a | &
 *  A -> a |
 *
 * e
 *
 *  A -> a | & | b
 *  A -> a || b
 *
 * e
 *  
 *  A -> &
 *  A ->
 *
 * e
 *
 *  A -> & | b
 *  A -> | b
 */
#ifndef PARSE_GRAMMAR_H
#define PARSE_GRAMMAR_H

#include <string>
#include <vector>
#include <cstdio>
using std::printf;
#include "exceptions.h"
#include "grammar/grammar.h"

/* Faz o parsing da gramática descrita pela lista de strings, de acordo
 * com a especificação acima.
 *
 * Strings vazias são ignoradas, exceto a primeira.
 *
 * Exceções lançadas:
 *  invalid_argument - caso o vetor esteja vazio ou a primeira string
 *                     seja constituida apenas de espaços em branco.
 *  invalid_left_hand_side
 *  invalid_replacement_symbol
 */
Grammar<std::string, std::string> parseGrammar( std::vector<std::string> );
#endif // PARSE_GRAMMAR_H
