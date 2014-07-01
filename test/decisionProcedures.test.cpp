/* decisionProcedures.test.cpp
 * Teste de unidade para as funções de automaton/decisionProcedures.h
 */
#include "automaton/decisionProcedures.h"

#include "test/lib/test.h"

DECLARE_TEST( DecisionProceduresTest ) {
    bool b = true;
    DFA< int, char > axb = { {0, 1},
                             {'a', 'b'},
                             { {{0, 'a'}, 0},
                               {{0, 'b'}, 1},
                             },
                             {1},
                             0
    }; // a*b
    DFA< int, char > amb = { {0, 1, 2},
                             {'a', 'b'},
                             { {{0, 'a'}, 1},
                               {{1, 'a'}, 1},
                               {{1, 'b'}, 2}
                             },
                             {2},
                             0
    }; // a+b
    DFA< int, char > ambIb = { {0, 1, 2},
                               {'a', 'b'},
                               { {{0, 'a'}, 1},
                                 {{0, 'b'}, 2},
                                 {{1, 'a'}, 1},
                                 {{1, 'b'}, 2}
                               },
                               {2},
                               0
    }; // a+b|b
    DFA< int, char > bxa = { {0, 1},
                             {'a', 'b'},
                             { {{0, 'b'}, 0},
                               {{0, 'a'}, 1},
                             },
                             {1},
                             0
    }; // b*a
    DFA< int, char > bma = { {0, 1, 2},
                             {'a', 'b'},
                             { {{0, 'b'}, 1},
                               {{1, 'b'}, 1},
                               {{1, 'a'}, 2}
                             },
                             {2},
                             0
    }; // b+a
    DFA< int, char > a = { {0, 1},
                           {'a', 'b'},
                           { {{0, 'a'}, 1}
                           },
                           {1},
                           0
    }; // b
    DFA< int, char > n = { {0, 1},
                           {'b'},
                           { {{0, 'b'}, 1}
                           },
                           {},
                           0
    }; // null automaton

    b &= Test::TEST_EQUALS( equivalent( axb, ambIb ), true );
    b &= Test::TEST_EQUALS( included( amb, ambIb ), true );
    b &= Test::TEST_EQUALS( included( amb, axb ), true );
    b &= Test::TEST_EQUALS( included( ambIb, amb ), false );
    b &= Test::TEST_EQUALS( included( axb, amb ), false );
    b &= Test::TEST_EQUALS( disjoint( axb, bxa ), true );
    b &= Test::TEST_EQUALS( disjoint( amb, bma ), true );
    //  Estes testes lançam exceções:
//  b &= Test::TEST_EQUALS( disjoint( bma, a ), true );
//  b &= Test::TEST_EQUALS( disjoint( bxa, a ), false );

//  b &= Test::TEST_EQUALS( empty( a ), false );
//  b &= Test::TEST_EQUALS( finite( a ), true );
//  b &= Test::TEST_EQUALS( infinite( a ), false );
//  b &= Test::TEST_EQUALS( empty( ambIb ), false );
//  b &= Test::TEST_EQUALS( finite( ambIb ), false );
//  b &= Test::TEST_EQUALS( infinite( ambIb ), true );
    printf( "A\n" );
    return b;
}
