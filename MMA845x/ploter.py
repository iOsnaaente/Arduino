import dearpygui.dearpygui as dpg 

import numpy as np
import struct
import serial 
import time 


COMPORT = 'COM16'
BAUDRATE = 9600

try:    comp = serial.Serial( COMPORT, baudrate = BAUDRATE )
except: print(0)

X = [ 0 for _ in range(100) ]
Y = [ 0 for _ in range(100) ]
Z = [ 0 for _ in range(100) ]

def make_data_run( phase ):
    global X
    global Y
    global Z
    t = np.linspace(0,2*np.pi, 100)
    X = np.cos(t + np.radians(phase) )
    Y = np.sin(2*t + np.radians(phase) )
    Z = np.cos(t + np.radians(phase) )*np.sin(t + np.radians(phase) )
    
    dpg.configure_item("X_value", x = t, y = X )
    dpg.configure_item("Y_value", x = t, y = Y )
    dpg.configure_item("Z_value", x = t, y = Z )

def att_data_run( x_val, y_val, z_val ):
    global X
    global Y 
    global Z
    
    t = np.linspace(0,2*np.pi, 100)
    
    X.append(x_val)
    X.pop(0)
    
    Y.append(y_val)
    Y.pop(0)
    
    Z.append(z_val)
    Z.pop(0)

    dpg.configure_item("X_value", x = t, y = X )
    dpg.configure_item("Y_value", x = t, y = Y )
    dpg.configure_item("Z_value", x = t, y = Z )

    

with dpg.window() as main_window:
    pass

with dpg.window(id='Ploter', label="simple plot", no_resize=True, no_title_bar=True, no_move=True):
    with dpg.plot(id='Graph', label="Plot Acelerometro", height=300, width=400, anti_aliased=True):
        dpg.add_plot_legend()

        dpg.add_plot_axis(dpg.mvXAxis, label="Tempo", id='x_axis')
        dpg.set_axis_limits("x_axis", 0, 2*3.1415)

        dpg.add_plot_axis(dpg.mvYAxis, label="Valores XYZ", id="y_axis")
        #dpg.set_axis_limits('y_axis', -1.25,1.25)
        
        dpg.add_line_series([], [], label="X_value", id="X_value", parent="y_axis")
        dpg.add_line_series([], [], label="Y_value", id="Y_value", parent="y_axis")
        dpg.add_line_series([], [], label="Z_value", id="Z_value", parent="y_axis")


def resize_group( sender, data, user ):
    dpg.configure_item('Ploter', height = data[1], width = data[0] ) 
    dpg.configure_item('Graph', height = data[1]*0.9, width = data[0]*0.9, pos=[ data[0]*0.05, data[1]*0.05] ) 


dpg.setup_viewport()

dpg.set_primary_window     ( main_window, True              )
dpg.set_viewport_min_height( height = 700                   ) 
dpg.set_viewport_min_width ( width  = 800                   ) 
dpg.set_viewport_title     ( title  = 'Ploter Acelerometro' )

dpg.maximize_viewport() 

dpg.add_resize_handler(main_window, callback=resize_group)



x_data, y_data, z_data = 0.0, 0.0, 0.0 

while dpg.is_dearpygui_running():
    dpg.render_dearpygui_frame() 
    count = dpg.get_frame_count() 
    try: 
        pBytes = b''
        while comp.in_waiting: 
            nBytes = comp.inWaiting()
            pBytes += comp.read(nBytes)
            time.sleep(0.001)
            #print(pBytes)

        print( f'[{count} / {len(pBytes)}]: {pBytes}' ) 
        #x_data, y_data, z_data = struct.unpack('fff', pBytes )
        #att_data_run(x_data, y_data, z_data)
    except: 
        pass#print( e, f'. Recebido um buffer de {nBytes}' )
