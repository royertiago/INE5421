/* binaryTree.h
 * Árvore binária.
 */
#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <vector>
#include <type_traits>

/* A estrutura interna da árvore é otimizada para ocupar menos
 * espaço, tanto na heap (por usar inteiros ao invés de ponteiros)
 * quanto no alocador do sistema (por não alocar nodos dinamicamente).
 *
 * Um nodo contém apenas os índices dos demais nodos. */
template< typename T, typename Index >
struct TreeNode {
    static_assert( std::is_unsigned<Index>::value,
        "Index must be an unsigned integral type" );
    Index parent;
    Index leftChild;
    Index rightChild;
    T data;

    /* Como Index é um tipo primitivo sem sinal, o padrão do C++ garante
     * que -1 é o maior valor representável pelo tipo Index (4.7/2).
     *
     * Como a representação interna da árvore é um vetor de nodos,
     * representar o nodo nulo como 0 tiraria o espaço inicial do
     * vetor, desperdiçando sizeof(TreeNode) bytes. */
    static constexpr Index null = -1;
};

template< typename T, typename Index >
class BinaryTreeIterator {
};

template< typename T, typename Index = unsigned >
class BinaryTree {
    typedef TreeNode<T, Index> node;

    std::vector< node > nodes;
    Index root;

public:
    /* Classe interna, que será o iterador da árvore.
     * Como ela está numa área pública, ela pode ser
     * acessada de fora da classe, mas não construída. */
    class iterator {
        friend class BinaryTree<T, Index>;

        BinaryTree<T, Index>* tree = nullptr;
        Index index = node::null;

        iterator( BinaryTree<T, Index>*, Index );

    public:
        /* Construtor disponível por conveniência; algum outro
         * iterador deve ser atribuido sobre este antes que ele
         * possa ser utilizado. */
        iterator() = default;

        /* Retorna iteradores para os elementos especificados.
         * Caso não existam, um iterador apontando para o nodo nulo
         * é retornado.
         *
         * A nulidade de um iterador pode ser testada com operator bool() */
        iterator leftChild();
        iterator rightChild();
        iterator parent();

        /* Retorna um iterador para o filho da esquerda (direita).
         * Caso não exista, ele é criado com o valor nulo de T. */
        iterator leftDescent();
        iterator rightDescent();

        /* "Ascensão" do iterador.
         * Constrói um novo nodo (copiando o atual) que será colocado 
         * na posição do nó atual, posiciona o nó atual como filho da 
         * direita (esquerda) do novo nó, e altera o iterador para 
         * apontar para este novo nodo.
         *
         * Exemplo:
         *      a
         *     / \
         *    b   c
         *       / \
         *      d   e
         * Executar rightAscend() num iterador apontando para c resulta em
         *      a
         *     / \
         *    b   x
         *       /
         *      c
         *     / \
         *    d   e
         * x é um novo nó, que é o elemento apontado por c agora.
         *
         * TODO talvez inverter a nomenclatura.
         * TODO uniformizar a nomenclatura. leftDescent e leftAscent
         *      possuem comportamento bastante diferente.
         *
         * É como se o nó atual "produzisse" um ascendente que assumirá
         * seu lugar na árvore. */
        void leftAscent();
        void rightAscent();

        /* Retorna false caso o iterador aponte para o nodo nulo,
         * true caso contrário. */
        operator bool();

        /* Retorna o elemento apontado por este iterador. */
        T& operator*();
    };

    class const_iterator {
        friend class BinaryTree<T, Index>;

        const BinaryTree<T, Index>* tree = nullptr;
        Index index = node::null;

        const_iterator( const BinaryTree<T, Index>*, Index );
    public:
        const_iterator() = default;

        const_iterator leftChild();
        const_iterator rightChild();
        const_iterator parent();
        operator bool();
        const T& operator*();
    };

    /* Construtor padrão.
     * Constrói a árvore com apenas o nodo raíz, cujo valor é
     * o valor nulo de T. */
    BinaryTree();

    /* Retorna um iterador para a raíz da árvore. */
    iterator iteratorToRoot();
    const_iterator iteratorToRoot() const;

    /* Retorna o vetor de nodos desta árvore. */
    const std::vector<node>& raw() const;

private: // Métodos usados internamente
    /* Cria um novo nodo na árvore para ser o filho da esquerda
     * (direita) do nó no índice especificado.
     *
     * Assumiremos que o nodo especificado não possua
     * filho da esquerda (direita). */
    void makeLeftChild( Index );
    void makeRightChild( Index );

    /* Efetua a ascenção à esquerda (direita) do nodo especificado.
     * O nodo especificado será modificado para armazenar o novo nodo.
     * Outras referências são mantidas. */
    void leftAscent( Index );
    void rightAscent( Index );
};

// Implementação
template< typename T, typename Index >
BinaryTree<T, Index>::BinaryTree() :
    nodes( 1, {node::null, node::null, node::null, T()} )
{}

template< typename T, typename Index >
auto BinaryTree<T, Index>::iteratorToRoot() -> iterator {
    return iterator( this, 0 );
}
template< typename T, typename Index >
auto BinaryTree<T, Index>::iteratorToRoot() const -> const_iterator {
    return const_iterator( this, 0 );
}

template< typename T, typename Index >
void BinaryTree<T, Index>::makeLeftChild( Index index ) {
    nodes[index].leftChild = nodes.size();
    nodes.push_back({ index, node::null, node::null, T() });
}
template< typename T, typename Index >
void BinaryTree<T, Index>::makeRightChild( Index index ) {
    nodes[index].rightChild = nodes.size();
    nodes.push_back({ index, node::null, node::null, T() });
}

template< typename T, typename Index >
auto BinaryTree<T, Index>::raw() const -> const std::vector<node>& {
    return nodes;
}

template< typename T, typename Index >
void BinaryTree<T, Index>::leftAscent( Index index ) {
    /* Faremos o contrário: copiaremos o nó atual para um novo lugar
     * e apontamos o nó atual para o outro lado. */
    Index ascendentIndex = nodes.size();
    nodes[index].rightChild = ascendentIndex;
    nodes[index].leftChild = node::null;
    nodes.push_back( nodes[index] );
    nodes[ascendentIndex].parent = index;
}
template< typename T, typename Index >
void BinaryTree<T, Index>::rightAscent( Index index ) {
    Index ascendentIndex = nodes.size();
    nodes.push_back( nodes[index] );
    nodes[index].leftChild = ascendentIndex;
    nodes[index].rightChild = node::null;
    nodes[ascendentIndex].parent = index;
}

// Implementação do iterador
template< typename T, typename Index >
BinaryTree<T, Index>::iterator::iterator( BinaryTree<T, Index>* t, Index i ) :
    tree( t ),
    index( i )
{}

template< typename T, typename Index >
auto BinaryTree<T, Index>::iterator::parent() -> iterator {
    return iterator( tree, tree->nodes[index].parent );
}
template< typename T, typename Index >
auto BinaryTree<T, Index>::iterator::leftChild() -> iterator {
    return iterator( tree, tree->nodes[index].leftChild );
}
template< typename T, typename Index >
auto BinaryTree<T, Index>::iterator::rightChild() -> iterator {
    return iterator( tree, tree->nodes[index].rightChild );
}

template< typename T, typename Index >
auto BinaryTree<T, Index>::iterator::leftDescent() -> iterator {
    if( tree->nodes[index].leftChild == node::null )
        tree->makeLeftChild( index );
    return leftChild();
}
template< typename T, typename Index >
auto BinaryTree<T, Index>::iterator::rightDescent() -> iterator {
    if( tree->nodes[index].rightChild == node::null )
        tree->makeRightChild( index );
    return rightChild();
}

template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::leftAscent() {
    tree->leftAscent( index );
}
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::rightAscent() {
    tree->rightAscent( index );
}

template< typename T, typename Index >
BinaryTree<T, Index>::iterator::operator bool() {
    return index != node::null && tree != nullptr;
}

template< typename T, typename Index >
T& BinaryTree<T, Index>::iterator::operator*() {
    return tree->nodes[index].data;
}

// Implementação do iterador constante
template< typename T, typename Index >
BinaryTree<T, Index>::const_iterator::const_iterator( 
        const BinaryTree<T, Index>* t, Index i ) :
    tree( t ),
    index( i )
{}

template< typename T, typename Index >
auto BinaryTree<T, Index>::const_iterator::parent() -> const_iterator {
    return const_iterator( tree, tree->nodes[index].parent );
}
template< typename T, typename Index >
auto BinaryTree<T, Index>::const_iterator::leftChild() -> const_iterator {
    return const_iterator( tree, tree->nodes[index].leftChild );
}
template< typename T, typename Index >
auto BinaryTree<T, Index>::const_iterator::rightChild() -> const_iterator {
    return const_iterator( tree, tree->nodes[index].rightChild );
}

template< typename T, typename Index >
BinaryTree<T, Index>::const_iterator::operator bool() {
    return index != node::null && tree != nullptr;
}

template< typename T, typename Index >
const T& BinaryTree<T, Index>::const_iterator::operator*() {
    return tree->nodes[index].data;
}

#endif // BINARY_TREE_H
