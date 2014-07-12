/* resource.h
 * Classe que simula o gerenciamento de um recurso; feito para
 * testes em testes de unidade para contêiners.
 *
 * Esta classe não possui construtor padrão e possui destrutor não-trivial.
 */
#ifndef RESOURCE_H
#define RESOURCE_H

class Resource {
    bool valid;
public:
    /* Informa a quantidade de cópias (não movimentos) feitas ao
     * longo da execução do programa, desde a última execução de reset(). */
    static unsigned copyCount();

    /* Informa a quantidade de objetos que ainda não foram destruídos. */
    static unsigned aliveCount();

    /* Reinicia a contagem de
     *  - cópias criadas
     *  - variáveis existentes
     */
    static void reset();

    /* Constrói um recurso ativo. */
    static Resource make();

    Resource( unsigned );
    Resource( const Resource& );
    Resource( Resource&& );
    Resource& operator=( const Resource& );
    Resource& operator=( Resource&& );
    ~Resource();
};

#endif // RESOURCE_H
