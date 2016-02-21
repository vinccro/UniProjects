"""Author ControlHat
    Date 10th of October 2015"""
import serial as s
import serial.tools.list_ports

import thread


def startSerial(baudR, partityR, stopR, byteR):

    ports = list(serial.tools.list_ports.comports())
    print ports
    Cport = None
    for p in ports:
        if "Stellaris" in p[1]:
            Cport = p[0] 
            break
    if Cport == None:
        return 0
        
    Cport = Cport.replace("COM", "")
    
    print Cport

    global ser
    ser = None
    ser = s.Serial(int(Cport)-1, baudR, byteR, partityR, stopR )
    print ser
    return 1

def stopSerial():
    if ser.isOpen():
        ser.close()
        print "Serial Closed"
    else:
        print "Serial Prevously Closed"

def setBaud(baud=9600):
    if ser.isOpen():
        ser.baudrate = baud
    else:
        print "Error Serial port Not OPEN"
        
def setParity(parityp =  s.PARITY_EVEN):
    ser.parity = parityp

def setStopbit(bit = s.STOPBITS_ONE):
    ser.stopbits = bit

def write(letter):
    if ser.isOpen():
        ser.write(letter)
    else:
        print "Error Serial port Not OPEN"

def read():
    if ser.isOpen() == False:
        print "Error Serial port Not OPEN"
        return False
    else:
        holdstring = ""
        #print "getting output"
        while ser.inWaiting() > 0:
            holdstring =  holdstring + ser.read()
        #print "got"
        return holdstring
        
def Sopen():
    if ser.isOpen() == True:
        return 1
    else:
        return 0
