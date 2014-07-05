/* binaryTree.h
 * Árvore binária.
 */
#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <cstddef> // nullptr_t
#include <vector>
#include <type_traits>

template< typename T, typename Index = unsigned >
class BinaryTree {
    // Classe auxiliar
    struct node;
    std::vector< node > nodes;

public:
    typedef       T  value_type;
    typedef       T& reference;
    typedef const T& const_reference;
    typedef       T* pointer;
    typedef const T* const_pointer;

    class iterator;
    class const_iterator;

    /* Classe interna, que será o iterador da árvore.
     * Como ela está numa área pública, ela pode ser
     * acessada de fora da classe, mas não construída. */
    class iterator {
        friend class BinaryTree<T, Index>;

        BinaryTree<T, Index>* tree = nullptr;
        Index index = node::null;

        iterator( BinaryTree<T, Index>*, Index );

    public:
        typedef       T  value_type;
        typedef       T& reference;
        typedef const T& const_reference;
        typedef       T* pointer;
        typedef const T* const_pointer;
        /* Construtor disponível por conveniência; algum outro
         * iterador deve ser atribuido sobre este antes que ele
         * possa ser utilizado. */
        iterator() = default;

        /* Constrói um iterador nulo.
         * Note que o construtor é implícito. */
        iterator( nullptr_t );

        /* Retorna iteradores para os elementos especificados.
         * Caso não existam, um iterador apontando para o nodo nulo
         * é retornado.
         *
         * A nulidade de um iterador pode ser testada com operator bool() */
        iterator leftChild();
        iterator rightChild();
        iterator parent();

        /* Retorna um iterador para o filho da esquerda (direita).
         * Caso não exista, ele é criado com o valor nulo de T.
         *
         * Note que, apesar do nome, nem sempre o filho é criado:
         * ele só o é caso não exista. */
        iterator makeLeftChild();
        iterator makeRightChild();

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
         * É como se o nó atual "produzisse" um ascendente que assumirá
         * seu lugar na árvore. */
        void leftAscent();
        void rightAscent();

        /* Colapsa a subárvore da esquerda/direita.
         * Este método reverte leftAscent() / rightAscent(): dada uma árvore
         *      a
         *     / \
         *    b   c
         * executar collapseRight() no nodo 'a' resulta em
         *      c
         * O nodo b é deletado da árvore. */
        void collapseLeft();
        void collapseRight();

        /* Destrói toda a sub-árvore filha deste nodo. */
        void destroyLeftSubtree();
        void destroyRightSubtree();

        /* Altera o filho da esquerda/direita para ser o nó apontado
         * pelo iterador passado, ou o nodo nulo na terceira/sexta versão.
         *
         * Não será feita checagem alguma para garantir que esta estrutura
         * continuará sendo uma árvore. */
        void setLeftChild( iterator );
        void setLeftChild( const_iterator );
        void setLeftChild( std::nullptr_t );
        void setRightChild( iterator );
        void setRightChild( const_iterator );
        void setRightChild( std::nullptr_t );

        /* Retorna false caso o iterador aponta para o nodo nulo,
         * true caso contrário. */
        operator bool() const;
        /* Negação do operador acima. */
        bool operator!() const;

        /* Retorna o elemento apontado por este iterador. */
              reference operator*();
        const_reference operator*() const;
              pointer operator->();
        const_pointer operator->() const;

        /* Função para fins de debug - retorna o índice do iterador. */
        Index rawIndex() const;

        /* Função de conveniência, para uso em std::map e std::set.
         * A relação pai/filho entre os nós não é considerada,
         * os iteradores são assumidos pertencerem à mesma árvore,
         * e iteradores nulos são considerados menores que não-nulos. */
        friend bool operator<( const iterator& lhs, const iterator& rhs ) {
            if( !rhs ) return false;
            if( !lhs ) return true;
            return lhs.index < rhs.index;
        }
    };

    class const_iterator {
        friend class BinaryTree<T, Index>;

        const BinaryTree<T, Index>* tree = nullptr;
        Index index = node::null;

        const_iterator( const BinaryTree<T, Index>*, Index );
    public:
        typedef       T  value_type;
        typedef       T& reference;
        typedef const T& const_reference;
        typedef       T* pointer;
        typedef const T* const_pointer;
        const_iterator() = default;

        const_iterator leftChild();
        const_iterator rightChild();
        const_iterator parent();
        operator bool();
        const_reference operator*();
        const_pointer operator->();
    };

    /* Construtor padrão.
     * Constrói a árvore com apenas o nodo raiz, cujo valor é
     * o valor nulo de T. */
    BinaryTree();

    /* Retorna um iterador para a raiz da árvore. */
    iterator       root();
    const_iterator root() const;

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

    /* Efetua o colapso à esquerda (direita) do nodo especificado.
     * O nodo especificado será modificado para conter o nó filho.
     * Outras referências são mantidas. */
    void collapseLeft( Index );
    void collapseRight( Index );

    /* Destrói o nodo passado na primeira versão, e o nodo passado
     * e todos os subnós na segunda versão.
     * FIXME: a função destrói nada, na verdade. */
    void destroy( Index );
    void recursivelyDestroy( Index );
};

// Implementação

/* A estrutura interna da árvore é otimizada para ocupar menos
 * espaço, tanto na heap (por usar inteiros ao invés de ponteiros)
 * quanto no alocador do sistema (por não alocar nodos dinamicamente).
 *
 * Um nodo contém apenas os índices dos demais nodos. */
template< typename T, typename Index >
struct BinaryTree<T, Index>::node {
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

// BinaryTree - construtor
template< typename T, typename Index >
BinaryTree<T, Index>::BinaryTree() :
    nodes( 1, {node::null, node::null, node::null, T()} )
{}

// BinaryTree - iteradores
template< typename T, typename Index >
auto BinaryTree<T, Index>::root() -> iterator {
    return iterator( this, 0 );
}
template< typename T, typename Index >
auto BinaryTree<T, Index>::root() const -> const_iterator {
    return const_iterator( this, 0 );
}

// BinaryTree - raw data
template< typename T, typename Index >
auto BinaryTree<T, Index>::raw() const -> const std::vector<node>& {
    return nodes;
}

// BinaryTree - criação de filhos
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

// BinaryTree - ascensão
template< typename T, typename Index >
void BinaryTree<T, Index>::leftAscent( Index index ) {
    /* Copiaremos o nó atual para um novo lugar
     * e promoveremos o atual para pai da cópia. */
    Index childIndex = nodes.size();
    nodes.push_back( nodes[index] );

    // Corrigir os filhos do nodo pai
    nodes[index].rightChild = childIndex;
    nodes[index].leftChild = node::null;

    // Corrigir o pai do nodo filho
    nodes[childIndex].parent = index;

    /* Note que, como o nodo que ascendeu continou no mesmo
     * lugar, o nodo zero será sempre o nodo raíz. */
}

template< typename T, typename Index >
void BinaryTree<T, Index>::rightAscent( Index index ) {
    Index childIndex = nodes.size();
    nodes.push_back( nodes[index] );

    nodes[index].leftChild = childIndex;
    nodes[index].rightChild = node::null;

    nodes[childIndex].parent = index;
}

// BinaryTree - queda (colapso)
template< typename T, typename Index >
void BinaryTree<T, Index>::collapseLeft( Index index ) {
    /* Iremos destruir os nodos da sub-árvore da direita,
     * copiar o filho da esquerda para o nó atual e arrumar
     * as referências: o nó recém-copiado deve ter seu pai
     * corrigido e os filhos do antigo nó devem ter seus
     * pais trocados de lugar. */
    Index parentIndex = nodes[index].parent;
    Index childIndex = nodes[index].leftChild;
    if( nodes[index].rightChild != node::null )
        recursivelyDestroy( nodes[index].rightChild );

    nodes[index] = nodes[childIndex];
    nodes[index].parent = parentIndex;
    destroy( childIndex );
    if( nodes[index].leftChild != node::null )
        nodes[nodes[index].leftChild].parent = index;
    if( nodes[index].rightChild != node::null )
        nodes[nodes[index].rightChild].parent = index;
}

template< typename T, typename Index >
void BinaryTree<T, Index>::collapseRight( Index index ) {
    Index parentIndex = nodes[index].parent;
    Index childIndex = nodes[index].rightChild;
    if( nodes[index].leftChild != node::null )
        recursivelyDestroy( nodes[index].leftChild );

    nodes[index] = nodes[childIndex];
    nodes[index].parent = parentIndex;
    destroy( childIndex );
    if( nodes[index].leftChild != node::null )
        nodes[nodes[index].leftChild].parent = index;
    if( nodes[index].rightChild != node::null )
        nodes[nodes[index].rightChild].parent = index;
}

// BinaryTree - destruição
template< typename T, typename Index >
void BinaryTree<T, Index>::destroy( Index index ) {
    // FIXME: nada é feito para recuperar a memória perdida
}

template< typename T, typename Index >
void BinaryTree<T, Index>::recursivelyDestroy( Index index ) {
    // FIXME: nada é feito para recuperar a memória perdida
}


// Implementação do iterador

// Construtor
template< typename T, typename Index >
BinaryTree<T, Index>::iterator::iterator( BinaryTree<T, Index>* t, Index i ) :
    tree( t ),
    index( i )
{}

template< typename T, typename Index >
BinaryTree<T, Index>::iterator::iterator( std::nullptr_t ) :
    tree( nullptr ),
    index( node::null )
{}

// Navegação
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

// Criação de filhos
template< typename T, typename Index >
auto BinaryTree<T, Index>::iterator::makeLeftChild() -> iterator {
    if( tree->nodes[index].leftChild == node::null )
        tree->makeLeftChild( index );
    return leftChild();
}
template< typename T, typename Index >
auto BinaryTree<T, Index>::iterator::makeRightChild() -> iterator {
    if( tree->nodes[index].rightChild == node::null )
        tree->makeRightChild( index );
    return rightChild();
}

// Ascenção
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::leftAscent() {
    tree->leftAscent( index );
}
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::rightAscent() {
    tree->rightAscent( index );
}

// Queda (colapsamento)
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::collapseLeft() {
    tree->collapseLeft( index );
}
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::collapseRight() {
    tree->collapseRight( index );
}

// Destruir subárvores
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::destroyLeftSubtree() {
    if( tree->nodes[index].leftChild != node::null ) {
        tree->recursivelyDestroy( tree->nodes[index].leftChild );
        tree->nodes[index].leftChild = node::null;
    }
}
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::destroyRightSubtree() {
    if( tree->nodes[index].rightChild != node::null ) {
        tree->recursivelyDestroy( tree->nodes[index].rightChild );
        tree->nodes[index].rightChild = node::null;
    }
}

// Alterar "ponteiros"
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::setLeftChild( iterator it ) {
    tree->nodes[index].leftChild = it.index;
}
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::setLeftChild( const_iterator it ) {
    tree->nodes[index].leftChild = it.index;
}
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::setLeftChild( std::nullptr_t ) {
    tree->nodes[index].leftChild = node::null;
}

template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::setRightChild( iterator it ) {
    tree->nodes[index].rightChild = it.index;
}
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::setRightChild( const_iterator it ) {
    tree->nodes[index].rightChild = it.index;
}
template< typename T, typename Index >
void BinaryTree<T, Index>::iterator::setRightChild( std::nullptr_t ) {
    tree->nodes[index].rightChild = node::null;
}


// Funcionalidade básica
template< typename T, typename Index >
BinaryTree<T, Index>::iterator::operator bool() const {
    return index != node::null && tree != nullptr;
}

template< typename T, typename Index >
bool BinaryTree<T, Index>::iterator::operator !() const {
    return !operator bool();
}

template< typename T, typename Index >
T& BinaryTree<T, Index>::iterator::operator*() {
    return tree->nodes[index].data;
}

template< typename T, typename Index >
const T& BinaryTree<T, Index>::iterator::operator*() const {
    return tree->nodes[index].data;
}

template< typename T, typename Index >
T* BinaryTree<T, Index>::iterator::operator->() {
    return &tree->nodes[index].data;
}

template< typename T, typename Index >
const T* BinaryTree<T, Index>::iterator::operator->() const {
    return &tree->nodes[index].data;
}

template< typename T, typename Index >
Index BinaryTree<T, Index>::iterator::rawIndex() const {
    return index;
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
template< typename T, typename Index >
const T* BinaryTree<T, Index>::const_iterator::operator->() {
    return &tree->nodes[index].data;
}

#endif // BINARY_TREE_H
