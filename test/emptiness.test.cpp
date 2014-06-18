/* emptiness.test.cpp
 * Teste de unidade para a função que testa vacuidade em
 * linguagens livres de contexto. */

#include "emptiness.h"
#include "test/lib/test.h"

DECLARE_TEST( EmptinessTest ) {
    bool b = true;
//  Grammar< char, char > g1 = { /* Vn = */ {'S', 'A'},
//                               /* Vt = */ {'a', 'b'},
//                               /* P = */  { {'S', {'a', 'A', 'S'} },
//                                            {'S', {'a'} },
//                                            {'A', {'S', 'b', 'a'} },
//                                            {'A', {'b', 'a'} },
//                                            {'A', {'S', 'S'} }
//                                           },
//                               /* S = */ 'S' };
//  b &= Test::TEST_EQUALS( isEmpty( g1 ), false );

//  Grammar< char, char > g2 = { /* Vn = */ {'S', 'A', 'B'},
//                               /* Vt = */ {'a'},
//                               /* P = */  { {'S', {'a', 'A', 'S'} },
//                                            {'S', {'a'} },
//                                            {'A', {'S', 'B', 'a'} },
//                                            {'A', {'B', 'a'} },
//                                            {'A', {'S', 'S'} },
//                                            {'B', {'S', 'a', 'B'} },
//                                            {'B', {'A', 'a', 'B'} }
//                                     },
//                               /* S = */ 'S' };
//  b &= Test::TEST_EQUALS( isEmpty( g2 ), false );

//  g2.startSymbol = 'B';
//  b &= Test::TEST_EQUALS( isEmpty( g2 ), true );

//  g2.startSymbol = 'A';
//  b &= Test::TEST_EQUALS( isEmpty( g2 ), false );

//  g2.productions.erase( {'A', {'S', 'S'}} );
//  b &= Test::TEST_EQUALS( isEmpty( g2 ), true );

//  // Gramáticas com tipos heterogêneos

//  Grammar< int, char > g3 = { /* Vn = */ {0, 1},
//                              /* Vt = */ {'a', 'b'},
//                              /* P = */  { {0, {'a', 1, 0} },
//                                           {0, {'a'} },
//                                           {1, {0, 'b', 'a'} },
//                                           {1, {'b', 'a'} },
//                                           {1, {0, 0} }
//                                          },
//                              /* S = */ 0 };
//  b &= Test::TEST_EQUALS( isEmpty( g3 ), false );

//  Grammar< int, char > g4 = { /* Vn = */ {0, 1, 2},
//                              /* Vt = */ {'a'},
//                              /* P = */  { {0, {'a', 1, 0} },
//                                           {0, {'a'} },
//                                           {1, {0, 2, 'a'} },
//                                           {1, {2, 'a'} },
//                                           {1, {0, 0} },
//                                           {2, {0, 'a', 2} },
//                                           {2, {1, 'a', 2} }
//                                    },
//                              /* S = */ 0 };
//  b &= Test::TEST_EQUALS( isEmpty( g4 ), false );

//  g4.startSymbol = 2;
//  b &= Test::TEST_EQUALS( isEmpty( g4 ), true );
//    
//  g4.startSymbol = 1;
//  b &= Test::TEST_EQUALS( isEmpty( g4 ), false );
//    
//  g4.productions.erase( {1, {0, 0}} );
//  b &= Test::TEST_EQUALS( isEmpty( g4 ), true );

    return b;
}
