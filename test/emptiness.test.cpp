/* emptiness.test.cpp
 * Teste de unidade para a função que testa vacuidade em
 * linguagens livres de contexto. */

#include "emptiness.h"
#include "test/lib/test.h"

DECLARE_TEST( EmptinessTest ) {
    bool b = true;
    Grammar< char > g1 = { /* Vt = */ {'a', 'b'},
                           /* Vn = */ {'S', 'A'},
                           /* P = */  { {'S', {'a', 'A', 'S'} },
                                        {'S', {'a'} },
                                        {'A', {'S', 'b', 'a'} },
                                        {'A', {'b', 'a'} },
                                        {'A', {'S', 'S'} }
                                       },
                           /* S = */ 'S' };
    b &= Test::TEST_EQUALS( isEmpty( g1 ), false );

    Grammar< char > g2 = { /* Vt = */ {'a'},
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

    return b;
}
