/* either.test.cpp
 * Teste de unidade para a classe Either.
 */
#include "utility/either.h"

#include <algorithm> // sort
#include <map>
#include "test/mock/resource.h"
#include <tuple>
#include <vector>
#include "test/lib/test.h"

namespace {
// Retorna true caso o valor passado coincida com o valor de t.
bool eitherForwardTest( std::tuple<Either<int, char>> t, char c ) {
    return std::get<0>(t).is<char>() && std::get<0>(t).getAs<char>() == c;
}
bool eitherForwardTest( std::tuple<Either<int, char>> t, int i ) {
    return std::get<0>(t).is<int>() && std::get<0>(t).getAs<int>() == i;
}

Either<double, double, double> getEitherDouble() {
    return Either<double, double, double>( 3.5 );
}

Either< Resource, int > getEitherResource() {
    return Resource( 23 );
}

} // anonymous namespace

DECLARE_TEST( EitherTest ) {
    bool b = true;

    // Construtor padrão
    Either< int, char > e;
    b &= Test::TEST_EQUALS( e.is<int>(), true );
    b &= Test::TEST_EQUALS( e.is<char>(), false );
    b &= Test::TEST_EQUALS( e.getAs<int>(), 0 );
    EXPECT_THROW( e.getAs<char>(), std::runtime_error, b );

    // Construtor de atribuição
    Either< int, char > p = 'A';
    b &= Test::TEST_EQUALS( p.is<int>(), false );
    b &= Test::TEST_EQUALS( p.is<char>(), true );
    EXPECT_THROW( p.getAs<int>(), std::runtime_error, b );
    b &= Test::TEST_EQUALS( p.getAs<char>(), 'A' );

    Either< int, char > q = (int) 'A';
    b &= Test::TEST_EQUALS( q.is<int>(), true );
    b &= Test::TEST_EQUALS( q.is<char>(), false );
    b &= Test::TEST_EQUALS( q.getAs<int>(), (int) 'A' );
    EXPECT_THROW( q.getAs<char>(), std::runtime_error, b );

    // Atribuição
    e = 7;
    b &= Test::TEST_EQUALS( e.is<int>(), true );
    b &= Test::TEST_EQUALS( e.is<char>(), false );
    b &= Test::TEST_EQUALS( e.getAs<int>(), 7 );
    EXPECT_THROW( e.getAs<char>(), std::runtime_error, b );

    int i = e;
    b &= Test::TEST_EQUALS( i, 7 );

    // Alteração de tipo
    e = '7';
    b &= Test::TEST_EQUALS( e.is<int>(), false );
    b &= Test::TEST_EQUALS( e.is<char>(), true );
    EXPECT_THROW( e.getAs<int>(), std::runtime_error, b );
    b &= Test::TEST_EQUALS( e.getAs<char>(), '7' );

    char c = e;
    b &= Test::TEST_EQUALS( c, '7' );

    // Cópia
    Either< int, char > f = e;
    b &= Test::TEST_EQUALS( f.is<int>(), false );
    b &= Test::TEST_EQUALS( f.is<char>(), true );
    EXPECT_THROW( f.getAs<int>(), std::runtime_error, b );
    b &= Test::TEST_EQUALS( f.getAs<char>(), '7' );
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
    b &= Test::TEST_EQUALS( e < f, false );
    b &= Test::TEST_EQUALS( f < e, true );
    e = 801;
    b &= Test::TEST_EQUALS( e == f, false );
    b &= Test::TEST_EQUALS( f < e, true );
    b &= Test::TEST_EQUALS( e < f, false );

    // Homogêneo
    Either< int, int > j;
    b &= Test::TEST_EQUALS( j.is<int>(), true );
    j = 7;
    b &= Test::TEST_EQUALS( j.is<int>(), true );
    b &= Test::TEST_EQUALS( j.getAs<int>(), 7 );
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

    // forward_as_tuple

    b &= Test::TEST_EQUALS( eitherForwardTest(
                std::forward_as_tuple(
                        Either<int, char>('a')
                )
            , 'a'), true );
    b &= Test::TEST_EQUALS( eitherForwardTest(
                std::forward_as_tuple( std::move(
                        Either<int, char>('b')
                ) )
            , 'a'), false );
    b &= Test::TEST_EQUALS( eitherForwardTest(
                std::forward_as_tuple( std::move(
                        Either<int, char>(7)
                ) )
            , 'a'), false );
    b &= Test::TEST_EQUALS( eitherForwardTest(
                std::forward_as_tuple( std::move(
                        Either<int, char>(7)
                ) )
            , 7), true );
    b &= Test::TEST_EQUALS( eitherForwardTest(
                std::forward_as_tuple( std::move(
                        Either<int, char>(8)
                ) )
            , 7), false );
    b &= Test::TEST_EQUALS( eitherForwardTest(
                std::forward_as_tuple( std::move(
                        Either<int, char>(7)
                ) )
            , '7'), false );

    // Semânticas de movimento
    Either<double, double, double> ed = getEitherDouble();
    b &= Test::TEST_EQUALS( ed.getAs<double>(), 3.5 );

    Either<double, double, double> ef = std::move(getEitherDouble());
    b &= Test::TEST_EQUALS( ef.getAs<double>(), 3.5 );

    ed = 2.0;
    ed = getEitherDouble();
    b &= Test::TEST_EQUALS( ed.getAs<double>(), 3.5 );

    ef = 2.0;
    ef = std::move( getEitherDouble() );
    b &= Test::TEST_EQUALS( ef.getAs<double>(), 3.5 );

    // Uso em contêiners - std::vector
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
        b &= Test::TEST_EQUALS( v[i].is<int>(), true );
        b &= Test::TEST_EQUALS( v[i].is<char>(), false );
    }
    for( int i = 4; i < 8; ++i ) {
        b &= Test::TEST_EQUALS( v[i].is<int>(), false );
        b &= Test::TEST_EQUALS( v[i].is<char>(), true );
    }

    w = {1, 5, 45, 879, 'a', 'b', 'd', 'z'};
    for( auto it = w.begin(), jt = v.begin(); it != w.end(); ++it, ++jt )
        b &= Test::TEST_EQUALS( *it == *jt, true );

    // Uso em contêiners - std::map
    std::map< Either< int, char >, bool > m; // m[int] == true
    m.insert({Either<int, char>( 'A' ), true});
    m.insert({Either<int, char>( 65 ), false});

    for( const auto& pair : m )
        b &= Test::TEST_EQUALS( pair.first.is<char>(), pair.second );

    auto result = m.insert({Either<int, char>( 'A' ), false});
    b &= Test::TEST_EQUALS( result.second, false );

    result = m.insert({Either<int, char>( 65 ), true});
    b &= Test::TEST_EQUALS( result.second, false );

    m.insert({ 'B', true });
    m.insert({ 66, false });
    b &= Test::TEST_EQUALS( m.size(), 4 );

    for( const auto& pair : m )
        b &= Test::TEST_EQUALS( pair.first.is<char>(), pair.second );

    m.at( 'A' ) = false;
    m.at( 'B' ) = false;
    m.at( 65 ) = true;
    m.at( 66 ) = true;

    for( const auto& pair : m ) // não é mais is<char>
        b &= Test::TEST_EQUALS( pair.first.is<int>(), pair.second );

    m['A'] = true;
    m['B'] = true;
    m['C'] = true;
    m[ 65 ] = false;
    m[ 66 ] = false;
    m[ 67 ] = false;
    b &= Test::TEST_EQUALS( m.size(), 6 );
    for( const auto& pair : m )
        b &= Test::TEST_EQUALS( pair.first.is<char>(), pair.second );

    v.clear();
    for( const auto& pair : m )
        v.insert( v.begin(), pair.first ); // testar movimento

    w = {'C', 'B', 'A', 67, 66, 65};
    for( auto it = w.begin(), jt = v.begin(); it != w.end(); ++it, ++jt )
        b &= Test::TEST_EQUALS( *it == *jt, true );

    // Uso em contêiners - map com Either homogêneo
    std::map< Either<double, double, double>, bool > n;
    n.insert({Either<double, double, double>(1.0), true});
    n.insert({Either<double, double, double>(-1.0), false});

    for( const auto& pair : n )
        b &= Test::TEST_EQUALS( pair.first.getAs<double>() > 0, pair.second );

    auto resul = n.insert({Either<double, double, double>(1.0), true});
    b &= Test::TEST_EQUALS( result.second, false );

    resul = n.insert({Either<double, double, double>(-1.0), false});
    b &= Test::TEST_EQUALS( result.second, false );
    
    for( const auto& pair : n )
        b &= Test::TEST_EQUALS( pair.first.getAs<double>() > 0, pair.second );

    n.insert({ 2.0, true });
    n.insert({ -2.0, false });
    b &= Test::TEST_EQUALS( n.size(), 4 );

    for( const auto& pair : n )
        b &= Test::TEST_EQUALS( pair.first.getAs<double>() > 0, pair.second );

    n.at( 1.0 ) = false;
    n.at( 2.0 ) = false;
    n.at( -1.0 ) = true;
    n.at( -2.0 ) = true;

    for( const auto& pair : n )
        b &= Test::TEST_EQUALS( pair.first.getAs<double>() < 0, pair.second );

    n[1.0] = true;
    n[2.0] = true;
    n[3.0] = true;
    n[-1.0] = false;
    n[-2.0] = false;
    n[-3.0] = false;
    b &= Test::TEST_EQUALS( n.size(), 6 );
    for( const auto& pair : n )
        b &= Test::TEST_EQUALS( pair.first.getAs<double>() > 0, pair.second );

    std::vector< Either<double, double, double> > vd;
    vd.clear();
    for( const auto& pair : n )
        vd.insert( vd.begin(), pair.first ); // testar movimento

    std::vector< Either<double, double, double>> wd
        = {3.0, 2.0, 1.0, -1.0, -2.0, -3.0};
    for( auto it = wd.begin(), jt = vd.begin(); it != wd.end(); ++it, ++jt )
        b &= Test::TEST_EQUALS( *it == *jt, true );

    // Teste com objetos complexos
    {
        Resource::reset();

        Resource r( 17 );
        Either< Resource, int > e = std::move( r );
        b &= Test::TEST_EQUALS( Resource::copyCount(), 0 );

        Either< Resource, int > f = 7;
        f = std::move( e );
        b &= Test::TEST_EQUALS( Resource::copyCount(), 0 );

        Either< Resource, int > g = Resource::make();
        b &= Test::TEST_EQUALS( Resource::copyCount(), 0 );

        g = f = 7;
        f = getEitherResource();
        b &= Test::TEST_EQUALS( Resource::copyCount(), 0 );
    }
    b &= Test::TEST_EQUALS( Resource::aliveCount(), 0 );
    return b;
}
