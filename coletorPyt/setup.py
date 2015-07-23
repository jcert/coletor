'''
Created on 22/03/2015

@author: Jair
'''

BAUDRATE =  115200

if __name__ == '__main__':
    import serial
    import threading
    import time
    import sys
    import csv
    
    iMutex   = threading.Lock()
    oMutex   = threading.Lock()
    executar = True
    
    leituras={}
    leiturasBuffer = leituras
    
    iFile   = open("data.txt","ab")
    iBuffer = ''
    oBuffer = ''
    
    def parse(text):
        text = text.split("|")
        if len(text)==3:
            return text
        else:
            return None
    
    def port(num):  
        if sys.platform.startswith('win32'):
            return 'COM'+str(num)
    pass
    
    
    
    def searchPorts():
        
        if sys.platform.startswith('win32'):
            print 'available serial ports (COM):'
            ports = ['COM' + str(i + 1) for i in range(32)]
            abertas = []
            for port in ports:
                try:
                    ser = serial.Serial(port, BAUDRATE, timeout=1)
                    ser.close()
                    abertas.append(port)
                except:
                    pass
        print abertas
        return abertas    
    
    def ask_data():
        while(executar):
            time.sleep(4)
            sendMess("G")
        pass
    
    def doDataMagic(text):
        global leituras
        if text <> '':
            text = text.split("\r\n")
            for i in text:
                aux = parse(i)#[1,2,3] 
                if aux <> None:
                    if aux[0] in leituras:
                        #print "more data"
                        leituras[aux[0]].append(aux[2])
                    else:
                        #print "more to dict"
                        leituras[aux[0]]=[aux[2]]
                       
                else:
                    if i<>'':
                        print "info:",i
        vectorToCSV(leituras) 
        leituras.clear()                
        pass
    
    def vectorToCSV(sensores):
        #print 'sensors: ',sensores.keys()
        for sensor in sensores:
            with open(sensor+'.csv','ab')as f:
                write = csv.writer(f, delimiter=';')
                for j in sensores[sensor]:
                    write.writerow([j])
 
    def sendMess(text):
        global iBuffer,oBuffer
        oMutex.acquire(1)
        oBuffer = oBuffer + "\n" + text
        oMutex.release()
    
    def comHandle(port):
        global iBuffer,oBuffer,executar
        ser = serial.Serial(port, BAUDRATE, timeout=0)
        
        while(executar):
            oMutex.acquire(1)
            if(len(oBuffer)>0):
                aux = oBuffer.split("\n")
                for i in aux:#envio via serial
                    time.sleep(.1)
                    ser.write(i)
                oBuffer = ''  
            oMutex.release()
            
            #TODO se existe algo no buffer de entrada
                #TODO espera estabilizar o tamanho no buffer
                    #processa o que recebeu
            if(ser.inWaiting()>0):
                atual = ser.inWaiting()
                while True:
                    time.sleep(0.01)
                    novo = ser.inWaiting()
                    if novo == atual:
                        break
                    else:
                        atual = novo
                pass
                iMutex.acquire(1)
                incoming =  ser.read(ser.inWaiting())
                iFile.write(incoming)
                iBuffer = iBuffer + incoming
                iMutex.release()
        ser.close()
        pass
    
    def dataHandle():
        global iBuffer,oBuffer,executar
        while(executar):
            iMutex.acquire(1)
            doDataMagic(iBuffer)#if data, turns into .csv. if other, print to screen
            iBuffer = ''
            iMutex.release()
            time.sleep(2)
        pass
    pass
    
    def adicionaMedida(pino,periodo,funcao):
        sendMess("A"+" "+str(pino)+" "+str(periodo)+" "+str(funcao))
    pass
    
    def adicionaSaida(pino,periodo,periodoHigh):
        sendMess("H"+" "+str(pino)+" "+str(periodo)+" "+str(periodoHigh))
    pass
    
    def alteraMedida(pino,periodo,funcao):
        sendMess("C"+" "+str(pino))
        sendMess("A"+" "+str(pino)+" "+str(periodo)+" "+str(funcao))
    pass
    
    def alteraSaida(pino,periodo,funcao):
        sendMess("C"+" "+str(pino))
        sendMess("A"+" "+str(pino)+" "+str(periodo)+" "+str(funcao))
    pass
    
    def removePino(pino,tarefa):
        sendMess("C"+" "+str(pino)+" "+str(tarefa))
    pass

    def exibir():
        sendMess("E")
        time.sleep(3)
    pass
        
    open_ports = searchPorts()
    port_choice = int(raw_input('Em qual esta o arduino?'))
    if(not (port(port_choice) in open_ports)):
        print 'fim'
        exit()
    
    comunica = threading.Thread(target=comHandle,args=[port(port_choice)])
    organiza = threading.Thread(target=dataHandle)
    chama = threading.Thread(target=ask_data)
    
    comunica.start()
    organiza.start()
    chama.start()
    while(executar):
        print("Menu"
              "\n\tAdicionar\t1"
              "\n\tRemove\t\t2"
              "\n\tExibir Tarefas\t3"
              "\n\tComando Manual\t4"
              "\n\tComando Gravado\t5"
              "\n\tSair\t\t6")
        
        aux = input("comando:")
        if aux ==1:
            print("Tipos"
                "\n\tAnalogico\t\t1"
                "\n\tDigital\t\t\t2"
                "\n\tSaida\t\t\t3")
            
            aux = input("tipo:")
            pin = input("pino:")
            per = input("periodo:")
            if aux==1:
                adicionaMedida(pin,per,3)
            elif aux==2:
                adicionaMedida(pin,per,1)
            elif aux==3:
                perH = input("periodo em HIGH:")
                adicionaSaida(pin,per,perH)    
            
        elif aux==2:
            print("Tipos"
                "\n\tDigital\t\t\t1"
                "\n\tSaida\t\t\t2"
                "\n\tAnalogico\t\t3")
            aux = input("tipo:")
            pin = input("pino:")
            removePino(pin,aux)
            
        elif aux==3:
            exibir()

        elif aux==4:  
            print ("Arduino Ajuda:"
                   "\n\tADICIONA  1: pino periodo funcao"
                   "\n\tBUSCA     2: pino"
                   "\n\tDESTROI   3"
                   "\n\tEXIBE     4"
                   "\n\tDESTROI_E 5"
                   "\n\tCLOCK     6"
                   "\n\tGET_DATA  7")
            aux = input("comando:")
            sendMess(aux)
        elif aux == 5:
            #adicionaMedida(1,10,3)
            adicionaMedida(2,2,3)
            adicionaMedida(3,2,3)
            adicionaMedida(4,2,3)
            adicionaMedida(5,2,3)
            adicionaMedida(6,2,3)
            adicionaMedida(7,2,3)
            adicionaMedida(8,2,3)
            adicionaMedida(9,2,3)
            adicionaSaida(9,2,1)
                    
        elif aux==6:    
            executar = False
        pass
    pass
    
    print 'fim'
    
    
    
