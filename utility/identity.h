/* identity.h
 * Tipo auxiliar, usado para definir especializações de
 * funções membro sem precisar sespecializar toda a classe.
 *
 * Ela está num cabeçalho separado para evitar problemas
 * com múltiplas definições, caso esta estratégia seja usada
 * por vários cabeçalhos incluídos por uma mesma unidade
 * de tradução.
 */
#ifndef IDENTITY_H
#define IDENTITY_H

template< typename T >
struct identity {
};

#endif // IDENTITY_H
