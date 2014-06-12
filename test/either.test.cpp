/* either.test.cpp
 * Teste de unidade para a classe Either.
 */
#include "utility/either.h"

#include <vector>
#include <algorithm>
#include "test/lib/test.h"

DECLARE_TEST( EitherTest ) {
    bool b = true;

    // Construtor padrão
    Either< int, char > e;
    b &= Test::TEST_EQUALS( e.isFirst(), true );
    b &= Test::TEST_EQUALS( e.isSecond(), false );
    b &= Test::TEST_EQUALS( e.first(), 0 );
    EXPECT_THROW( e.second(), std::bad_cast, b );

    // Construtor de atribuição
    Either< int, char > p = 'A';
    b &= Test::TEST_EQUALS( p.isFirst(), false );
    b &= Test::TEST_EQUALS( p.isSecond(), true );
    EXPECT_THROW( p.first(), std::bad_cast, b );
    b &= Test::TEST_EQUALS( p.second(), 'A' );

    Either< int, char > q = (int) 'A';
    b &= Test::TEST_EQUALS( q.isFirst(), true );
    b &= Test::TEST_EQUALS( q.isSecond(), false );
    b &= Test::TEST_EQUALS( q.first(), (int) 'A' );
    EXPECT_THROW( q.second(), std::bad_cast, b );

    // Atribuição
    e = 7;
    b &= Test::TEST_EQUALS( e.isFirst(), true );
    b &= Test::TEST_EQUALS( e.isSecond(), false );
    b &= Test::TEST_EQUALS( e.first(), 7 );
    EXPECT_THROW( e.second(), std::bad_cast, b );

    int i = e;
    b &= Test::TEST_EQUALS( i, 7 );

    // Alteração de tipo
    e = '7';
    b &= Test::TEST_EQUALS( e.isFirst(), false );
    b &= Test::TEST_EQUALS( e.isSecond(), true );
    EXPECT_THROW( e.first(), std::bad_cast, b );
    b &= Test::TEST_EQUALS( e.second(), '7' );

    char c = e;
    b &= Test::TEST_EQUALS( c, '7' );

    // Cópia
    Either< int, char > f = e;
    b &= Test::TEST_EQUALS( f.isFirst(), false );
    b &= Test::TEST_EQUALS( f.isSecond(), true );
    EXPECT_THROW( f.first(), std::bad_cast, b );
    b &= Test::TEST_EQUALS( f.second(), '7' );
    c = f;
    b &= Test::TEST_EQUALS( c, '7' );

    // Operadores relacionais
    b &= Test::TEST_EQUALS( e == f, true );
    b &= Test::TEST_EQUALS( e < f, false );
    b &= Test::TEST_EQUALS( f < e, false );
    e = '8';
    b &= Test::TEST_EQUALS( e == f, false );
    b &= Test::TEST_EQUALS( e < f, false );
    b &= Test::TEST_EQUALS( f < e, true );
    f = 800;
    b &= Test::TEST_EQUALS( e == f, false );
    b &= Test::TEST_EQUALS( e < f, false ); // Note que T == int, U == char
    b &= Test::TEST_EQUALS( f < e, true );
    e = 801;
    b &= Test::TEST_EQUALS( e == f, false );
    b &= Test::TEST_EQUALS( f < e, true );
    b &= Test::TEST_EQUALS( e < f, false );

    // Homogêneo
    Either< int, int > j;
    b &= Test::TEST_EQUALS( j.isFirst(), true );
    b &= Test::TEST_EQUALS( j.isSecond(), true );
    j = 7;
    b &= Test::TEST_EQUALS( j.isFirst(), true );
    b &= Test::TEST_EQUALS( j.isSecond(), true );
    b &= Test::TEST_EQUALS( j.first(), 7 );
    b &= Test::TEST_EQUALS( j.second(), 7 );
    Either< int, int > k = j;
    b &= Test::TEST_EQUALS( j == k, true );
    b &= Test::TEST_EQUALS( j < k, false );
    b &= Test::TEST_EQUALS( k < j, false );
    k = 14;
    b &= Test::TEST_EQUALS( j == k, false );
    b &= Test::TEST_EQUALS( j < k, true );
    b &= Test::TEST_EQUALS( k < j, false );
    j = 21;
    b &= Test::TEST_EQUALS( j == k, false );
    b &= Test::TEST_EQUALS( j < k, false );
    b &= Test::TEST_EQUALS( k < j, true );

    // Uso em contêiners
    std::vector< Either< int, char > > v;
    e = 0;
    v.resize( 5 );
    b &= Test::TEST_EQUALS( v.size(), 5 );
    for( auto& x : v )
        b &= Test::TEST_EQUALS( x == e, true );
    v = {1, 5, 'd', 45, 'a', 'b', 'z', 879};
    e = 'a';
    b &= Test::TEST_EQUALS( e == v[4], true );
    e = 45;
    b &= Test::TEST_EQUALS( e == v[3], true );
    b &= Test::TEST_EQUALS( e == v[4], false );

    std::vector< Either<int, char> > w = v;
    w.resize( w.capacity() + 1 ); // Forçar realocação
    w.resize( v.size() );
    for( auto it = w.begin(), jt = v.begin(); it != w.end(); ++it, ++jt )
        b &= Test::TEST_EQUALS( *it == *jt, true );

    std::sort( v.begin(), v.end() );
    for( int i = 0; i < 4; ++i ) {
        b &= Test::TEST_EQUALS( v[i].isFirst(), true );
        b &= Test::TEST_EQUALS( v[i].isSecond(), false );
    }
    for( int i = 4; i < 8; ++i ) {
        b &= Test::TEST_EQUALS( v[i].isFirst(), false );
        b &= Test::TEST_EQUALS( v[i].isSecond(), true );
    }

    w = {1, 5, 45, 879, 'a', 'b', 'd', 'z'};
    for( auto it = w.begin(), jt = v.begin(); it != w.end(); ++it, ++jt )
        b &= Test::TEST_EQUALS( *it == *jt, true );

    return b;
}
