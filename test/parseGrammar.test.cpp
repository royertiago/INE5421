/* parseGrammar.test.cpp
 * Teste de unidade para a função parseGrammar, de ui/parseGrammar.h
 */
#include "ui/parseGrammar.h"
#include "test/lib/test.h"

DECLARE_TEST( ParseGrammarTest ) {
    // Este é um teste de exceptions, na verdade
    bool b = true;
    EXPECT_THROW( parseGrammar({}), std::invalid_argument, b );
    EXPECT_THROW( parseGrammar({""}), std::invalid_argument, b );
    EXPECT_THROW( parseGrammar({"", "A -> a"}), std::invalid_argument, b );

    EXPECT_THROW( parseGrammar({"A a -> b"}),
            invalid_left_hand_side, b );
    EXPECT_THROW( parseGrammar({"A a | b"}),
            invalid_left_hand_side, b );
    EXPECT_THROW( parseGrammar({"-> b"}),
            invalid_left_hand_side, b );
    EXPECT_THROW( parseGrammar({"S -> a", "A a -> b"}),
            invalid_left_hand_side, b );
    EXPECT_THROW( parseGrammar({"S -> a", "A a | b"}),
            invalid_left_hand_side, b );
    EXPECT_THROW( parseGrammar({"S -> a", "-> b"}),
            invalid_left_hand_side, b );

    EXPECT_THROW( parseGrammar({"A -> a -> b"}),
            invalid_replacement_symbol, b );
    EXPECT_THROW( parseGrammar({"A | b"}),
            invalid_replacement_symbol, b );
    EXPECT_THROW( parseGrammar({"S -> a", "A -> a -> b"}),
            invalid_replacement_symbol, b );
    EXPECT_THROW( parseGrammar({"S -> a", "A | b"}),
            invalid_replacement_symbol, b );
    return b;
}
