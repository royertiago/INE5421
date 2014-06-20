/* parsing.h
 * Cabeçalho que traduz expressões regulares para árvores
 * de expressão.
 */
#ifndef PARSING_H
#define PARSING_H

#include <vector>
#include "epsilon.h"
#include "regex/tokens.h"

/* Converte a sequência especificada em uma sequência de tokens.
 *
 * Os iteradores devem apontar para elementos conversíveis para char;
 * será assumido que a sequência não contêm caracteres que ocupem
 * mais de uma posição (como ocorre com UTF).
 *
 * Os caracteres de controle são:
 *  l*+?.|()&\
 *
 * & denota o épsilon. A aparição de uma contrabarra força o próximo
 * caractere a ser interpretado literalmente. O significado dos demais 
 * está explicado na documentação da enumeração Operator.
 *
 * A função tokenize() remove todos os pontos da função sendo avaliada;
 * outras funções devem lidar com o fato de a concatenação ser implícita.
 *
 * Contrabarra terminal é ignorado.
 */
template< typename ForwardIterator >
auto tokenize( ForwardIterator begin, ForwardIterator end ) ->
    TokenVector< decltype((*begin)) >;

// Implementação
template< typename ForwardIterator >
auto tokenize( ForwardIterator begin, ForwardIterator end ) ->
    TokenVector< decltype((*begin)) >
{
    TokenVector< decltype(*begin) > v;

    bool nextIsLiteral = false;

    for( ForwardIterator i = begin; i != end; ++i ) {
        if( nextIsLiteral ) {
            v.push_back( *i );
            nextIsLiteral = false;
            continue;
        }
        switch( char(*i) ) {
            case 'l' : v.push_back( Operator::SigmaClosure    );    break;
            case '*' : v.push_back( Operator::KleneeClosure   );    break;
            case '+' : v.push_back( Operator::PositiveClosure );    break;
            case '?' : v.push_back( Operator::Optional        );    break;
            case '|' : v.push_back( Operator::VerticalBar     );    break;
            case '(' : v.push_back( Parentheses::Left         );    break;
            case ')' : v.push_back( Parentheses::Right        );    break;
            case '&' : v.push_back( epsilon                   );    break;
            case '.' : /* não adicionaremos pontos */               break;
            case '\\': nextIsLiteral = true;                        break;
            default  : v.push_back( *i );                           break;
        }
    } // fim for

    return v;
}

#endif // PARSING_H
