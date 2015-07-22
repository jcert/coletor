#include "listas.h"
#include "comunicacao.h"
#include <QueueList.h>



#define CLOCK_PADRAO 240//240

QueueList <Coleta> queue;
bool flag_vect[1]={0};//[0]=can_i_send
Lista tarefas;
unsigned long contador_global=0; 


ISR(TIMER2_OVF_vect)
{
  if(192<contador_global++)//300
  {
    clock(&tarefas,&queue);
    contador_global = 0;
  }
  TCNT2 =  CLOCK_PADRAO;
  TIFR2 = 0x00;
}

void setup() {
  // put your setup code here, to run once:
  TCCR2B = 0x00; //desliga a interrupÃƒÂ§ÃƒÂ£o, para configurar. T*0
  TCNT2  = CLOCK_PADRAO;
  TIFR2  = 0x00;
  TIMSK2 = 0x01; //tipo de interrupÃƒÂ§ÃƒÂ£o, comparaÃƒÂ§ÃƒÂ£o
  TCCR2A = 0x00;
  TCCR2B = 0x05; //liga a interrupÃƒÂ§ÃƒÂ£o, T/128 
  
  //pinMode(7,OUTPUT);
  Serial.begin(115200);//115200//57600//19200
  init_lista(&tarefas);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(Serial.available())
  {
    if (can_i_read())
    {
      read_and_do(flag_vect,&queue,&tarefas);
    }
  }
  else
  {
    if(can_i_send(flag_vect))
    {
      send_data(&queue);
    }
  }
  delay(100);
}



