#include <arduino.h>
#include "tarefas.h"
#include "listas.h"



void do_task(Dados *a, QueueList<Coleta> *b)
{
  int var;
  Coleta medida = {var,a->pino,a->index};
  switch(a->tarefa)
  {
   case DIGITAL_IN:
      medida.medida = digitalRead(a->pino);
      b->push(medida);
      break; 
   case DIGITAL_OUT:
     digitalWrite(a->pino,!digitalRead(a->pino));//toggle do estado do pino
     var = a->periodoAlter;
     a->periodoAlter = a->periodo;
     a->periodo    =  var;
      break;
   case ANALOG_IN:
      medida.medida = analogRead(a->pino); 
      b->push(medida);
      break;   
  }
}

void update_top(Lista *lista_task, QueueList<Coleta> *b)
{
  if( *lista_task == NULL) return; // se a lista estÃƒÂ¡ vazia nÃƒÂ£o hÃƒÂ¡ o que fazer
  
  Dados aux = (*lista_task)->dados;
  do_task(&((*lista_task)->dados),b);
  destroi_elemento_lista(lista_task, aux, false);
  adiciona_lista(lista_task, aux);
  
  
}

void clock(Lista *lista_task, QueueList<Coleta> *b)
{
  if( *lista_task == NULL) return; // se a lista estÃƒÂ¡ vazia nÃƒÂ£o hÃƒÂ¡ o que fazer
  
  while( (*lista_task)->dados.tempo == 0 ) update_top(lista_task,b); // chegou a hora da tarefa no topo da lista
  (*lista_task)->dados.tempo--;
  //exibe_lista(lista_task);
  return;


}


