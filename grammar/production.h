/* production.h
 * Estrutura que representa uma produção de uma gramática livre
 * de contexto.
 *
 * Terminais e não-terminais podem ter o mesmo tipo;
 * para determinar se um símbolo é terminal ou não, 
 * deve-se consultar a gramática.
 */
#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <vector>
#include "utility/either.h"

template< typename Terminal, typename NonTerminal >
struct Production {
    NonTerminal left;
    std::vector< Either<Terminal, NonTerminal> > right;
};

#endif // PRODUCTION_H
