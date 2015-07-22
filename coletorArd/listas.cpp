#include <arduino.h>
#include "listas.h"

void print( Dados x)
{
      Serial.print("|d");
      Serial.print(x.pino);
      Serial.print("|te");
      Serial.print(x.tempo);
      Serial.print("|ta");
      Serial.print(x.tarefa);
      Serial.print("|in");
      Serial.print(x.index);
};

bool init_lista(Lista *lista_task)
{
    *lista_task = NULL;
    return true;
};

bool exibe_lista(Lista *lista_task)
{
    Lista aux = *lista_task;
    while(aux!= NULL)
    {
      
      print((aux)->dados);
      Serial.println();
      (aux) = (aux)->prox;
    }
    Serial.println(" exibe_tasks ");

};
bool busca_lista(Lista *lista_task, int states[])
{
    Lista aux = *lista_task;
    int alvo  = states[1];
    
    Serial.println(" busca_lista ");
    
    while(aux!= NULL)
    {
      if (aux->dados.pino == alvo)
      {
        Serial.println("Existente");
        return true;
      }
      aux = aux->prox;
    }
    Serial.println("Inexistente");
    return false;
};

bool compare(Dados a, Dados b)
{
     bool resultado = a.pino == b.pino;
     resultado &= a.tarefa == b.tarefa; 
    return resultado;
};

bool destroi_elemento_lista(Lista *lista_task, Dados alvo, bool opcao)
{
    Lista anterior  = *lista_task;
    Lista aux       = *lista_task;
    
    while(aux!= NULL)
    {
      if (compare(aux->dados,alvo))
      {
        if( anterior != aux)
        {
          anterior->prox = aux->prox;
          delete aux; 
        }else{
          aux = aux->prox;
          delete anterior;
          *lista_task = aux;
        }
        if(opcao && (alvo.tarefa== 2))pinMode(alvo.pino, INPUT);//se é pra deletar definitivamente e é uma saida digital, representada pelo 2;
        return true;
      }
      anterior = aux;
      aux = aux->prox;
    }
    Serial.println("Inexistente");
    return false;
};

bool destroi_lista(Lista *lista_task)
{
    Serial.println(" destroi_lista ");


};

bool adiciona_lista(Lista *lista_task, int states[])
{
    int tempo_acumulado = 0;
    int periodo = states[2]; //iniciado como o periodo
    //pino, periodo, tempo ate execucao
    bool percorrer = true;
    
    while( percorrer )
    {
      percorrer &= *lista_task != NULL;
      if( percorrer && ( (tempo_acumulado + (*lista_task)->dados.tempo) < periodo ))
      {
        tempo_acumulado += (*lista_task)->dados.tempo;
        lista_task = &(*lista_task)->prox;
      }
      else
      {
        percorrer &= false;
      }
    }
    return preenche_pos(lista_task,states,tempo_acumulado);

};

bool adiciona_lista_saida(Lista *lista_task, int states[])
{
    int tempo_acumulado = 0;
    int periodo = states[2]; //iniciado como o periodo
    //pino, periodo, tempo atÃƒÂ© execuÃƒÂ§ÃƒÂ£o
    bool percorrer = true;
    
    
    while( percorrer )
    {
      percorrer &= *lista_task != NULL;
      if( percorrer && ( (tempo_acumulado + (*lista_task)->dados.tempo) < periodo ))
      {
        tempo_acumulado += (*lista_task)->dados.tempo;
        lista_task = &(*lista_task)->prox;
      }
      else
      {
        percorrer &= false;
      }
    }
    return preenche_pos_saida(lista_task,states,tempo_acumulado);

};

bool adiciona_lista(Lista *lista_task, Dados info)
{
    int tempo_acumulado = 0;
    int periodo = info.periodo; //iniciado como o periodo
    //pino, periodo, tempo atÃƒÂ© execuÃƒÂ§ÃƒÂ£o
    bool percorrer = true;
    
    while( percorrer )
    {
      percorrer &= *lista_task != NULL;
      if( percorrer && ( (tempo_acumulado + (*lista_task)->dados.tempo) <= periodo ))
      {
        tempo_acumulado += (*lista_task)->dados.tempo;
        lista_task = &(*lista_task)->prox;
      }
      else
      {
        percorrer &= false;
      }
    }
    return preenche_pos(lista_task,info,tempo_acumulado);

};

bool preenche_pos(Lista *lista_task, int states[], int tempo_acu)
{
    Dados dado={states[1],states[2],0,states[2]-tempo_acu,states[3],0};
    Celula a={dado,*lista_task};
    
    *lista_task = new Celula;
    **lista_task = a;
}

bool preenche_pos_saida(Lista *lista_task, int states[], int tempo_acu)
{
    Dados dado={states[1],states[2]-states[3],states[3],states[2]-tempo_acu,2,0};
    Celula a={dado,*lista_task};//comeÃ§ara no estado baixo
    
    pinMode(dado.pino,OUTPUT);
    
    *lista_task = new Celula;
    **lista_task = a;
}

bool preenche_pos(Lista *lista_task, Dados info, int tempo_acu)
{
    Dados dado =  info;
    dado.tempo =  dado.periodo;
    dado.tempo -= tempo_acu;
    dado.index++;
    Celula a={dado,*lista_task};
    
    *lista_task = new Celula;
    **lista_task = a;
}


