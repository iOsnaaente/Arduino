from datetime import datetime 
from serial import Serial 
from struct import unpack

import time 
import sys 

vel = 115200 
comport = Serial('COM3', vel, timeout=1)

for _ in range(20):

    data = datetime.now()

    ano, mes, dia = data.year, data.month, data.day
    hora, minuto, segundos = data.hour, data.minute, data.second

    data_send = [ano, mes, dia, hora, minuto, segundos ]
    data_bytes = [2, 2, 2, 2, 2, 2]

    con = [ int.to_bytes(val, tam, byteorder='little') for val,tam in zip(data_send, data_bytes) ]

    values_bytes = b''
    for val in con:
        values_bytes += val 

    values_bytes += b'\\'

    print('send: ', data_send, " Em bytes : ", values_bytes)
    comport.write( values_bytes )

    read_from_serial = comport.read(4)
    print('receiving Azemute: ', read_from_serial, " Decodificado : ", int.from_bytes( read_from_serial, byteorder='little')/100  )
    
    read_from_serial = comport.read(4)
    print('receiving Zenite: ', read_from_serial, " Decodificado : ", int.from_bytes( read_from_serial, byteorder='little')/100  )
    
    read_from_serial = comport.read(4)
    print('receiving Altura: ', read_from_serial, " Decodificado : ", int.from_bytes( read_from_serial, byteorder='little')/100  )




    time.sleep(3)