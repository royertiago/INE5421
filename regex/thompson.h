/* thompson.h
 * Implementação do método de Thompson para conversão de expressões
 * regulares para autômatos finitos.
 */
#ifndef THOMPSON_H
#define THOMPSON_H

#include "epsilon.h"
#include "automaton/compaction.h"
#include "automaton/nonDeterministicWithEpsilon.h"
#include "regex/tokens.h"
#include "utility/binaryTree.h"
#include "utility/either.h"
#include "utility/type_traits.h"

/* Converte a árvore de expressão de uma expressão regular num
 * autômato finito com transições-épsilon.
 *
 * As demais funções deste cabeçalho são auxiliares e coordenadas
 * por esta função. */
template< typename Char >
NFAe< int, Char > thompson( BinaryTree< Either<Char, Epsilon, Operator> > );

/* Constrói um autômato finito que reconhece apenas a palavra 'c',
 * ou a palavra vazia (no segundo caso).
 *
 * Os autômatos retornados terão exatamente dois estados, um deles
 * terminal e o outro inicial, e exatamente uma transição, do estado
 * inicial para o final. */
template< typename Char >
NFAe< int, Char > nfaeTo( Char c );
template< typename Char >
NFAe< int, Char > nfaeTo( Epsilon = epsilon );

/* Constrói um autômato finito que reconhece apenas a palavra vazia. */

/* Constrói autômatos finitos equivalentes a fazer as operações de:
 *  - Concatenação    - Operator::Concatenation
 *  - União           - Operator::Union
 *  - Operador sigma  - Operator::SigmaClosure
 *  - Fecho de Klenee - Operator::KleneeClosure
 *  - Fecho positivo  - Operator::PositiveClosure
 *  - Opcional        - Operator::Optional
 * respectivamente. Tanto os autômatos de entrada quanto os de saída
 * devem satisfazer / satisfarão às seguintes exigências: 
 *  - O estado inicial não é reentrante (não existem transições para ele);
 *  - Existe um único estado final, que é diferente do estado inicial;
 *  - Não existem transições partindo do estado final.
 */
template< typename Char >
NFAe< int, Char > concatenate( NFAe< int, Char >, NFAe< int, Char > );
template< typename Char >
NFAe< int, Char > unite( NFAe< int, Char >, NFAe< int, Char > );
template< typename Char >
NFAe< int, Char > sigmaClosure( NFAe< int, Char >, NFAe< int, Char > );
template< typename Char >
NFAe< int, Char > klenneClosure( NFAe< int, Char > );
template< typename Char >
NFAe< int, Char > positiveClosure( NFAe< int, Char > );
template< typename Char >
NFAe< int, Char > optional( NFAe< int, Char > );

/* Executa o método de Thompson no nó passado. */
template< typename TreeIterator >
NFAe< int, typename extract_head_type< 
                typename TreeIterator::value_type 
            >::type >
thompson( TreeIterator t );

/* Executa a junção dos dois autômatos passados. Isto é, dados 
 * dois autômatos a e b, joinAutomaton irá modificar b para que
 * seu conjunto de estados não intersecte com a e retornar um autômato
 * finito r que contêm a e b dentro de si e possui um novo estado 
 * inicial e um novo estado final.
 *
 * Esta função é usada pelas funções concatenate, unite e sigmaClosure
 * para juntar os estados e transições dos dois autômatos num único.
 *
 * b é modificado para permitir que a função chamadora referencie o 
 * estado inicial/final de b dentro do autômato retornado pela variável
 * local. */
template< typename Char >
NFAe< int, Char > joinAutomaton( NFAe< int, Char > a, NFAe< int, Char >& b );

/* Extende o autômato passado com um novo estado inicial e um único
 * estado final, também novo. */
template< typename Char >
NFAe< int, Char > extendAutomaton( NFAe< int, Char > );

/* Açúcar sintático. Retorna o estado inicial/final do autômato passado. */
template< typename Char > int initialStateOf( NFAe< int, Char > );
template< typename Char > int   finalStateOf( NFAe< int, Char > );

// Implementação
template< typename Char >
NFAe< int, Char > thompson( BinaryTree< Either<Char, Epsilon, Operator> > t ) {
    return thompson( t.root() );
}

// Base
template< typename Char >
NFAe< int, Char > nfaeTo( Char c ) {
    return { /* Q */    {0, 1},
             /* Sigma */{c},
             /* delta */{ { {0, c}, {1} } 
                        },
             /* F */    {1},
             /* Q_0 */  0
    };
}

template< typename Char >
NFAe< int, Char > nfaeTo( Epsilon ) {
    return { /* Q */    {0, 1},
             /* Sigma */{},
             /* delta */{ { {0, epsilon}, {1} } 
                        },
             /* F */    {1},
             /* Q_0 */  0
    };
}

// Indução
template< typename Char >
NFAe< int, Char > concatenate( NFAe< int, Char > a, NFAe< int, Char > b ) {
    NFAe< int, Char > r = joinAutomaton( a, b );
    r.addTransition( initialStateOf( r ), epsilon, initialStateOf( a ) );
    r.addTransition(   finalStateOf( a ), epsilon, initialStateOf( b ) );
    r.addTransition(   finalStateOf( b ), epsilon,   finalStateOf( r ) );
    return r;
}

template< typename Char >
NFAe< int, Char > unite( NFAe< int, Char > a, NFAe< int, Char > b ) {
    NFAe< int, Char > r = joinAutomaton( a, b );
    r.addTransition( initialStateOf( r ), epsilon, initialStateOf( a ) );
    r.addTransition( initialStateOf( r ), epsilon, initialStateOf( b ) );
    r.addTransition(   finalStateOf( a ), epsilon,   finalStateOf( r ) );
    r.addTransition(   finalStateOf( b ), epsilon,   finalStateOf( r ) );
    return r;
}

template< typename Char >
NFAe< int, Char > sigmaClosure( NFAe< int, Char > a, NFAe< int, Char > b ) {
    NFAe< int, Char > r = joinAutomaton( a, b );
    r.addTransition( initialStateOf( r ), epsilon, initialStateOf( a ) );
    r.addTransition(   finalStateOf( a ), epsilon,   finalStateOf( r ) );
    r.addTransition(   finalStateOf( a ), epsilon, initialStateOf( b ) );
    r.addTransition(   finalStateOf( b ), epsilon, initialStateOf( a ) );
    return r;
}

template< typename Char >
NFAe< int, Char > kleneeClosure( NFAe< int, Char > a ) {
    NFAe< int, Char > r = positiveClosure( a );
    r.addTransition( initialStateOf( r ), epsilon, finalStateOf( r ) );
    return r;
}

template< typename Char >
NFAe< int, Char > positiveClosure( NFAe< int, Char > a ) {
    NFAe< int, Char > r = extendAutomaton( a );
    r.addTransition( initialStateOf( r ), epsilon, initialStateOf( a ) );
    r.addTransition(   finalStateOf( a ), epsilon, initialStateOf( a ) );
    r.addTransition(   finalStateOf( a ), epsilon,   finalStateOf( r ) );
    return r;
}

template< typename Char >
NFAe< int, Char > optional( NFAe< int, Char > a ) {
    a.addTransition( initialStateOf( a ), epsilon, finalStateOf( a ) );
    return a;
}

// Função que percorre a árvore
template< typename TreeIterator >
NFAe< int, typename extract_head_type< 
                typename TreeIterator::value_type 
            >::type >
thompson( TreeIterator t )
{
    typedef typename extract_head_type< 
                typename TreeIterator::value_type
            >::type Char;

    if( t->template is<Char>() )
        return nfaeTo( t->operator Char() );
    if( t->template is<Epsilon>() )
        return nfaeTo<Char>( epsilon );

    switch( t->template getAs<Operator>() ) {
        case Operator::KleneeClosure : 
            return kleneeClosure( thompson(t.leftChild()) );
        case Operator::PositiveClosure :
            return positiveClosure( thompson(t.leftChild()) );
        case Operator::Optional :
            return optional( thompson(t.leftChild()) );
        case Operator::SigmaClosure :
            return sigmaClosure( thompson(t.leftChild()), 
                                 thompson(t.rightChild()) );
        case Operator::Concatenation :
            return concatenate( thompson(t.leftChild()), 
                                thompson(t.rightChild()) );
        case Operator::VerticalBar :
            return unite( thompson(t.leftChild()), 
                          thompson(t.rightChild()) );
    };
    throw token_error( "Extraneous operator found" );
}

// Função auxiliare
template< typename Char >
NFAe< int, Char > joinAutomaton( NFAe< int, Char > a, NFAe< int, Char >& b ) {
    NFAe< int, Char > r; // retorno

    // Troquemos b.Q para não intersectar com a.Q.
    b = compact( b, * a.states.rbegin() + 1 );

    // Unamos os dois conjunto de estados e os dois alfabetos
    r.states = a.states; 
    r.states.insert( b.states.begin(), b.states.end() );
    r.alphabet = a.alphabet; 
    r.alphabet.insert( b.alphabet.begin(), b.alphabet.end() );

    for( auto pair : a.delta )
        r.delta.insert( pair.first, pair.second );
    for( auto pair : b.delta )
        r.delta.insert( pair.first, pair.second );

    return extendAutomaton( r );
}

template< typename Char >
NFAe< int, Char > extendAutomaton( NFAe< int, Char > a ) {
    int q0, f; // Novo estado inicial e final;
    a.states.insert( q0 = generateNewState( a ) );
    a.states.insert(  f = generateNewState( a ) );
    a.initialState = q0;
    a.finalStates = {f};
    return a;
}

template< typename Char > int initialStateOf( NFAe< int, Char > a ) {
    return a.initialState;
}
template< typename Char > int   finalStateOf( NFAe< int, Char > a ) {
    return *a.finalStates.begin();
    /* *set.begin() retorna o primeiro elemento do conjunto; como o
     * conjunto é unitário, *set.begin() retorna o estado final dos
     * autômatos. */
}

#endif // THOMPSON_H
