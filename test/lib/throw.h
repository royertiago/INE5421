/* throw.h
 * Macro que executa uma linha de código especificada
 * e espera que lance uma exceção.
 */
#ifndef THROW_H
#define THROW_H

#include <cstdio>

/* Macro que expande num código que executa a expressão especificada, esperando
 * pela exceção exception. Caso não seja lançada (ou uma excessão diferente
 * o seja), variable é alterada para false.
 *
 * expression não deve conter o ponto-e-vírgula terminal.
 * exception deve ser uma classe; o código conterá a cláusula
 *  catch( exception& )
 * variable pode ser qualquer tipo para o qual a expressão
 *  variable = false;
 * faça sentido.
 * Como a macro está envolvido numa expressão if(true) ... else (void) 0,
 * é seguro usar a macro dentro de laços, e ela exige ser terminada com
 * um ponto-e-vírgula. */
#define EXPECT_THROW( expression, exception, variable )                    \
if( true ) {                                                               \
    try {                                                                  \
        expression;                                                        \
        variable = false;                                                  \
        std::printf( "Expected exception "#exception " by " #expression    \
                " at line %i\n", __LINE__ );         \
    } catch( exception& ) {                                                \
    } catch( ... ) {                                                       \
        std::printf( "Wrong exception thrown by " #expression              \
                " at line %i\n", __LINE__ );         \
        variable = false;                                                  \
    }                                                                      \
} else (void) 0

#endif // THROW_H
