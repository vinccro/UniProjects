There are 3 folders:
	ESTR 				 ---contains the working program
	Graphics User INterface(GUI) ... ---contains the source and compiled Graphics User Interface that connects to the ESTR.
	Test 				 ---contains the UART and Interrupt tests

In ESTR folder:
	main.c 		---contains the ESTR program
	ringbuf.h 	---contains Texas Instrument librarys of a circular buffer except some functions added on.
	CommonESTR.h    ---contains string format checker(used by the tests)

In Graphics User Interface(GUI) ... folder:
	estr 						---contains a compiled version of the GUI program
	Graphics User Interface(GUI) Source Files	---contains the python source code thats uses pyqt4

In Test folder:
	main.c ---contains the UART tests and Interrupt tests.
	
	
