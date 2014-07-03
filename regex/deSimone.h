/* deSimone.h
 * Implementação do algoritmo para conversão de expressões regulares
 * em autômatos finitos de Estevam De Simone.
 */
#ifndef DE_SIMONE_H
#define DE_SIMONE_H

#include "epsilon.h"
#include "algorithm/trees.h"
#include "automaton/deterministic.h"
#include "regex/tokens.h"
#include "utility/binaryTree.h"
#include "utility/either.h"

/* Converte a árvore referida em um autômato finito determinístico,
 * segundo o método de De Simone.
 *
 * As demais funções deste cabeçalho são auxiliares e coordenadadas
 * por esta função. */
template< typename Char >
DFA< int, Char > deSimone( BinaryTree< Either<Char, Epsilon, Operator> > );


/* O algoritmo de De Simone não é capaz de trabalhar com o operador libra
 * nem com épsilons na expressão regular. Estas duas funções removem
 * operadores libra, expandindo-os na definição do operador, e convertem
 * épsilons, de acordo com operadores anexos. Por exemplo,
 *  a | épsilon
 * vira
 *  a?
 * e
 *  x : y
 * vira
 *  x . ( y . x ) *
 */
template< typename Char >
void removeSigmaClosure( BinaryTree< Either< Char, Epsilon, Operator > >& );
template< typename Char >
void removeEpsilon( BinaryTree< Either<Char, Epsilon, Operator> >& );

/* Remove operadores libra no nó passado e abaixo dele. */
template< typename TreeIterator >
void removeSigmaClosure( TreeIterator );

/* Remove os épsilons abaixo do nó atual.
 * Caso o nó atual seja épsilon, a função retorna true; senão, false. */
template< typename TreeIterator >
bool removeEpsilon( TreeIterator );


// Implementação
template< typename Char >
void removeSigmaClosure( BinaryTree< Either<Char, Epsilon, Operator> >& tree )
{
    removeSigmaClosure( tree.root() );
}

template< typename Char >
void removeEpsilon( BinaryTree< Either<Char, Epsilon, Operator> >& tree ) {
    removeEpsilon( tree.root() );
}

/* Árvore original:
 *     :
 *    / \
 *   x   y
 *
 * Etapas da modificação:
 *     0        1       2      3       4
 *     :        .       .      .       .
 *    / \      / \     / \    / \     / \
 *   x   y    x   y   x   *  x   *   x   *
 *                       /      /       /
 *                      y      .       .
 *                              \     / \
 *                               y   x   y
 */
template< typename TreeIterator >
void removeSigmaClosure( TreeIterator iterator ) {
    if( !iterator->template is<Operator>() )
        return;
    switch( iterator->template getAs<Operator>() ) {
        case Operator::Concatenation:
        case Operator::VerticalBar:
            removeSigmaClosure( iterator.rightChild() );
            // fall-through intencional.
        case Operator::KleneeClosure:
        case Operator::PositiveClosure:
        case Operator::Optional:
            removeSigmaClosure( iterator.leftChild() );
            return;

        case Operator::SigmaClosure:
            *iterator = Operator::Concatenation; // Etapa 1
            TreeIterator lhs = iterator.leftChild(); // Usaremos depois

            iterator = iterator.rightChild();
            iterator.rightAscent();
            *iterator = Operator::KleneeClosure; // Etapa 2

            iterator = iterator.leftChild();
            iterator.leftAscent();
            *iterator = Operator::Concatenation; // Etapa 3

            copySubtree( lhs, iterator.makeLeftChild() ); // Etapa 4

            removeSigmaClosure( iterator.leftChild() );
            removeSigmaClosure( iterator.rightChild() );
            removeSigmaClosure( lhs );
            return;
    }
    throw token_error( "Extraneous operator found" ); 
}
#endif // DE_SIMONE_H
