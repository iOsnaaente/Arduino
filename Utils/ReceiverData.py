import struct
import serial 
import time 

comp = serial.Serial( 'COM11', baudrate = 9600 )

x_data, y_data, z_data = 0.0, 0.0, 0.0 

last_time = time.time() 

while True:    
    try:
        nBytes = comp.inWaiting()
        pBytes = comp.read(nBytes) 
        x_data, y_data, z_data = struct.unpack('fff', pBytes[:-2] )
        print(x_data, y_data, z_data, time.time()-last_time)
        last_time = time.time() 

    except struct.error as e : 
        print( e )

    time.sleep(0.25)