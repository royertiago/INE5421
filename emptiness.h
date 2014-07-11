/* emptiness.h
 * Função que testa vacuidade de linguagens de gramáticas livres de contexto.
 */
#ifndef EMPTINESS_H
#define EMPTINESS_H

#include <map>
#include "grammar/grammar.h"
#include "grammar/manipulations.h"

template< typename NonTerminal, typename Terminal >
bool isEmpty( const Grammar<NonTerminal, Terminal> & g ) {
    return removeDead( g ).productionsFrom( g.startSymbol ).empty();
}

#endif // EMPTINESS_H
