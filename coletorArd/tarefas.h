#ifndef TAREFAS_H_   /* Include guard */
#define TAREFAS_H_

#include "listas.h"
#include <QueueList.h>
#define DIGITAL_IN   1
#define DIGITAL_OUT  2
#define ANALOG_IN    3



//QueueListqueue;

void update_top(Lista *lista_task, QueueList<Coleta> *b);
void do_task(Dados *a, QueueList<Coleta> *b);
void clock(Lista *lista_task, QueueList<Coleta> *b);


#endif






