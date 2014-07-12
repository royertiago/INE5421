/* resource.cpp
 * Implementação de resource.h
 */
#include "resource.h"

namespace {
    static unsigned copyCount = 0;
    static unsigned objectCount = 0;
}

// Métodos estáticos
unsigned Resource::copyCount() {
    return ::copyCount;
}

unsigned Resource::aliveCount() {
    return ::objectCount;
}

void Resource::reset() {
    ::copyCount = 0;
    ::objectCount = 0;
}

Resource Resource::make() {
    return Resource( 42 );
}

// Construtores
Resource::Resource( unsigned ) :
    valid( true )
{
    ::objectCount++;
}

Resource::Resource( const Resource& ) :
    valid( true )
{
    ::copyCount++;
    ::objectCount++;
}

Resource::Resource( Resource&& r ) :
    valid( r.valid )
{
    r.valid = false;
}

Resource& Resource::operator=( const Resource& r ) {
    if( valid && !r.valid )
        ::objectCount--; // Este objeto também passa a ser inválido
    if( !valid && r.valid )
        ::objectCount++; // Este objeto passa a ser válido.

    valid = r.valid;
    return *this;
}

Resource& Resource::operator=( Resource&& r ) {
    bool tmp = valid;
    valid = r.valid;
    r.valid = tmp;
    return *this;
}

Resource::~Resource() {
    if( valid )
        ::objectCount--;
}
