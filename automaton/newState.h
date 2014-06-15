/* newState.h
 * Função que gera novos estados para autômatos finitos.
 */
#ifndef NEW_STATE_H
#define NEW_STATE_H

/* Retorna um novo estado para o autômato m.
 * Exigências sobre o template Automaton:
 *  - Possuir a variável-membro state, que é um contêiner;
 *  - Possuir a variável-membro alphabet, que é um contêiner;
 *  - Se não vazios, o último elemento destes dois contêineres deve ser
 *    o maior deles.
 * Os três autômatos do diretório automaton/ satisfazem estas restrições.
 *
 * Exigências sobre o tipo State:
 *  - Suportar somar-se com um inteiro;
 *  - operator< modelar um ordenamento total estrito.
 * Qualquer tipo aritmético (int, double, char) satisfaz esta restrição. */
template< template< typename, typename > class Automaton,
          typename State, typename Symbol 
        >
State generateNewState( const Automaton< State, Symbol >& m ) {
    if( m.states.empty() )
        return State();

    return *m.states.rbegin() + 1;
}

/* Esta sobrecarga é necessária para manter o conjunto de
 * estados e o alfabeto dos autômatos disjuntos. */
template< template< typename, typename > class Automaton, typename State >
State generateNewState( Automaton<State, State> m ) {
    if( m.states.empty() ) {
        if( m.alphabet.empty() )
            return State();
        return *m.alphabet.rbegin() + 1;
    }
    if( m.alphabet.empty() )
        return *m.states.rbegin() + 1;
    State s = *m.alphabet.rbegin();
    State t = *m.states.rbegin();
    if( s < t )
        return t + 1;
    return s + 1;
}

#endif // NEW_STATE_H
