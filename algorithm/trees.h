/* trees.h
 * Algoritmos que trabalham com árvores.
 */
#ifndef TREES_H
#define TREES_H

/* Copia um ramo de uma árvore para outro ramo.
 * O ramo de origem não é modificado.
 * As árvores dos iteradores não precisam ser as mesmas. */
template< typename ConstTreeIterator, typename TreeIterator >
void copySubtree( ConstTreeIterator from, TreeIterator to );

// Implementação
template< typename ConstTreeIterator, typename TreeIterator >
void copySubtree( ConstTreeIterator from, TreeIterator to ) {
    *to = *from;
    if( from.leftChild() )
        copySubtree( from.leftChild(), to.makeLeftChild() );
    if( from.rightChild() )
        copySubtree( from.rightChild(), to.makeRightChild() );
}
#endif // TREES_H
