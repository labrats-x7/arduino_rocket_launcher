# import libraries for serial port and Tkinter GUI
import serial
import tkinter as tk

BUTTON_COLOR = 'green'

speed_mode = False
 
# Open serial port
ser = serial.Serial('COM3', baudrate=9600, timeout=1)
 
# Create the root window
root = tk.Tk()
root.geometry('400x400')
root.title("RocketLauncher")


def write_command(pos):  #manipuliert ein bit in eienem byte
    output = 1 << pos
    if speed_mode:
        output |= 1
    print("Output:", output)
    ser.write([output])
    
def cmd_up(e):       
    write_command(4)
        
def cmd_down(e):     #Bit 3
    write_command(3)

def cmd_left(e):     #Bit 1
    write_command(1)

def cmd_right(e):    #Bit 2
    write_command(2)

def cmd_fire(e):     #Bit 5
    write_command(5)

def cmd_stop(e):
    write_command(0)
	
def cmd_spd(e):      #Bit 0
    global speed_mode
    if speed_mode:
        speed_mode = False
    else:
        speed_mode = True
    print("speed_mode:", speed_mode)

    
# Create a keystroke handler
def key(event):
    if (event.char == 'q'):
        print("quit")
        root.quit

root.bind("<KeyPress-w>", cmd_up)
root.bind("<KeyPress-s>", cmd_down)
root.bind("<KeyPress-a>", cmd_left)
root.bind("<KeyPress-d>", cmd_right)
root.bind("<KeyPress-f>", cmd_fire)
root.bind("<KeyRelease>", cmd_stop)



#Create a label with instructions
#label = tkinter.Label(root, text='Press 0-9 to send a number, or "q" to quit')
#label.pack(fill=tkinter.BOTH, expand=1)
#label.grid(row=0, column=1)
root.bind('<Key>', key)
#label.focus_set()


btn_up = tk.Button(root,
                   text="UP",
                   fg=BUTTON_COLOR,
                   command=cmd_up)        

btn_down = tk.Button(root,
                     text="DOWN",
                     fg=BUTTON_COLOR,
                     command=cmd_down)

btn_left = tk.Button(root,
                     text="LINKS",
                     fg=BUTTON_COLOR,
                     command=cmd_left)

btn_right = tk.Button(root,
                      text="RIGHT",
                      fg=BUTTON_COLOR,
                      command=cmd_right)

btn_fire = tk.Button(root,
                      text="FIRE",
                      fg=BUTTON_COLOR,
                      command=cmd_fire)

cbx_spd = tk.Checkbutton(root,
                         command=cmd_spd)

btn_up.grid(row=0, column=1)
btn_down.grid(row=2, column=1)
btn_left.grid(row=1, column=0)
btn_right.grid(row=1, column=2)
btn_fire.grid(row=3, column=3)
cbx_spd.grid(row=1, column=1)

#btn_up.bind('<ButtonPress-1>',cmd_up(1))
#btn_up.bind('<ButtonRelease-1>',cmd_up(0))




# Hand over to the Tkinter event loop
root.mainloop()
 
# Close serial port
ser.close()
