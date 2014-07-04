/* trees.h
 * Algoritmos que trabalham com árvores.
 */
#ifndef TREES_H
#define TREES_H

#include <cstddef> // std::nullptr_t

/* Copia um ramo de uma árvore para outro ramo.
 * O ramo de origem não é modificado.
 * As árvores dos iteradores não precisam ser as mesmas. */
template< typename ConstTreeIterator, typename TreeIterator >
void copySubtree( ConstTreeIterator from, TreeIterator to );

/* Adiciona costuras à direita na árvore passada.
 * Note que a árvore pode se tornar inconsistente após esta
 * manipulação.
 *
 * O iterador passado será considerado a raíz da árvore. */
template< typename TreeIterator >
void addRightThreads( TreeIterator );

// Implementação
template< typename ConstTreeIterator, typename TreeIterator >
void copySubtree( ConstTreeIterator from, TreeIterator to ) {
    *to = *from;
    if( from.leftChild() )
        copySubtree( from.leftChild(), to.makeLeftChild() );
    if( from.rightChild() )
        copySubtree( from.rightChild(), to.makeRightChild() );
}

/* Função auxiliar. Adiciona costuras à árvore, de forma que o
 * nó mais à direita da sub-árvore apontada por current seja target,
 * ou, no caso da segunda sobrecarga, nullptr. */
template< typename TreeIterator >
void addRightThreads( TreeIterator target, TreeIterator current ) {
    if( current.rightChild() )
        addRightThreads( target, current.rightChild() );
    else
        current.setRightChild( target );

    if( current.leftChild() )
        addRightThreads( current, current.leftChild() );
}

template< typename TreeIterator >
void addRightThreads( std::nullptr_t, TreeIterator current ) {
    if( current.rightChild() )
        addRightThreads( nullptr, current.rightChild() );
    else
        current.setRightChild( nullptr );

    if( current.leftChild() )
        addRightThreads( current, current.leftChild() );
}

template< typename TreeIterator >
void addRightThreads( TreeIterator iterator ) {
    addRightThreads( nullptr, iterator );
}
#endif // TREES_H
