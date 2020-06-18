import sys
import glob
import serial

"""
    A função funciona tanto em Windows quanto em Linux 
    
    Testa todas as portas possíveis no computador e tenta abri-las
        Caso ele consiga, significa que a porta existe
        Caso a porta não possa ser aberta, ela existe
        Caso retorne Erro (SerialException), ela não existe
    
    Retorna uma lista com os nomes das portas disponíveis 

"""

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