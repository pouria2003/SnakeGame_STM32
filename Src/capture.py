import serial
import msvcrt

# Change these values as needed
port = 'COM15' # ttyUSB in Linux
baudrate = 9600

# Open the serial port
ser = serial.Serial(port, baudrate)

while 1:
    input_char = msvcrt.getch()
    if input_char == b'\x03': # CTRL + C
        break
        
    print(input_char.upper() + b"\n")
    ser.write(input_char.upper() + b"\n")
