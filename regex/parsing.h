/* parsing.h
 * Cabeçalho que traduz expressões regulares para árvores
 * de expressão.
 */
#ifndef PARSING_H
#define PARSING_H

#include <stdexcept>
#include <vector>
#include "epsilon.h"
#include "regex/tokens.h"
#include "utility/binaryTree.h"
#include "utility/type_traits.h"

/* Exceção lançada pelas funções de análise sintática caso
 * um erro sintático seja encontrado. */
struct syntax_error : public std::runtime_error {
    explicit syntax_error( const char * what ) :
        runtime_error( what )
    {}
};

/* Efetua o parsing de todo o conteúdo do contêiner passado.
 * Os elementos deste contêiner devem ser convertíveis para char.
 *
 * O retorno é uma BinaryTree de Either<Char, Epsilon, Operator>,
 * em que Char é o tipo dos elementos do contêiner.
 *
 * Esta função simplesmente coordena o esforço das próximas funções. */
template< typename Container >
auto parse( const Container& c )
    -> BinaryTree<Either<
        typename unqualified<decltype(*c.begin())>::type, Epsilon, Operator
       >>;

/* Converte a sequência especificada em uma sequência de tokens.
 *
 * Os iteradores devem apontar para elementos conversíveis para char;
 * será assumido que a sequência não contêm caracteres que ocupem
 * mais de uma posição (como ocorre com UTF).
 *
 * Os caracteres de controle são:
 *  :*+?.|()&\
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

/* Explicita os operadores de concatenação numa sequência de tokens.
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

/* Constrói a árvore da expressão regular representada pelo vetor
 * de tokens passado.
 *
 * Caso a expressão não esteja sintaticamente correta, a exceção
 * bad_regex é lançada.
 */
template< typename Char >
BinaryTree< Either<Char, Epsilon, Operator> >
    buildExpressionTree( const TokenVector< Char >& );

/* Constrói uma subexpressão parentetizada no local apontado.
 * Por exemplo, a expressão 
 *  a.(a|b)
 * é analisada como
 *     .
 *    / \
 *   a  <subexpressão>
 * Esta função espera um TreeIterator apontando para um novo nodo na
 * na árvore, que será substituído pela estrutura adequada.
 *
 * Argumentos:
 *  where   Local da árvore em que deve ser construída a subexpressão.
 *  begin   Ponto de início da iteração. Deve apontar para o primeiro
 *          elemento após o abre-parênteses; no exemplo, begin deveria
 *          apontar para o 'a'. Note que este argumento é passado por 
 *          referência; ele será alterado para corresponder ao fecha-
 *          -parênteses terminal, ou a 'end', caso a sentença não contenha
 *          um fecha-parênteses.
 *  end     Limite da iteração. begin jamais será deslocado para além
 *          de end.
 *
 * Hipóteses sobre os tipos:
 *  TreeIterator    Aponta para um Either cujo primeiro tipo
 *                  é o caractere que está sendo considerado e possui
 *                  ao menos os tipos Epsilon e Operator.
 *  ForwardIterator Aponta para um Either que satisfaz todas as exigências
 *                  acima e também possui o tipo Parentheses.
 */
template< typename TreeIterator, typename ForwardIterator >
void buildSubexpression( TreeIterator     where,
                         ForwardIterator& begin,
                         ForwardIterator  end );

/* Similar à função anterior, mas interrompe a construção na primeira
 * barra vertical encontrada.
 * begin será deslocado até a primeira barra ou até end. */
template< typename TreeIterator, typename ForwardIterator >
void buildUntilBar( TreeIterator     where,
                    ForwardIterator& begin,
                    ForwardIterator  end );

/* Similar à função anterior, mas interrompe a construção no primeiro
 * operador de concatenação encontrado.
 * begin será deslocado até a primeira concatenação ou até end. */
template< typename TreeIterator, typename ForwardIterator >
void buildUntilCat( TreeIterator     where,
                    ForwardIterator& begin,
                    ForwardIterator  end );

/* Similar à função anterior, mas constrói apenas os operadores unários.
 * begin será deslocado até o primeiro operador binário ou até end. */
template< typename TreeIterator, typename ForwardIterator >
void buildUnary( TreeIterator     where,
                 ForwardIterator& begin,
                 ForwardIterator  end );

// Implementação
template< typename Container >
auto parse( const Container& c )
    -> BinaryTree<Either<
        typename unqualified<decltype(*c.begin())>::type, Epsilon, Operator
       >>
{
    return buildExpressionTree(
                explicitConcatenations(
                    tokenize( c.begin(), c.end() )
                )
            );
}

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
            case ':' : v.push_back( Operator::SigmaClosure    );    break;
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

// Parsing
template< typename Char >
BinaryTree< Either<Char, Epsilon, Operator> >
    buildExpressionTree( const TokenVector< Char >& v )
{
    BinaryTree< Either<Char, Epsilon, Operator> > tree;
    auto iterator = v.begin();
    buildSubexpression( tree.root(), iterator, v.end() );

    if( iterator != v.end() )
        throw syntax_error( "Unbalanced parentheses" );

    return tree;
}

template< typename TreeIterator, typename ForwardIterator >
void buildSubexpression( TreeIterator     nodeIterator,
                         ForwardIterator& currentToken,
                         ForwardIterator  iterationLimit )
{
    buildUntilBar( nodeIterator, currentToken, iterationLimit );
    while( currentToken != iterationLimit        &&
          *currentToken == Operator::VerticalBar )
    {
        nodeIterator.rightAscent();
        *nodeIterator = Operator::VerticalBar;
        ++currentToken;
        buildUntilBar( nodeIterator.rightDescent(),
                       currentToken, iterationLimit );
    }
}

template< typename TreeIterator, typename ForwardIterator >
void buildUntilBar( TreeIterator     nodeIterator,
                    ForwardIterator& currentToken,
                    ForwardIterator  iterationLimit )
{
    buildUntilCat( nodeIterator, currentToken, iterationLimit );
    while( currentToken != iterationLimit        &&
          *currentToken == Operator::Concatenation )
    {
        nodeIterator.rightAscent();
        *nodeIterator = Operator::Concatenation;
        ++currentToken;
        buildUntilCat( nodeIterator.rightDescent(), 
                       currentToken, iterationLimit );
    }
}

template< typename TreeIterator, typename ForwardIterator >
void buildUntilCat( TreeIterator     nodeIterator,
                    ForwardIterator& currentToken,
                    ForwardIterator  iterationLimit )
{
    buildUnary( nodeIterator, currentToken, iterationLimit );
    while( currentToken != iterationLimit        &&
          *currentToken == Operator::SigmaClosure )
    {
        nodeIterator.rightAscent();
        *nodeIterator = Operator::SigmaClosure;
        ++currentToken;
        buildUnary( nodeIterator.rightDescent(),
                    currentToken, iterationLimit );
    }
}

template< typename TreeIterator, typename ForwardIterator >
void buildUnary( TreeIterator     nodeIterator,
                 ForwardIterator& currentToken,
                 ForwardIterator  iterationLimit )
{
    // Como o tipo do caractere desapareceu até aqui, temos de reconstruí-lo:
    typedef typename extract_head_type< 
                typename TreeIterator::value_type
            >::type Char;

    if( *currentToken == Parentheses::Left ) {
        buildSubexpression( nodeIterator, currentToken, iterationLimit );
        if( *currentToken != Parentheses::Right )
            throw syntax_error( "Unbalanced parentheses" );
    } 
    else if( currentToken->template is<Char>() )
        *nodeIterator = currentToken->template operator Char();
    else if( currentToken->template is<Epsilon>() )
        *nodeIterator = epsilon;
    else if( *currentToken == Parentheses::Right )
        throw syntax_error( "Lacking inner symbol" );
    else
        throw syntax_error( "Wrong operator sequence" );

    ++currentToken;
    // Já construímos a "base". Agora, adicionemos os operadores unários:
    while( currentToken != iterationLimit &&
            (
              *currentToken == Operator::KleneeClosure   ||
              *currentToken == Operator::PositiveClosure ||
              *currentToken == Operator::Optional
            )
         )
    {
        nodeIterator.rightAscent();
        *nodeIterator = currentToken->template operator Operator();
        ++currentToken;
    }
}

#endif // PARSING_H
