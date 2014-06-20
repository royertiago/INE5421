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
    TokenVector< typename unqualified<decltype(*begin)>::type >;

/* Esta função explicita a concatenação numa sequência de tokens.
 * Mas precisamente, um Operator::Concatenation será adicionado
 * antes de cada:
 *  Parentheses::Left;
 *  Char; e
 *  épsilon,
 * exceto se esses caracteres forem precedidos de
 *  SigmaClosure;
 *  VerticalBar;
 *  Concatenation;
 *  Parentheses::Left;
 *  ou estiverem no início do vetor.
 */
template< typename Char >
TokenVector< Char > explicitConcatenations( const TokenVector< Char >& );

// Implementação
template< typename ForwardIterator >
auto tokenize( ForwardIterator begin, ForwardIterator end ) ->
    TokenVector< typename unqualified<decltype(*begin)>::type >
{
    TokenVector< typename unqualified<decltype(*begin)>::type > v;

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

template< typename Char >
TokenVector< Char > explicitConcatenations( const TokenVector< Char >& in ) {
    TokenVector< Char > out;
    bool skipNext = true; // não adicionamos '.' no começo do vetor.

    for( auto e : in ) {
        if( skipNext ) {
            out.push_back( e );
            skipNext = false;
            continue;
        }

        if(
            e.template is<Char>()    || 
            e.template is<Epsilon>() ||
            e == Parentheses::Left
          )
            out.push_back( Operator::Concatenation );

        if(
            e == Operator::VerticalBar   ||
            e == Operator::SigmaClosure  ||
            e == Operator::Concatenation ||
            e == Parentheses::Left
          )
            skipNext = true;

        out.push_back( e );
    }
    return out;
}

#endif // PARSING_H
