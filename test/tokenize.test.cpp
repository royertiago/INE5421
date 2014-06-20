/* tokenize.test.cpp
 * Teste para as funções tokenize() e explicitConcatenations(),
 * de regex/parsing.h.
 */
#include "regex/parsing.h"

#include <string>
#include "test/lib/test.h"
#include "print.h"

DECLARE_TEST( TokenizeTest ) {
    bool b = true;

    std::string s = "alb(\\(.*\\.))";
    TokenVector<char> w = {'a', Operator::SigmaClosure, 'b',
        Parentheses::Left, '(', Operator::KleneeClosure, '.',
        Parentheses::Right, Parentheses::Right };
    TokenVector<char> t = {'a', Operator::SigmaClosure, 'b',
        Operator::Concatenation, Parentheses::Left, '(', 
        Operator::KleneeClosure, Operator::Concatenation, '.', 
        Parentheses::Right, Parentheses::Right };
    TokenVector<char> v = tokenize( s.begin(), s.end() );
    TokenVector<char> k = explicitConcatenations( v );

    b &= Test::TEST_EQUALS( (int) v.size(), (int) w.size() );
    b &= Test::TEST_EQUALS( (int) k.size(), (int) t.size() );
    for( auto it = w.begin(), jt = v.begin(); it != w.end(); ++it, ++jt )
        b &= Test::TEST_EQUALS( *it == *jt, true );
    for( auto it = t.begin(), jt = k.begin(); it != t.end(); ++it, ++jt )
        b &= Test::TEST_EQUALS( *it == *jt, true );

    s = "a (a+|b?&";
    w = {'a', ' ', Parentheses::Left, 'a', Operator::PositiveClosure, 
        Operator::VerticalBar, 'b', Operator::Optional, epsilon };
    t = {'a', Operator::Concatenation, ' ', Operator::Concatenation,
         Parentheses::Left, 'a', Operator::PositiveClosure, 
         Operator::VerticalBar, 'b', Operator::Optional, 
         Operator::Concatenation, epsilon };
    v = tokenize( s.begin(), s.end() );
    k = explicitConcatenations( v );

    b &= Test::TEST_EQUALS( (int) v.size(), (int) w.size() );
    b &= Test::TEST_EQUALS( (int) k.size(), (int) t.size() );
    for( auto it = w.begin(), jt = v.begin(); it != w.end(); ++it, ++jt )
        b &= Test::TEST_EQUALS( *it == *jt, true );
    for( auto it = t.begin(), jt = k.begin(); it != t.end(); ++it, ++jt )
        b &= Test::TEST_EQUALS( *it == *jt, true );

    return b;
}
