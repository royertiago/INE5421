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

/* Indica que a produção não possui o símbolo que separa não-terminais
 * de terminais, ou possui mais de uma vez. Por exemplo:
 *  "A a A | a"
 *  "A -> a A -> a"
 * 
 * index é o índice da produção que causou o erro. */
struct invalid_replacement_symbol : public syntax_error {
    unsigned index;
    explicit invalid_replacement_symbol( const char * what, unsigned index ) :
        syntax_error( what ),
        index( index )
    {}
};

/* Indica que o lado direito de uma produção a ser interpretada
 * contém mais de um não-terminal ou não contêm não-terminais;
 * por exemplo:
 *  "-> a A | a"
 *  "A B -> a A"
 * Entretanto, está correto
 *  "AB -> a A"
 * AB é interpretado como um único não-terminal da gramática, cujos
 * não-terminais são strings.
 *
 * index é o índice da produção que causou o erro. */
struct invalid_left_hand_side : public syntax_error {
    unsigned index;
    explicit invalid_left_hand_side( const char * what, unsigned index ) :
        syntax_error( what ),
        index( index )
    {}
};
#endif // EXCEPTIONS_H
