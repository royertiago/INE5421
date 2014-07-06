/* deSimone.h
 * Implementação do algoritmo para conversão de expressões regulares
 * em autômatos finitos de Estevam De Simone.
 */
#ifndef DE_SIMONE_H
#define DE_SIMONE_H

#include <functional>
#include <map>
#include <utility>
#include "epsilon.h"
#include "algorithm/trees.h"
#include "automaton/compaction.h"
#include "automaton/deterministic.h"
#include "regex/tokens.h"
#include "utility/binaryTree.h"
#include "utility/either.h"
#include "utility/type_traits.h"

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

/* Constrói as composições dos nós folha da árvore.
 * Esta função assume que a árvore passada está costurada.
 *
 * A composição de um nó é o conjunto de nós que são alcançados
 * na próxima "transição", ao interpretar a árvore como uma
 * expressão regular. Por exemplo:
 *          .
 *         / \
 *        .   c
 *       / \
 *      a   b
 * Após reconhecer um 'a', um autômato construído para esta árvore
 * pode apenas reconhecer um 'b'; portanto, a composição do nodo
 * 'a' é {'b'}. Similarmente, a composição de 'b' é {'c'}.
 *          .
 *         / \
 *        a   |
 *        1  / \
 *          a   b
 *          2   3
 * Aqui, existem dois nodos iguais a 'a', portanto, vamos numerá-los.
 * A partir do nodo 1, a próxima transição pode reconhecer um 'a',
 * no nodo 2, ou um 'b', no nodo 3; alcançamos, portanto, dois nodos: 2 e 3.
 * A composição de 1 é {2, 3}. A partir do nodo 2, não é possível alcançar
 * o nodo 3, pois já "escolhemos" este lado do operador de união.
 * De fato, após efetuar a transição por 'a', chegamos ao fim da árvore. 
 * Para representar este fato, incluiremos um nó nulo na sua composição;
 * portanto, a composição de 2 e de 3 é {0}.
 *
 * Um nodo pode pertencer à sua composição:
 *          .
 *         / \
 *        *   .
 *       /   / \
 *      a   b   c
 *      1   2   3
 * A partir do nodo 1, podemos avançar pela árvore para alcançar 2,
 * ou voltar para o nodo 1 (por causa do fechamento). Portanto, a composição
 * do nodo 1 é {1, 2}.
 *
 * A composição inicial é o conjunto de nodos que podem ser alcançados no
 * início da execução do autômato. Para o exemplo anterior, o autômato
 * pode começar reconhecendo um 'a' (descendo pelo fechamento) ou ignorar
 * o 'a' e reconhecer um 'b'. Portanto, a composição inicial desta árvore
 * é {1, 2}.
 *
 * Por simplicidade, identificaremos os nós pelos iteradores que apontam
 * para eles.
 *
 * Esta função retorna um par de valores. O primeiro é a composição
 * inicial da árvore, e o segundo é um mapa que contém os pares
 * (nodo, composição), descrito acima. O nó nulo é representado pelo
 * iterador nulo.
 */
template< typename TreeIterator >
std::pair< std::set< TreeIterator >,
           std::map< TreeIterator, std::set< TreeIterator > >
         >
buildComposition( TreeIterator root );

/* Constrói o autômato finito a partir da composição inicial e da
 * composição de cada nodo folha.
 *
 * Note que o autômato retornado possui o conjunto de estados sobre
 * std::set< TreeIterator >, portanto é necessário compactá-lo. */
template<
        typename TreeIterator,
        typename Char = typename extract_head_type<
                    decltype(*std::declval<TreeIterator>())
                >::type
        >
DFA< std::set< TreeIterator >, Char >
buildAutomaton(
        std::pair<
            std::set< TreeIterator >,
            std::map< TreeIterator, std::set< TreeIterator > >
        >
    );


// Implementação
template< typename Char >
DFA< int, Char > deSimone( BinaryTree<Either<Char, Epsilon, Operator>> tree ) {
    removeSigmaClosure( tree );
    removeEpsilon( tree );
    if( *tree.root() == epsilon )
        return { /* Q */     {0},
                 /* Sigma */ {},
                 /* Delta */ {},
                 /* F */     {0},
                 /* Q_0*/    0
        };
    addRightThreads( tree.root() );
    return compaction(
                buildAutomaton(
                    forwardCompositions(
                        tree.root()
                    )
                )
           );
}

template< typename Char >
void removeSigmaClosure( BinaryTree< Either<Char, Epsilon, Operator> >& tree )
{
    removeSigmaClosure( tree.root() );
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

template< typename Char >
void removeEpsilon( BinaryTree< Either<Char, Epsilon, Operator> >& tree ) {
    removeEpsilon( tree.root() );
}

template< typename TreeIterator >
bool removeEpsilon( TreeIterator iterator ) {
    if( iterator->template is<Epsilon>() )
        return true;
    if( !iterator->template is<Operator>() )
        return false;

    switch( iterator->template operator Operator() ) {
        case Operator::KleneeClosure:
        case Operator::PositiveClosure:
        case Operator::Optional:
            /* O fecho de Klenne, fecho positivo e opcional do épsilon
             * é igual a épsilon; caso a subárvore seja épsilon,
             * a destruiremos e substituiremos o nodo atual por épsilon. */
            if( removeEpsilon( iterator.leftChild() ) ) {
                iterator.destroyLeftSubtree();
                *iterator = epsilon;
                return true;
            }
            return false;

        case Operator::Concatenation:
            /* Para a concatenação temos quatro casos:
             *  x       . y       = x.y (se mantém)
             *  x       . épsilon = x
             *  épsilon . y       = y
             *  épsilon . épsilon = épsilon
             * O segundo caso aparece caso a remoção de épsilons na
             * subárvore à direita resulte em épsilon; é o caso
             * do primeiro if.
             *
             * Neste caso, destruamos a subárvore da direita e troquemos
             * a concatenação pela subárvore da esquerda. Para pegar
             * o quarto caso já neste if, basta analisar a subárvore da
             * esquerda, que agora é o nodo atual.
             *
             * O terceiro caso ocorre na situação contrária: a análise
             * da subárvore da esquerda concluiu que lá só tem épsilon.
             * Note que agora não precisamos fazer a análise da subárvore
             * da direita aqui, pois ela já foi feita no teste anterior
             * e já sabemos que a subárvore da direita não é épsilon.
             *
             * Finalmente, o return false lida com o primeiro caso. */
            if( removeEpsilon( iterator.rightChild() ) ) {
                iterator.destroyRightSubtree();
                iterator.collapseLeft();
                return removeEpsilon( iterator );
            }
            if( removeEpsilon( iterator.leftChild() ) ) {
                iterator.destroyLeftSubtree();
                iterator.collapseRight();
            }
            return false;

        case Operator::VerticalBar:
            /* Os quatro casos daqui são
             * x       | y       = x | y (se mantém)
             * x       | épsilon = x?
             * épsilon | y       = y?
             * épsilon | épsilon = épsilon */
            if( removeEpsilon( iterator.rightChild() ) ) {
                iterator.destroyRightSubtree();
                *iterator = Operator::Optional;
                return removeEpsilon( iterator );
            }
            if( removeEpsilon( iterator.leftChild() ) ) {
                iterator.destroyLeftSubtree();
                *iterator = Operator::Optional;
                iterator.setLeftChild( iterator.rightChild() );
                iterator.setRightChild( nullptr );
            }
            return false;

        case Operator::SigmaClosure:
            /* x       : y       = x : y (se mantém)
             * x       : épsilon = x+
             * épsilon : y       = y*
             * épsilon : épsilon = épsilon */
            if( removeEpsilon( iterator.rightChild() ) ) {
                iterator.destroyRightSubtree();
                *iterator = Operator::PositiveClosure;
                return removeEpsilon( iterator );
            }
            if( removeEpsilon( iterator.leftChild() ) ) {
                iterator.destroyLeftSubtree();
                *iterator = Operator::KleneeClosure;
                iterator.setLeftChild( iterator.rightChild() );
                iterator.setRightChild( nullptr );
            }
            return false;
    }
    throw token_error( "Extraneous operator found" );
}

template< typename TreeIterator >
std::pair< std::set< TreeIterator >,
           std::map< TreeIterator, std::set< TreeIterator > >
         >
buildComposition( TreeIterator root ) {
    using std::set;
    typedef typename extract_head_type< 
            typename unqualified< decltype(*root) >::type
        >::type Char;

    /* nodeList contém a lista de todos os nós da árvore acessíveis
     * a partir do nodo raíz, e leafList contém apenas os nós folha. */
    set< TreeIterator > nodeList;
    set< TreeIterator > leafList;
    std::function< void( TreeIterator ) > buildNodeList = 
        [&]( TreeIterator iterator ) {
            nodeList.insert( iterator );
            if( iterator->template is<Operator>() )
                switch( iterator->template operator Operator() ) {
                    case Operator::Concatenation:
                    case Operator::VerticalBar:
                        buildNodeList( iterator.rightChild() );
                        // fall-through intencional
                    case Operator::KleneeClosure:
                    case Operator::PositiveClosure:
                    case Operator::Optional:
                        buildNodeList( iterator.leftChild() );
                        break;
                    default:
                        throw token_error( "Unsupported operator" );
                }
            else if( iterator->template is<Char>() )
                leafList.insert( iterator );
        };

    /* A navegação pela árvore será feita recursivamente e pode passar
     * pelo mesmo nodo várias vezes. Para evitar recursão infinita,
     * marcaremos neste mapa quais nodos já foram visitados.
     *
     * São feitas duas operações: aprofundar e avançar (ou descer e subir).
     * Cada operação só pode ocorrer uma vez em cada elemento, mas podem
     * ocorrer ambas as operações; portanto, precisamos de dois mapeamentos.
     *
     * A função resetStatus (definida adiante) zera estes mapeamentos. */
    std::map< TreeIterator, bool > deepened;
    std::map< TreeIterator, bool > advanced;

    auto resetStatus = [&]() {
        for( TreeIterator iterator : nodeList )
            deepened[iterator] = advanced[iterator] = false;
        // Note que o mapeamento é construído na primeira execução.
    };

    /* Durante a navegação pela árvore, este conjunto armazenará
     * a lista de iteradores que pertence à composição do nó analisado
     * no momento. */
    set< TreeIterator > current;

    std::function< void( TreeIterator ) > deepen; // "rotinas descer"
    std::function< void( TreeIterator ) > advance; // "rotinas subir"
    /* Estas duas funções mutuamente recursivas farão o trabalho pesado.
     *
     * deepen irá aprofundar a busca na árvore, buscando os nós que
     * participarão das próximas transições. 
     * advance tentará alcançar o próximo nodo, após já ter transitado
     * pelos nodos da sub-árvore atual.
     * Por exemplo:
     *          .                     .
     *         / \                   /4\
     *        |   b   Numerando:    |   b
     *       / \                   /2\  5
     *      a   c                 a   c
     *                            1   3
     * Quando esta expressão regular for reconhecida a partir do topo
     * (nó 4), a próxima transição têm de ser por 'a', no nodo 1, ou
     * por 'b', no nodo 3. O objetivo de deepen(4) é montar o conjunto
     * dos nós que possivelmente conterão a primeira transição do autômato
     * (o conjunto {1, 3} neste caso).
     *
     * Executar deepen no nodo 4 irá redirecionar a execução para o nodo 2,
     * tentando aprofundar. Por sua vez, deepen(2) se dividirá pelos nodos
     * 1 e 3, também aprofundando. Finalmente, deepen(1) adiciona o nodo
     * 1 ao conjunto "current", e deepen(3) adiciona o 3.
     *
     * advance é executado após a transição, para encontrar os "sucessores".
     * advance(1) redireciona para advance(2), que redireciona para 
     * advance(4). Como o nó 4 é uma concatenação, prosseguir para o próximo
     * nodo significa procurar as transições da subárvore da direita,
     * portanto, advance(4) redireciona para deepen(5).
     * 
     * deepen também pode invocar advance:
     *      .
     *     /3\
     *    *   b
     *   /2   4
     *  a
     *  1
     * deepen(3) redireciona para deepen(2), que se divide em deepen(1)
     * e advance(3) (pois o fechamento pode ser escolhido como épsilon,
     * avançando para a próxima subárvore).
     *
     * Detalhes sobre o comportamento em cada operador é encontrado abaixo.
     */
    deepen = [&]( TreeIterator iterator ) {
        if( deepened[iterator] ) return;
        deepened[iterator] = true;
        if( !iterator || iterator->template is<Char>() ) {
            current.insert( iterator );
            return;
        }

        switch( iterator->template operator Operator() ) {
            case Operator::KleneeClosure:
                /*       * ->
                 *     /
                 * Podemos tanto descer na árvore 
                 * quanto ir para o próximo nodo. 
                 *
                 * Como a árvore está costurada, o nó pai (que deve
                 * ser o alvo do próximo advance) está, justamente,
                 * no filho da direita deste nodo. */
                deepen( iterator.leftChild() );
                advance( iterator.rightChild() );
                return;

            case Operator::PositiveClosure:
                /*       +
                 *     /
                 * Aqui somos obrigados a descer na árvore, pois o fecho
                 * positivo reconhece ao menos uma vez a subárvore. */
                deepen( iterator.leftChild() );
                return;

            case Operator::Optional:
                /*       ? ->
                 *     /
                 */
                deepen( iterator.leftChild() );
                advance( iterator.rightChild() );
                return;

            case Operator::Concatenation:
                /*      .
                 *    / 
                 */
                deepen( iterator.leftChild() );
                return;

            case Operator::VerticalBar:
                /*      |
                 *    /   \
                 */
                deepen( iterator.leftChild() );
                deepen( iterator.rightChild() );
                return;

            default:
                /* Apenas para silenciar o warning.
                 * Qualquer operador não suportado será detectado
                 * em buildNodeList. */;
        }
    }; // fim deepen
    advance = [&]( TreeIterator iterator ) {
        if( advanced[iterator] ) return;
        advanced[iterator] = true;
        if( !iterator ) {
            // Chegamos ao fim da árvore.
            current.insert( nullptr );
            return;
        }
        if( iterator->template is<Char>() ) {
            advance( iterator.rightChild() );
            return;
        }

        switch( iterator->template operator Operator() ) {
            case Operator::KleneeClosure:
                /*       * ->
                 *     /
                 * Novamente podemos descer na árvore (voltando)
                 * quanto ir para o próximo nodo (avançando). */
                deepen( iterator.leftChild() );
                advance( iterator.rightChild() );
                return;

            case Operator::PositiveClosure:
                /*       + ->
                 *     /
                 * Já passamos uma vez por aqui, então podemos nos comportar
                 * como Operator::KleneeClosure. */
                deepen( iterator.leftChild() );
                advance( iterator.rightChild() );
                return;

            case Operator::Optional:
                /*       ? ->
                 */
                advance( iterator.rightChild() );
                return;

            case Operator::Concatenation:
                /*      .
                 *        \
                 */
                deepen( iterator.rightChild() );
                return;

            case Operator::VerticalBar:
                /*      | ->
                 * Para este operador precisaremos fazer um pouco de
                 * malabarismo: desceremos pela subárvore à direita até
                 * achar algum nó costurado e "subiremos" pela costura.
                 */
                do
                    iterator = iterator.rightChild();
                while( *iterator == Operator::Concatenation ||
                       *iterator == Operator::VerticalBar );
                advance( iterator.rightChild() );
                return;

            default:
                ;
        }
    }; // fim advance

    std::map< TreeIterator, set< TreeIterator > > map;
    buildNodeList( root );
    nodeList.insert( nullptr );
    resetStatus();
    for( TreeIterator iterator : leafList ) {
        advance( iterator );
        map[iterator] = current;
        current.clear();
        resetStatus();
    }
    deepen( root ); // Obter a composição inicial
    return {current, map};
}
#endif // DE_SIMONE_H
