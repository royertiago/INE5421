/* epsilon.h
 * Tipo que deve ser usado para representar a cadeia de caracteres vazia.
 *
 * Utilizada pelo autômato finito com transições épsilon
 * e para a representação de expressões regulares.
 */
#ifndef EPSILON_H
#define EPSILON_H

/* Tipo que é usado para representar o épsilon.
 * Todos os objetos desta classe serão considerados iguais;
 * entretanto, por conveniência, uma constante "epsilon" é provida. */
struct Epsilon {};
constexpr Epsilon epsilon = Epsilon();

// Operadores relacionais
inline bool operator ==( const Epsilon&, const Epsilon& ) {
    return true;
}
inline bool operator !=( const Epsilon&, const Epsilon& ) {
    return false;
}
inline bool operator < ( const Epsilon&, const Epsilon& ) {
    return false;
}
inline bool operator > ( const Epsilon&, const Epsilon& ) {
    return false;
}
inline bool operator <=( const Epsilon&, const Epsilon& ) {
    return true;
}
inline bool operator >=( const Epsilon&, const Epsilon& ) {
    return true;
}

#endif // EPSILON_H
