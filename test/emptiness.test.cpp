/* emptiness.test.cpp
 * Teste de unidade para a função que testa vacuidade em
 * linguagens livres de contexto. */

#include "emptiness.h"
#include "test/lib/test.h"

DECLARE_TEST( EmptinessTest ) {
    bool b = true;
    Grammar< char, char > g1 = { /* Vt = */ {'a', 'b'},
                                 /* Vn = */ {'S', 'A'},
                                 /* P = */  { {'S', {'a', 'A', 'S'} },
                                              {'S', {'a'} },
                                              {'A', {'S', 'b', 'a'} },
                                              {'A', {'b', 'a'} },
                                              {'A', {'S', 'S'} }
                                             },
                                 /* S = */ 'S' };
    b &= Test::TEST_EQUALS( isEmpty( g1 ), false );

    Grammar< char, char > g2 = { /* Vt = */ {'a'},
                                 /* Vn = */ {'S', 'A', 'B'},
                                 /* P = */  { {'S', {'a', 'A', 'S'} },
                                              {'S', {'a'} },
                                              {'A', {'S', 'B', 'a'} },
                                              {'A', {'B', 'a'} },
                                    /*P[4]*/  {'A', {'S', 'S'} },
                                              {'B', {'S', 'a', 'B'} },
                                              {'B', {'A', 'a', 'B'} }
                                       },
                                 /* S = */ 'S' };
    b &= Test::TEST_EQUALS( isEmpty( g2 ), false );

    g2.startSymbol = 'B';
    b &= Test::TEST_EQUALS( isEmpty( g2 ), true );

    g2.startSymbol = 'A';
    b &= Test::TEST_EQUALS( isEmpty( g2 ), false );

    g2.productions[4].right[1] = 'B';
    b &= Test::TEST_EQUALS( isEmpty( g2 ), true );

    // Gramáticas com tipos heterogêneos

    Grammar< char, int > g3 = { /* Vt = */ {'a', 'b'},
                                /* Vn = */ {0, 1},
                                /* P = */  { {0, {'a', 1, 0} },
                                             {0, {'a'} },
                                             {1, {0, 'b', 'a'} },
                                             {1, {'b', 'a'} },
                                             {1, {0, 0} }
                                            },
                                /* S = */ 0 };
    b &= Test::TEST_EQUALS( isEmpty( g3 ), false );

    Grammar< char, int > g4 = { /* Vt = */ {'a'},
                                /* Vn = */ {0, 1, 2},
                                /* P = */  { {0, {'a', 1, 0} },
                                             {0, {'a'} },
                                             {1, {0, 2, 'a'} },
                                             {1, {2, 'a'} },
                                   /*P[4]*/  {1, {0, 0} },
                                             {2, {0, 'a', 2} },
                                             {2, {1, 'a', 2} }
                                      },
                                /* S = */ 0 };
    b &= Test::TEST_EQUALS( isEmpty( g4 ), false );

    g4.startSymbol = 2;
    b &= Test::TEST_EQUALS( isEmpty( g4 ), true );
      
    g4.startSymbol = 1;
    b &= Test::TEST_EQUALS( isEmpty( g4 ), false );
      
    g4.productions[4].right[1] = 2;
    b &= Test::TEST_EQUALS( isEmpty( g4 ), true );

    return b;
}
