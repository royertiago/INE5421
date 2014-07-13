/* grammarDecisionProcedures.test.cpp
 * Teste de unidade para as funções de grammar/decisionProcedures.h.
 */

#include "print.h"
#include "grammar/decisionProcedures.h"
#include "test/lib/test.h"

DECLARE_TEST( GrammarDecisionProceduresTest ) {
    bool b = true;
    Grammar< char, char > g1 = { /* Vn = */ {'S', 'A'},
                                 /* Vt = */ {'a', 'b'},
                                 /* P = */  { {'S', {'a', 'A', 'S'} },
                                              {'S', {'a'} },
                                              {'A', {'S', 'b', 'a'} },
                                              {'A', {'b', 'a'} },
                                              {'A', {'S', 'S'} }
                                             },
                                 /* S = */ 'S' };
    b &= Test::TEST_EQUALS( empty( g1 ), false );
    b &= Test::TEST_EQUALS( finite( g1 ), false );
    b &= Test::TEST_EQUALS( infinite( g1 ), true );

    Grammar< char, char > g2 = { /* Vn = */ {'S', 'A', 'B'},
                                 /* Vt = */ {'a'},
                                 /* P = */  { {'S', {'a', 'A', 'S'} },
                                              {'S', {'a'} },
                                              {'A', {'S', 'B', 'a'} },
                                              {'A', {'B', 'a'} },
                                              {'A', {'S', 'S'} },
                                              {'B', {'S', 'a', 'B'} },
                                              {'B', {'A', 'a', 'B'} }
                                       },
                                 /* S = */ 'S' };
    b &= Test::TEST_EQUALS( empty( g2 ), false );
    b &= Test::TEST_EQUALS( finite( g2 ), false );
    b &= Test::TEST_EQUALS( infinite( g2 ), true );

    g2.startSymbol = 'B';
    b &= Test::TEST_EQUALS( empty( g2 ), true );
    b &= Test::TEST_EQUALS( finite( g2 ), true );
    b &= Test::TEST_EQUALS( infinite( g2 ), false );

    g2.startSymbol = 'A';
    b &= Test::TEST_EQUALS( empty( g2 ), false );
    b &= Test::TEST_EQUALS( finite( g2 ), false );
    b &= Test::TEST_EQUALS( infinite( g2 ), true );

    g2.productions.erase( {'A', {'S', 'S'}} );
    b &= Test::TEST_EQUALS( empty( g2 ), true );
    b &= Test::TEST_EQUALS( finite( g2 ), true );
    b &= Test::TEST_EQUALS( infinite( g2 ), false );

    // Gramáticas com tipos heterogêneos

    Grammar< int, char > g3 = { /* Vn = */ {0, 1},
                                /* Vt = */ {'a', 'b'},
                                /* P = */  { {0, {'a', 1, 0} },
                                             {0, {'a'} },
                                             {1, {0, 'b', 'a'} },
                                             {1, {'b', 'a'} },
                                             {1, {0, 0} }
                                            },
                                /* S = */ 0 };
    b &= Test::TEST_EQUALS( empty( g3 ), false );
    b &= Test::TEST_EQUALS( finite( g3 ), false );
    b &= Test::TEST_EQUALS( infinite( g3 ), true );

    Grammar< int, char > g4 = { /* Vn = */ {0, 1, 2},
                                /* Vt = */ {'a'},
                                /* P = */  { {0, {'a', 1, 0} },
                                             {0, {'a'} },
                                             {1, {0, 2, 'a'} },
                                             {1, {2, 'a'} },
                                             {1, {0, 0} },
                                             {2, {0, 'a', 2} },
                                             {2, {1, 'a', 2} }
                                            },
                                /* S = */ 0 };
    b &= Test::TEST_EQUALS( empty( g4 ), false );
    b &= Test::TEST_EQUALS( finite( g4 ), false );
    b &= Test::TEST_EQUALS( infinite( g4 ), true );

    g4.startSymbol = 2;
    b &= Test::TEST_EQUALS( empty( g4 ), true );
    b &= Test::TEST_EQUALS( finite( g4 ), true );
    b &= Test::TEST_EQUALS( infinite( g4 ), false );
      
    g4.startSymbol = 1;
    b &= Test::TEST_EQUALS( empty( g4 ), false );
    b &= Test::TEST_EQUALS( finite( g4 ), false );
    b &= Test::TEST_EQUALS( infinite( g4 ), true );
      
    g4.productions.erase( {1, {0, 0}} );
    b &= Test::TEST_EQUALS( empty( g4 ), true );
    b &= Test::TEST_EQUALS( finite( g4 ), true );
    b &= Test::TEST_EQUALS( infinite( g4 ), false );

    Grammar< int, char > g5 = { /* Vn */ {0, 1},
                                /* Vt */ {'a'},
                                /* P  */ { {0, {'a'} },
                                           {1, {'a'} },
                                         },
                                /* S  */ 0 };

    b &= Test::TEST_EQUALS( empty( g5 ), false );
    b &= Test::TEST_EQUALS( finite( g5 ), true );
    b &= Test::TEST_EQUALS( infinite( g5 ), false );
    
    g5.erase( 0 );
    b &= Test::TEST_EQUALS( empty( g5 ), true );
    b &= Test::TEST_EQUALS( finite( g5 ), true );
    b &= Test::TEST_EQUALS( infinite( g5 ), false );

    /* Não-terminal 0 deriva 1 e 2, 1 e 2 derivam 3.
     * O problema está no 3. */
    Grammar< int, char > g6 = { /* Vn */ {0, 1, 2, 3},
                                /* Vt */ {'a'},
                                /* P  */ { {0, {'a'} },
                                           {0, {1} },
                                           {0, {1, 1} },
                                           {0, {1, 2} },
                                           {0, {2, 2} },
                                           {0, {2, 1} },
                                           {0, {2} },
                                           {1, {'a'} },
                                           {1, {3, 'a'} },
                                           {1, {'a', 3} },
                                           {1, {3} },
                                           {2, {'a'} },
                                           {2, {3, 'a'} },
                                           {2, {'a', 3} },
                                           {2, {3} },
                                           {3, {'a'} },
                                           {3, {'a', 'a'} },
                                           {3, {'a', 3} },
                                         },
                                /* S  */ 0 };

    b &= Test::TEST_EQUALS( empty( g6 ), false );
    b &= Test::TEST_EQUALS( finite( g6 ), false );
    b &= Test::TEST_EQUALS( infinite( g6 ), true );

    g6.productions.erase( {3, {'a', 3} } );
    b &= Test::TEST_EQUALS( empty( g6 ), false );
    b &= Test::TEST_EQUALS( finite( g6 ), true );
    b &= Test::TEST_EQUALS( infinite( g6 ), false );
    return b;
}
