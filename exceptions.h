/* exceptions.h
 * Lista de exceções lançadas por funções do programa.
 *
 * Cada função explicita as exceções que pode lançar; este cabeçalho
 * apenas concentra todas as exceções lançadas por este módulo.
 */
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

/* Exceção que deve ser lançada pelos algoritmos ao encontrar probelmas 
 * com tokens, como um token não listado, por exemplo. */
struct token_error : public std::runtime_error {
    explicit token_error( const char * what ) :
        runtime_error( what )
    {}
};

/* Exceção lançada pelas funções de análise sintática caso
 * um erro sintático seja encontrado. */
struct syntax_error : public std::runtime_error {
    explicit syntax_error( const char * what ) :
        runtime_error( what )
    {}
};

#endif // EXCEPTIONS_H
