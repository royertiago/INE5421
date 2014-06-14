/* production.h
 * Estrutura que representa uma produção de uma gramática livre
 * de contexto.
 *
 * Terminais e não-terminais podem ter o mesmo tipo;
 * para determinar se um símbolo é terminal ou não, 
 * deve-se consultar a gramática.
 *
 * Para implementação eficiente de conjuntos de produções,
 * operator< está definido para Production. A comparação
 * é semelhante à de std::pair, mas produções com lados direitos
 * de tamanho menor terão "prioridade".
 */
#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <vector>
#include "utility/either.h"

template< typename NonTerminal, typename Terminal >
struct Production {
    NonTerminal left;
    std::vector< Either<NonTerminal, Terminal> > right;
};

// Implementação do operador
template< typename NonTerminal, typename Terminal >
bool operator<( const Production<NonTerminal, Terminal>& rhs,
                const Production<NonTerminal, Terminal>& lhs )
{
    if( rhs.left < lhs.left )
        return true;
    if( lhs.left < rhs.left )
        return false;
    if( rhs.right.size() < lhs.right.size() )
        return true;
    if( lhs.right.size() < rhs.right.size() )
        return false;
    for( auto it = rhs.right.begin(),
              jt = lhs.right.begin();
         it != rhs.right.end();
         ++it, ++jt )
    {
        if( *it < *jt )
            return true;
        if( *jt < *it )
            return false;
    }
    return false;
}
#endif // PRODUCTION_H
