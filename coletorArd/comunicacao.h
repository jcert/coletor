#ifndef COMUNICACAO_H_   /* Include guard */
#define COMUNICACAO_H_

#include "listas.h"
#include "tarefas.h"

bool can_i_read();
bool read_and_do(bool[], QueueList<Coleta>*,Lista*);
bool can_i_send(bool[]);
bool send_data(QueueList<Coleta>*);
void send_all(QueueList<Coleta>*);

#endif







