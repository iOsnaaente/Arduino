#! /usr/bin/python

from SerialList import serialPorts
import serial
import time
import sys 

def showSerialAvailable():
    listaPortas = serialPorts()
    if listaPortas is None:
        print("Não há portas Seriais abertas !!")
    else:
        for port in listaPortas:
            print(port, end="\n")
    return listaPortas


def initSerialListening(DEVICE, BAUDRATE, TIMEOUT):
    # Iniciando conexao serial
    #comport = serial.Serial('/dev/ttyUSB4', 9600, timeout=1)
    comport = serial.Serial(DEVICE, BAUDRATE, timeout=TIMEOUT)
    return comport


def getSerialValues(comport):

    VALUE_SERIAL = str(comport.readline()).split(",")

    VALUE_SERVO = VALUE_SERIAL[0].split("b'")[-1]
    VALUE_SONAR = VALUE_SERIAL[-1].replace("\\r\\n'", "")

    return VALUE_SERVO,VALUE_SONAR


def closeSerialConnection(comport):
    # Fechando conexao serial
    comport.close()