#ifndef LISTAS_H_   /* Include guard */
#define LISTAS_H_


typedef struct dados
{
  //TODO
  int  pino;
  int  periodo;
  int  periodoAlter;//so usado para saÃ­das, serÃ¡ o prÃ³ximo perÃ­odo, seja o de estado alto ou baixo
  int  tempo;
  int  tarefa;
  int   index;
}
Dados;

typedef struct coleta
{
  //TODO
  unsigned int  medida : 10;
  unsigned int    pino : 4 ;
  unsigned int   index : 8 ;
} Coleta;


typedef struct celula
{
  Dados    dados;
  celula   *prox;
}Celula;

typedef Celula* Lista;


bool init_lista(Lista *lista_task);
bool exibe_lista(Lista *lista_task);
bool busca_lista(Lista *lista_task, int states[]);
bool compare(Dados a, Dados b);
bool destroi_elemento_lista(Lista *lista_task, Dados pino, bool opcao);
bool destroi_lista(Lista *lista_task);
bool adiciona_lista(Lista *lista_task, int states[]);
bool adiciona_lista_saida(Lista *lista_task, int states[]);
bool adiciona_lista(Lista *lista_task, Dados info);
bool preenche_pos(Lista *aux, int states[], int tempo_acu);
bool preenche_pos_saida(Lista *lista_task, int states[], int tempo_acu);
bool preenche_pos(Lista *aux, Dados info, int tempo_acu);
#endif





