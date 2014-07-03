/* acceptanceList.h
 * Lista as palavras de determinado tamanho aceitas por dado autômato.
 */
#ifndef ACCEPTANCE_LIST_H
#define ACCEPTANCE_LIST_H

#include <vector>
#include "algorithm/tuple_iterator.h"
#include "automaton/deterministic.h"

template< typename State, typename Symbol >
std::vector< std::vector<Symbol> > acceptanceList(
        DFA<State, Symbol> dfa, std::size_t n )
{
    std::vector< std::vector<Symbol> > words;

    for( const std::vector<Symbol>& word : tuple_range( dfa.alphabet, n ) )
        if( dfa.accepts( word.begin(), word.end() ) )
            words.push_back( word );

    return words;
}

#endif
