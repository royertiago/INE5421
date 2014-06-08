/* production.h
 * Estrutura que representa uma produção de uma gramática livre
 * de contexto.
 *
 * Terminais e não-terminais precisam ter o mesmo tipo; para
 * determinar se um símbolo é terminal ou não, deve-se consultar
 * a gramática.
 */
#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <vector>

template< typename T >
struct Production {
    T left;
    std::vector< T > right;
};

#endif // PRODUCTION_H
