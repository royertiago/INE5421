/* tokenize.test.cpp
 * Teste para a função tokenize(), de regex/parsing.h
 */
#include "regex/parsing.h"

#include <string>
#include "test/lib/test.h"

DECLARE_TEST( TokenizeTest ) {
    bool b = true;

    std::string s = "alb(\\(.*\\.))";
    TokenVector<char> w = {'a', Operator::SigmaClosure, 'b',
        Parentheses::Left, '(', Operator::KleneeClosure, '.',
        Parentheses::Right, Parentheses::Right };
    TokenVector<char> v = tokenize( s.begin(), s.end() );

    for( auto it = w.begin(), jt = v.begin(); it != w.end(); ++it, ++jt )
        b &= Test::TEST_EQUALS( *it == *jt, true );

    s = "a a+|b?&";
    w = {'a', ' ', 'a', Operator::PositiveClosure, 
        Operator::VerticalBar, 'b', Operator::Optional, epsilon };
    v = tokenize( s.begin(), s.end() );

    for( auto it = w.begin(), jt = v.begin(); it != w.end(); ++it, ++jt )
        b &= Test::TEST_EQUALS( *it == *jt, true );

    return b;
}
