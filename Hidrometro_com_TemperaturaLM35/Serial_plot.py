#! /usr/bin/python

import serial
import glob
import time
import sys

"""
    A função funciona tanto em Windows quanto em Linux 
    
    Testa todas as portas possíveis no computador e tenta abri-las
        Caso ele consiga, significa que a porta existe
        Caso a porta não possa ser aberta, ela existe
        Caso retorne Erro (SerialException), ela não existe
    
    Retorna uma lista com os nomes das portas disponíveis 

"""

# IDENTIFICA O so E AS PORTAS POR TENTATIVA
def serialPorts():
    # Abre se o SO for Windows
    if sys.platform.startswith('win'):  
        ports = ['COM%s' % (i + 1) for i in range(256)]
    
    # Abre se o SO for Linux
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        ports = glob.glob('/dev/tty[A-Za-z]*')
    
    # Caso não seja nenhum dos dois, ele não suporta
    else:
        print("Sistema Operacional não suportado")

    # Testa as portas disponíveis 
    listaPorts = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            listaPorts.append(port)
        except (OSError, serial.SerialException):
            pass

    return listaPorts


# PRINTA E RETORNA UMA LISTA DE PORTAS ABERTAS 
def showSerialAvailable():
    listaPortas = serialPorts()
    if listaPortas is None:
        print("Não há portas Seriais abertas !!")
    else:
        for port in listaPortas:
            print(port, end="\n")
    return listaPortas


# INICIA O PROCESSO DE ABERTURA DA comport
def initSerialListening(DEVICE, BAUDRATE, TIMEOUT):
    # Iniciando conexao serial
    #comport = serial.Serial('/dev/ttyUSB4', 9600, timeout=1)
    comport = serial.Serial(DEVICE, BAUDRATE, timeout=TIMEOUT)
    return comport


# FECHA A comport SERIAL
def closeSerialConnection(comport):
    # Fechando conexao serial
    comport.close()


# RECEBE OS VALORES DA SERIAL comport
def getSerialValues(comport):
    return str(comport.readline()).split(" ")



# Código __main__ atrelado ao código Medicao_umidade_receptor
if __name__ == "__main__":

	comports = serialPorts()
	baudrate = 9600
	timeout  = 1

	comport = initSerialListening(comports[0], baudrate, timeout)

	dia = 0
	mes = 0
	ano = 0

	semana = 0

	horas = 0
	minutos = 0
	segundos = 0

	temperatura = 0 

	umidadePlanta1 = 0
	umidadePlanta2 = 0

	list_of_elements = [
						dia,
						mes,
						ano,
						semana,
						horas,
						minutos,
						segundos,
						temperatura, 
						umidadePlanta1,
						umidadePlanta2
						]
	# Abre o aquivo no modo 'a' (append text)
	fo = open("dados.txt","a")
	
	# Deve ser usado somente na primeira vez preferencialmente
	"""
	fo.write("dia\t")
	fo.write("mes\t")
	fo.write("ano\t")
	fo.write("Semana\t")
	fo.write("hora\t")
	fo.write("minuto\t")
	fo.write("segundo\t")
	fo.write("temp\t")
	fo.write("umid1\t")
	fo.write("umid2\n")
	"""
	
	while True:
		
		values = getSerialValues(comport)
		
		if len(values) < 10:
			print("Erro de leitura")
		
		else: 
			for i in range(len(list_of_elements)):
				list_of_elements[i] = int(values[i+1])
				fo.write(values[i+1])
				fo.write("\t")
			
			fo.write("\n")
			print(list_of_elements)
		
		# limite de medição
		if list_of_elements[4] is 7:
			fo.close()
			print("Fim do teste")
			break

