from SerialReader import Serial as sr
import spi

# Pinos ligados no arduino 
ledPin = 13 
clockPin = 12
CSnPin = 10
inputPin = 2 


BAUDRATE = 1000000

# definição das portas seriais conectadas ao RS845
while True:    
    print('Detectando portas Seriais....\n')
    comports = sr.serialPorts()
    for i, com in enumerate(comports):
        print(i, com, end='\t')
    
    if comports != []:
        ind = input("\nEscolha a porta Serial onde esta conectado o Conversor RS845: ")
    else: 
        print("Nenhuma porta serial detectada ! \nPressione enter para atualizar")
        input()

    try:
        ind = int(ind)
        comport = serial.Serial(comports[ind], baudrate= BAUDRATE)
        comport.close()
        comport = comports[ind]
        break
    except:
        print("Comport inválida, tente outra!")
        comport = 0

spi = spi.SPI(comport)

spi.mode = spi.SPI.MODE_0
spi.bits_per_word = 8
spi.speed = 1000000

received = spi.transfer([0x11, 0x22, 0xFF])
spi.write([0x12, 0x34, 0xAB, 0xCD])

received = spi.read(10)



