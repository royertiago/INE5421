/* parseGrammar.cpp
 * Implementação de parseGrammar.h
 */
#include <cctype>

#include "parseGrammar.h"
#include "utility/either.h"
#include "grammar/grammar.h"

using std::vector;
using std::string;
using std::isspace;

namespace {

enum class Tokens {
    Or, // Barra vertical
    Replacement // Substituição, "->"
};

/* Converte a string passada numa lista de elementos, que podem ser
 * strings ou tokens.
 *
 * A quebra é feita de acordo com a descrição em parseGrammar.h. */
vector< Either<string, Tokens> > tokenize( string str ) {
    vector< Either<string, Tokens> > vec;
    string current;
    bool building = false; // não estamos construindo string alguma

    auto push = [&](){
        if( current == "|" )
            vec.push_back( Tokens::Or );
        else if( current == "->" )
            vec.push_back( Tokens::Replacement );
        else
            vec.push_back( current );
        current.clear();
        building = false;
    };

    for( char c : str ) {
        if( isspace( c ) ) {
            if( building )
                push();
        }
        else if( c == '|' ) {
            if( building )
                push();
            vec.push_back( Tokens::Or );
        }
        else {
            building = true;
            current.append( 1, c );
        }
    }
    if( building ) push();

    return vec;
}

} // anonymous namespace

Grammar< string, string > parseGrammar( vector<string> vec ) {
    if( vec.empty() ) 
        throw std::invalid_argument( "Vector must not be empty" );

    Grammar< string, string > g;
    
    auto markTerminal = [&]( string s ) {
        if( g.nonTerminals.count( s ) == 0 )
            g.terminals.insert( s );
    };
    auto markNonTerminal = [&]( string s ) {
        g.terminals.erase( s );
        g.nonTerminals.insert( s );
    };

    // Pegar o símbolo inicial
    {
        auto tokens = tokenize( *vec.begin() );
        if( tokens.empty() )
            throw std::invalid_argument( "First string must not be blank" );
        auto start = *tokens.begin();
        if( !start.is<string>() )
            throw invalid_left_hand_side( 
                    "Left-hand side must not be empty", 0 );

        g.startSymbol = start;
    }

    // Analisar cada string
    for( unsigned i = 0; i < vec.size(); ++i ) {
        auto tokens = tokenize( vec[i] );
        if( tokens.empty() ) continue;

        Production< string, string > p;

        auto it = tokens.begin();

        { // Pegar o lado esquerdo da produção e o "->"
            auto token = *it++;

            if( !token.is<string>() )
                throw invalid_left_hand_side( 
                        "Left-hand side must not be empty", i );
            p.left = token;

            token = *it;
            if( token != Tokens::Replacement ) {
                if( token.is<string>() )
                    throw invalid_left_hand_side(
                            "Left-hand side must have exactly "
                            "one non-terminal", i );
                else
                    throw invalid_replacement_symbol(
                            "Expected \"->\"", i );
            }

            markNonTerminal( p.left );
        }

        // Analisar o resto da produção
        while( ++it != tokens.end() ) {
            if( *it == Tokens::Or ) {
                g.productions.insert( p );
                p.right.clear();
            }
            else if( *it == Tokens::Replacement )
                throw invalid_replacement_symbol(
                        "Right-hand side must have at most "
                        "one replacement symbol", i );
            else {
                markTerminal( *it );
                p.right.push_back( it->getAs<string>() );
            }
        } // fim while interno
        g.productions.insert( p );

    } // fim for externo

    return g;
}
