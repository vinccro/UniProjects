"""Author ControlHat
    Date 10th of October 2015"""


import sys
import comSer
import re

from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import QTimer
from rig import Ui_MainWindow

global idd

def f():
    raw0= comSer.read()
    if type(raw0) == type(True):
        return
    print raw0
    self.rawoutput.setPlainText(raw0)
class Main(QtGui.QMainWindow, Ui_MainWindow):
    def __init__(self):
        QtGui.QMainWindow.__init__(self)
        self.setupUi(self)

        self.lcdstcputemp.display(345)
        raw =" "
        #timers
        global timer
        timer = QTimer(self)
        timer.timeout.connect(self.f)

        global statusTimer
        statusTimer = QTimer(self)
        statusTimer.timeout.connect(self.statusClicked)

        global TransTimer
        TransTimer = QTimer(self)
        TransTimer.timeout.connect(self.TransponderClicked)

        #one shot timers
        global batlowTimer
        batlowTimer = QTimer(self)
        global vib42Timer
        vib42Timer = QTimer(self)
        global thrustTimer
        thrustTimer = QTimer(self)
        global deicingTimer
        deicingTimer = QTimer(self)
        global comp54Timer
        comp54Timer = QTimer(self)
        
        #buttons
        self.statusBut.clicked.connect(self.statusClicked)       
        self.EmergencyBut.clicked.connect(self.emergencyClicked)
        self.resetEBut.clicked.connect(self.resetEmergencyClicked)
        self.OFBut.clicked.connect(self.OscFUPClicked)
        self.OFDBut.clicked.connect(self.OscFDOWNClicked)
        self.OCUBut.clicked.connect(self.OscCUPClicked)
        self.OCDBut.clicked.connect(self.OscCDOWNClicked)
        self.pushButton_2.clicked.connect(self.resetUUT)
        self.Transponder.clicked.connect(self.TransponderClicked)
        self.QueueAdd.clicked.connect(self.AddToQueue)
        #terminal
        self.ConnectBut.clicked.connect(self.ConnectClicked)
        self.DisconnectBut.clicked.connect(self.disConnectClicked)
        self.TerminalSend.clicked.connect(self.TerminalSendClicked)
        self.getOutputBut.clicked.connect(self.getOutputClicked)
        self.terminalClearBut.clicked.connect(self.clearClicked)
        self.DisconnectBut.setVisible(False);

        self.checkBox_5.toggled.connect(self.statusCont)
        self.comboBox.currentIndexChanged[QtCore.QString].connect(self.bits)

        self.checkBox_11.toggled.connect(self.statusContT)
        self.comboBox_3.currentIndexChanged[QtCore.QString].connect(self.bitsT)

        self.label.setStyleSheet("""color : black; font: 75 28pt "Segoe Script";""")
        #print dir(self.rawoutput.verticalScrollBar())
        self.rawoutput.verticalScrollBar().setValue(self.rawoutput.verticalScrollBar().maximum())

    def AddToQueue(self):
        """This reads test parameters and sends test parameters to ESTR"""
        #test 1
        if self.mirrorMessage.isChecked():
            test1 = True
        else:
            test1 = False
        #test 2
        if self.mirrorMessageRx.isChecked():
            test2 = True
        else:
            test2 = False
        #test 3
        if self.MessageCheck.isChecked():
            test3 = True
        else:
            test3 = False
        #test 4
        if self.ClockVar.isChecked():
            test4 = True
        else:
            test4 = False
        #test 5
        if self.VarFreq.isChecked():
            test5 = True
        else:
            test5 = False
        #test 6
        if self.PulseB.isChecked():
            test6 = True
        else:
            test6 = False
        #test 7
        if self.SynEvents.isChecked():
            test7 = True
        else:
            test7 = False
        #test 8
        if self.AsynEvents.isChecked():
            test8 = True
        else:
            test8 = False
        #test 9
        if self.TransMessage.isChecked():
            test9 = True
        else:
            test9 = False

        #test 1
        self.mirrorMessage.setCheckState(False)
        #test 2
        self.mirrorMessageRx.setCheckState(False)
        #test 3
        self.MessageCheck.setCheckState(False)
        #test 4
        self.ClockVar.setCheckState(False)
        #test 5
        self.VarFreq.setCheckState(False)
        #test 6
        self.PulseB.setCheckState(False)
        #test 7
        self.SynEvents.setCheckState(False)
        #test 8
        self.AsynEvents.setCheckState(False)
        #test 9
        self.TransMessage.setCheckState(False)
        global idd
        global name
        if test1 or test2 or test3 or test4 or test5 or test6 or test7 or test8 or test9:
            print idd
            name = str(idd) + "\n" + name
            #self.queueList.setText(name)
            stringidd = str(idd)
            idd = idd + 1
            
            self.testid.setStyleSheet("""font: 20pt "Poor Richard";""")
            self.testid.setText(stringidd)
            bank = ""
            
            if len( stringidd ) > 3:
                print "error"
                return
            if len( stringidd ) == 0:
                print "errro idd"
                return
            if len( stringidd ) == 2:
                stringidd = '0' + stringidd
            if len( stringidd ) == 1:
                stringidd = '0' + '0' + stringidd

            bank = stringidd    
            
            if test1:
                bank = bank + 'a'
            else:
                bank = bank + 'b'

            if test2:
                bank = bank + 'a'
            else:
                bank = bank +'b'

            if test3:
                bank = bank + 'a'
            else:
                bank = bank + 'b'

            if test4:
                bank = bank + 'a'
            else:
                bank = bank + 'b'

            if test5:
                bank = bank + 'a'
            else:
                bank = bank +'b'

            if test6:
                bank = bank +'a'
            else:
                bank = bank +'b'

            if test7:
                bank = bank +'a'
            else:
                bank = bank +'b'

            if test8:
                bank = bank +'a'
            else:
                bank = bank + 'b'

            if test9:
                bank = bank + 'a'
            else:
                bank = bank + 'b'

            print bank
            comSer.write('<' + '#' + bank + '>')
            #self.tableWidget.setItem(0,1,self.QTableWidget +)

    def TransponderClicked(self):
        comSer.write("<Transponder>")
    def bitsT(self, item):
        if self.checkBox_11.isChecked()== True:
            TransTimer.start(int(item))
        else:
            TransTimer.stop()
    def statusContT(self):
        print "toogle"
        if self.checkBox_11.isChecked()== True:
            print "true"
            time = self.comboBox_3.currentText()
            TransTimer.start(int(time))
        else:
            print "false"
            TransTimer.stop()
    def bits(self, item):
        if self.checkBox_5.isChecked()== True:
            statusTimer.start(int(item))
        else:
            statusTimer.stop()
    def statusCont(self):
        print "toogle"
        if self.checkBox_5.isChecked()== True:
            print "true"
            time = self.comboBox.currentText()
            statusTimer.start(int(time))
        else:
            print "false"
            statusTimer.stop()
    def resetUUT(self):
        comSer.write("<reset>")
    def statusClicked(self):
        #print 's'
        comSer.write('s')

    def emergencyClicked(self):
        comSer.write('e')

    def batlowTimerf(self):
        self.batlow.setStyleSheet("""background-color: rgb(255, 255, 255);""")
    def vib42Timerf(self):
        self.vib42.setStyleSheet("""background-color: rgb(255, 255, 255);""")
    def thrustTimerf(self):
        self.thrust.setStyleSheet("""background-color: rgb(255, 255, 255);""")
    def deicingTimerf(self):
        self.deicing.setStyleSheet("""background-color: rgb(255, 255, 255);""")
    def comp54Timerf(self):
        self.comp54.setStyleSheet("""background-color: rgb(255, 255, 255);""")
    def resetEmergencyClicked(self):
        comSer.write('r')
    def OscFUPClicked(self):
        comSer.write('+')
    def OscFDOWNClicked(self):
        comSer.write('-')
    def OscCUPClicked(self):
        comSer.write('u')
    def OscCDOWNClicked(self):
        comSer.write('d')

    
    
    def f(self):
        """This looks for emergency commands in message and changes colours of labels"""
        global raw
        global results
        
        holds= comSer.read()
        print holds
        if type(holds) == type(True):
            return
        #print raw0
        if holds == "":
            return
        message = holds
        if message.find("ERR:E4") != -1:
            self.batlow.setStyleSheet("background-color: rgb(255, 0, 0)")
            batlowTimer.singleShot( 400, self.batlowTimerf)
        if message.find("ERR:13") != -1:
            self.vib42.setStyleSheet("background-color: rgb(255, 0, 0)")
            vib42Timer.singleShot( 400, self.vib42Timerf)
        if message.find("ERR:6") != -1:
            self.thrust.setStyleSheet("background-color: rgb(255, 0, 0)")
            thrustTimer.singleShot( 400, self.thrustTimerf)
        if message.find("ERR:9") != -1:
            self.deicing.setStyleSheet("background-color: rgb(255, 0, 0)")
            deicingTimer.singleShot( 400, self.deicingTimerf)
        if message.find("ERR:42") != -1:
            self.comp54.setStyleSheet("background-color: rgb(255, 0, 0)")
            comp54Timer.singleShot( 400, self.comp54Timerf)
    
        m = re.search('<(.+?)>', holds)  
        if m:
            found = m.group(1)
            results = results + "\n" + found
       

        
        self.queueList.setText(results)
        if len(raw) > 400:
            raw = " "
        raw = raw + holds
        self.rawoutput.setPlainText(raw)
        self.rawoutput.verticalScrollBar().setValue(self.rawoutput.verticalScrollBar().maximum()-3)
    
    def clearClicked(self):
        raw = ""
        raw0 = ""
        self.rawoutput.setPlainText("")
    def getOutputClicked(self):
        global raw0
        raw0= comSer.read()
        if type(raw0) == type(True):
            returnsetStyleSheet
        #print raw0
        self.rawoutput.setPlainText(raw0)
        
    def TerminalSendClicked(self):            
        terminalstring = self.terminal.text()
        print terminalstring
        comSer.write(terminalstring.toAscii())
        
    def disConnectClicked(self):
        """Disconnects from ESTR and sets back all to default values"""
        comSer.stopSerial()
        if comSer.Sopen() == 0:
            self.ConnectBut.setVisible(True);
            self.DisconnectBut.setVisible(False);
            self.label.setStyleSheet("""color : black; font: 75 28pt "Segoe Script";""")
            self.label_12.clear()
            self.label_18.clear()
            self.label_19.clear()
            self.label_20.clear()
            self.BaudCombo.setVisible(True);
            self.BytesizeCombo.setVisible(True);
            self.ParityCombo.setVisible(True);
            self.StopbitsCombo.setVisible(True);
            timer.stop()
        #self.ONOFFLabel.setText("<html><head/><body><p align=\"center\"><span style=\" font-size:24pt; color:#ff0000;\">OFF</span></p></body></html>")

    def ConnectClicked(self):
        """Connects to ESTR and sets serial parameters"""
        self.testid.setStyleSheet("""font: 20pt "Poor Richard";""")
        self.testid.setText(str(idd))
        R = self.BaudCombo.currentText()
        baudR = int(R)
        
        p = self.ParityCombo.currentText()
        if p == "Even":
            partityR = comSer.s.PARITY_EVEN
        elif p == "None":
            partityR = comSer.s.PARITY_NONE
        elif p == "Odd":
            partityR = comSer.s.PARITY_ODD
        elif p == "Mark":
            partityR = comSer.s.PARITY_MARK
        elif p == "Space":
            partityR = comSer.s.PARITY_SPACE
        
        sss = self.StopbitsCombo.currentText()
        if sss == "1":
            stopR = comSer.s.STOPBITS_ONE
        elif sss == "1.5":
            stopR = comSer.s.STOPBITS_ONE_POINT_FIVE
        elif sss == "2":
            stopR = comSer.s.STOPBITS_TWO

        
        byR = self.BytesizeCombo.currentText()
        if byR == "Eight Bits":
            byteR = comSer.s.EIGHTBITS
        elif byR == "Seven Bits":
            byteR = comSer.s.SEVENBITS
        elif byR == "Six Bits":
            byteR = comSer.s.SIXBITS
        elif byR == "Five Bits":
            byteR = comSer.s.FIVEBITS
            
        comSer.startSerial(baudR, partityR, stopR, byteR)
        if comSer.Sopen() == 1:
            self.ConnectBut.setVisible(False);
            self.DisconnectBut.setVisible(True);
            self.label.setStyleSheet("""color : green; font: 75 28pt "Segoe Script";""")
            self.label_12.setText( R)
            self.label_12.setStyleSheet("""font: 14pt "Poor Richard";""")
            self.label_18.setText(byR)
            self.label_18.setStyleSheet(""" font: 14pt "Poor Richard";""")
            self.label_19.setText(p)
            self.label_19.setStyleSheet(""" font: 14pt "Poor Richard";""")
            self.label_20.setText(sss)
            self.label_20.setStyleSheet("""font: 14pt "Poor Richard";""")
            self.BaudCombo.setVisible(False);
            self.BytesizeCombo.setVisible(False);
            self.ParityCombo.setVisible(False);
            self.StopbitsCombo.setVisible(False);
            timer.start(300)
            
        #self.ONOFFLabel.setText("<html><head/><body><p align=\"center\"><span style=\" font-size:24pt; color:#00ff00;\">ON</span></p></body></html>")
                
if __name__ == "__main__":
    idd = 1
    global raw
    global name
    name = ""
    results = ""
    raw = ""
    app = QtGui.QApplication(sys.argv)
    window = Main()
    window.show()
    sys.exit(app.exec_())
