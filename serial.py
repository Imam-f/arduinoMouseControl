import serial as s
ser = s.Serial("COM7", 115200)

while(True):
    # ser.write("What".encode('utf-8'))
    x = ser.readline()
    print(x)

ser.close()
