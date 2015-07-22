#include <arduino.h>
#include "comunicacao.h"
#include "listas.h"

#define ADICIONA_E  1 // A pino periodo funÃ§Ã£o
#define BUSCA     2 // B pino
#define DESTROI   3 // C pino
#define EXIBE     4
#define DESTROI_E 5
#define CLOCK     6
#define GET_DATA  7
#define ADICIONA_S  8//H pino periodo periodoAlto

#define MAX_ARG   5

void print_string(char *a)
{
    int i=0;
    while(a[i]!='\0') 
    {
      Serial.print(a[i]);
      i++;
    }
    Serial.println();
}

bool can_i_read()
{
	int i;
	for(i=0; (i<50) && !Serial.available(); i++)
	{
		delay(10);
	}
	if (i >=50 )
	{
		return false;
	}
	else
	{
		return true;
	}
};

bool is_number(char x)
{
	return (x>='0')and(x<='9');
	
}

int  to_number(int start, char command[])
{

	int 	number 		 = 0;
	int 	position 	 = start;
	bool 	only_numbers     = true;
	while(command[position]!=' ' && command[position]!='\b')
	{
	
		number 	*= 10;
		number 	+= command[position]-'0';
		only_numbers&= is_number(command[position]);
		position++;
	};

	if(only_numbers)
	{
		return number;
	}else
	{
		return -1;
	}
	
}

int parse_command(int num_arg ,char command[])
{
	int i =0;
        
	while(num_arg>0)
	{
		while(command[i]!=' ')
		{
			
                        i++;      
		}
		num_arg--;
		i++;
	}
	int state = -1;
	
	//this if is used mostly to convert the parameter passed as string to integers,
	//the first parameter is a letter and will be converted to a number, all the other should be
	//already a integer number number, but will also be passed as a string; If it returns a -1
	//the given parameter is not a integer, nor a valid char.
	if ( is_number(command[i]) )
	{
		state = to_number(i,command);
	}
	else
	{
		switch(command[i])
		{
			case 'A':// Adiciona, add new measure or task
				state = ADICIONA_E;
				break;
			case 'B'://	Busca, 	search if given pin is in use
				state = BUSCA;
				break;
                        case 'C':// Destroi, eliminates the task atributted in a given pin
				state = DESTROI_E;
				break;
			case 'D':// Destroi, eliminates all tasks
				state = DESTROI;
				break;
			case 'E':// Exibe,	shows all the tasks currently active
				state = EXIBE;
				break;	
                        case 'F':// forÃ§a um sinal de clock, que normalmente Ã© temporizado
				state = CLOCK;
				break;
                        case 'G':// pede por todos os dado atualmente no arduino
				state = GET_DATA;
				break;
                        case 'H':
                                state = ADICIONA_S;
                                break;
	
		}	
	}
	
	
	return state;
	
};

void do_command(int num_arg, int states[], QueueList<Coleta>* b, Lista *lista_task)
{

        switch(num_arg)
        {
        case 1:
            switch(states[0])
            {
              case EXIBE://Exibe
                exibe_lista(lista_task);
                break;
              case CLOCK://Clock
                clock(lista_task,b);
                break;  
              case GET_DATA://Clock
                send_all(b);
                break;                    
            }
          break;
        case 2:
          switch(states[0])
            {
              case BUSCA://Busca
                busca_lista(lista_task, states);
                break;
              case DESTROI://Destroi
                destroi_lista(lista_task);
                break;

            }
          break;
        case 3:
          switch(states[0])
              {
              case DESTROI_E://Destroi elemento
                Dados aux = {states[1],0,0,0,states[2],0};//monta obj Dados só com pino e tarefa, o que é levado em conta para achar e deletar
                destroi_elemento_lista(lista_task, aux, true);
              break;  
            };  
        break;
        case 4:
          switch(states[0])
          {
            case ADICIONA_E://Adiciona entrada
              adiciona_lista(lista_task, states);
              break;
            case ADICIONA_S://Adiciona saida
              adiciona_lista_saida(lista_task, states);
              break;  
          
              
          }
          break;
        }
	// must use the states give to perform the task identified in the first state, using the other states as parameters
	
	
}

void do_com(int num_arg ,char command[],QueueList<Coleta>* b, Lista *lista)
{
	int states[num_arg]; 	//will store the number representing each command and its arguments, being used in a switch case
				//to do the command in question;
	for(int i=0; i < num_arg ; i++ )
	{
		states[i] = parse_command(i,command);	//must go through the command starting from the i-th word and return 
  		                                        //the equivalent number to state. returns an int always
	}

        delete[] command;
	do_command(num_arg,states,b,lista);
	
	
};

bool read_com_port(int *num_arg, char **arg)
{
	char storage[500+1];//last char is a '\b' terminator
	int    i = 0;
	*num_arg = 0;
	while(Serial.available() and i<500)
	{
		storage[i] = Serial.read();
		if( storage[i] == ' ')
		{
			(*num_arg)++;
		}
		i++;
	}
        if( storage[i-1] != ' ')
        {
            storage[i] = ' ';
            i++;
        }
        
        storage[i] = '\b'; //places null terminator, to work as a string
        i++;
        
        (*num_arg)++;
        if(*num_arg>MAX_ARG or i>=500)
	{
		return false;
	}
	else
	{
		*arg = new char[i+1];
                (*arg)[i] = '\0';
		for(int j=0; j<i; j++)
		{
			(*arg)[j] = storage[j];
		}
	}
        return true;
};

bool read_and_do(bool flags_vector[], QueueList <Coleta> *queue, Lista *lista)
{
	int num_arg;
	char *command;
	
	if ( read_com_port(&num_arg,&command) )
	{
                do_com(num_arg,command,queue,lista);
	}
	else
	{
		Serial.println("ERRO");
	};

};

bool can_i_send(bool flags_vector[])
{
	//asks if someone is listening and awaits answer, times out in 0.5s. 
	//returns true if there's a response
        if(flags_vector[0] == true)
        {
          return true;
        }
        //TODO
        //asks for an echo
	return false;
	
	
};
bool send_data(QueueList <Coleta> *queue)
{
	/*
	//TODO, it sends all the data it can from the non-volatile memory
        for(int i=0; i< queue->count(); i++)
      {
        Coleta aux = queue->pop();
        Serial.write((uint8_t*)&aux,sizeof(aux));
        Serial.write(NULL);
      }  */
	return true;
	
};

void send_all(QueueList<Coleta>* data)
{
    while(!data->isEmpty())
    {
      Coleta aux = data->pop();
      Serial.print(aux.pino) ;Serial.print("|");
      Serial.print(aux.index);Serial.print("|");
      Serial.println(aux.medida);
      delay(1);//ver se o problema eram muitos seriais atolados
    };
};


