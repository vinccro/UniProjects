/**
 * @Date: 10th of October 2015
 * @Authors: ControlHat
 * @Project Title: Embedded System Test Rig(ESTR)
 * @Project Description: This project tests the capacitabities of the UUT.
 */
/* FreeRTOS header files */
#include "include/FreeRTOS.h"
#include "include/task.h"

/* Stellaris library includes. */
#include "inc\hw_types.h"
#include "inc\hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
#include "inc/hw_ints.h"

#include "driverlib/debug.h"
#include "include/semphr.h"

#include "include/projdefs.h"
/* Circular Buffers for bases of ESTR */
#include "ringbuf.h"
/* Other Created Header files */
#include "CommonESTR.h"

#define BUFFER_SIZE_RIG_INBOX 50
#define BUFFER_SIZE_RIG_OUTBOX 50
#define BUFFER_SIZE_COM_INBOX 50
#define BUFFER_SIZE_COM_OUTBOX 50



/* Background Tasks prototype functions */
void vTaskRig( void *pvParameters ); // reads data from buffer and write to Rig(UUT\UART1)
void vTaskCom( void *pvParameters ); // reads data from buffer and write to Com(Computer\UART0)
void vControl(void *pvParmeters); // reads data from Computer Output buffer and writes to Rig input buffer
									// reads data from Rig Output buffer and writes to Computer input buffer

/* Test Manager */
void vTestManager(void *pvParameters );

/* Test Tasks that implement accounding to test specification */
void vTestStatus( void *pvParameters );
void vTestStatus1( void *pvParameters );
void vTestStatus2( void *pvParameters );

/* Interrupt Service Routines (ISR) */
void vComInterrupt(void);//recieves UART0 input buffer interrupt and writes to Circular Buffer
void vRigInterrupt(void);//recieves UART1 input buffer interrupt and writes to Circular Buffer

/* Fault Functions */
void vTaskFault(void);
void vSemaphoreFault(void);
void vQueueFault(void);
/* Define the strings that will be passed in as the task parameters.  These are
defined const and off the stack to ensure they remain valid when the tasks are
executing. */
const char *pcTextForTask1 = "Task 1 is running\n";
const char *pcTextForTask2 = "Task 2 is running\n";

/* Circular Buffers */
tRingBufObject RigOutBuffer;
tRingBufObject RigInBuffer;
tRingBufObject ComOutBuffer;
tRingBufObject ComInBuffer;
/* Circular Buffer Semaphores */
xSemaphoreHandle semaROutbox;
xSemaphoreHandle semaRInbox;
xSemaphoreHandle semaCOutbox;
xSemaphoreHandle semaCInbox;

/* UART Semaphores */
xSemaphoreHandle semaUART0;
xSemaphoreHandle semaUART1;

/* Circular Test Buffers */
tRingBufObject BufferA;
tRingBufObject BufferB;
tRingBufObject BufferC;
/* Test Buffers Semaphores */
xSemaphoreHandle semaA;
xSemaphoreHandle semaB;
xSemaphoreHandle semaC;

xTaskHandle xHandle;
xTaskHandle xHandletest;
/* Tests Handles */
xTaskHandle xHandletest1;
xTaskHandle xHandletest2;
xTaskHandle xHandletest3;
xTaskHandle xHandletest4;
xTaskHandle xHandletest5;
xTaskHandle xHandletest6;
xTaskHandle xHandletest7;
xTaskHandle xHandletest8;
xTaskHandle xHandletest9;

/* Test Queue */
xQueueHandle xTask;

typedef struct testhold {
   char idd[3];//test id
   char test1;//mirror message TX to UUT
   char test2;//mirror message RX by ESTR
   char test3;//Message Check
   char test4;//test UUT processor
   char test5;//Variable frequency
   char test6;//Random occurring pulse burst
   char test7;//Synchronous events
   char test8;//Asynchronous events
   char test9;//Transponder message
} tests;

/* UART Tests */
char  test1Mark;//U = Unknown; P = Pass; F = Fail
char  test2Mark;
char  test3Mark;
char  test4Mark;
/* Interrupt Test */
char  test5Mark;
char  test6Mark;
char  test7Mark;
char  test8Mark;
char  test9Mark;

/* UART Tests Boolean: states whether tests have finished */
tBoolean test1Done;
tBoolean test2Done;
tBoolean test3Done;
tBoolean test4Done;
/* Interrupt Tests Boolean: states whether tests have finished */
tBoolean test5Done;
tBoolean test6Done;
tBoolean test7Done;
tBoolean test8Done;
tBoolean test9Done;


/*-----------------------------------------------------------*/

//*****************************************************************************
//
//! This functions initialises all necessary stellaris peripherals.
//!
//! \param None.
//!
//! Sets Clock.
//! Enables and configures GPIO port F pin 5 and 7.
//! Enables and configures UART0 and UART1.
//! Enables necessary interrupts.
//! Initialises Buffers.
//! Creates Tasks
//! Creates Queue
//! Creates Semaphores for Buffers
//!
//! \return int
//
//*****************************************************************************
int main( void )
{
	/* Set the clocking to run from the PLL at 50 MHz.  Assumes 8MHz XTAL,
	whereas some older eval boards used 6MHz. */
	SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ );

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_7|GPIO_PIN_5);

    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //
    // Enable processor interrupts.
    //
   // IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                        		UART_CONFIG_PAR_EVEN));
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                        		UART_CONFIG_PAR_EVEN));
    //
    // Enable the UART interrupt.
    //
	//UARTFIFOLevelSet(UART0_BASE,UART_FIFO_TX4_8 , UART_FIFO_RX4_8 );
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);

	//UARTFIFOEnable(UART0_BASE);

	UARTIntRegister(UART0_BASE, vComInterrupt);
	UARTIntRegister(UART1_BASE, vRigInterrupt);
    IntEnable(INT_UART0);
    IntEnable(INT_UART1);

	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7,0x00 );
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5,0xFF );


    unsigned char RigBuffI[BUFFER_SIZE_RIG_INBOX];
    RingBufInit(&ComInBuffer, RigBuffI, sizeof(RigBuffI) );

    unsigned char RigBuffO[BUFFER_SIZE_RIG_OUTBOX];
    RingBufInit(&ComOutBuffer, RigBuffO, sizeof(RigBuffO) );

    unsigned char ComBuffI[BUFFER_SIZE_COM_INBOX];
    RingBufInit(&RigInBuffer, ComBuffI, sizeof(ComBuffI) );

    unsigned char ComBuffO[BUFFER_SIZE_COM_OUTBOX];
    RingBufInit(&RigOutBuffer, ComBuffO, sizeof(ComBuffO) );


    unsigned char BufferABank[50];
    RingBufInit(&BufferA, BufferABank, sizeof(BufferABank) );

    unsigned char BufferBBank[50];
    RingBufInit(&BufferB, BufferBBank, sizeof(BufferBBank) );

    unsigned char BufferCBank[50];
    RingBufInit(&BufferC, BufferCBank, sizeof(BufferCBank) );

    /* Creation of the Background Tasks */
    const signed char* RigName = "Rig Communicator Task";
	if( xTaskCreate( vTaskRig, RigName, 240, (void*)pcTextForTask2, 4, NULL ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
		//printf("%s FAILED\n", RigName);
		vTaskFault();
	}

    const signed char* ComName = "Computer Communicator Task";
	if( xTaskCreate( vTaskCom, ComName, 240, (void*)pcTextForTask2, 3, NULL )  == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
		//printf("%s FAILED\n", ComName);
		vTaskFault();
	}

    const signed char* ControlName = "Control Task";
	if( xTaskCreate( vControl, ControlName, 240, (void*)pcTextForTask2, 1, NULL ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
		//printf("%s FAILED\n", ControlName);
		vTaskFault();
	}

	/* Creation of the Test Manager Task */
    const signed char* TestManagerName = "Test Manager Task";
	if( xTaskCreate( vTestManager, TestManagerName, 240, (void*)pcTextForTask2, 2, &xHandle )  == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
		//printf("%s FAILED\n", TestManagerName);
		vTaskFault();
	}





	xTask = xQueueCreate(5,sizeof(tests) );
	if(xTask == NULL){
		//printf("error Queue\n");
		vQueueFault();
	}

	vSemaphoreCreateBinary(semaCOutbox);
	if (semaCOutbox == NULL){
		//printf("semaphore Error\n");
		vSemaphoreFault();
	}

	vSemaphoreCreateBinary(semaCInbox);
	if (semaCInbox == NULL){
		//printf("semaphore Error\n");
		vSemaphoreFault();
	}

	vSemaphoreCreateBinary(semaROutbox);
	if (semaROutbox == NULL){
		//printf("semaphore Error\n");
		vSemaphoreFault();
	}

	vSemaphoreCreateBinary(semaRInbox);
	if (semaRInbox == NULL){
		//printf("semaphore Error\n");
		vSemaphoreFault();
	}

	vSemaphoreCreateBinary(semaUART0);
	if (semaROutbox == NULL){
		//printf("semaphore Error\n");
		vSemaphoreFault();
	}

	vSemaphoreCreateBinary(semaUART1);
	if (semaRInbox == NULL){
		//printf("semaphore Error\n");
		vSemaphoreFault();
	}
	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();	
	
	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//*****************************************************************************
//
//! Buffer updater: used to muliple tests working concurrently (NOT OPERATIONAL)
//!
//! \param None.
//!
//! The purpose of this function is due to limitations of the Circular Buffers. Since Circular Buffers(ringbuf.h) only allow
//! to peak of read one item at a time. Muliple tests might need to access same data from UUT leading to corruption. So the purpose of
//! this task, is copy the data from UUT to multiple circular buffers. Thus each test can have unaltered data from the UUT.
//!
//! \return None.
//
//*****************************************************************************
void updateBuffers(void){
	unsigned char message;

			message = RingBufReadOne(&RigOutBuffer);
			if(xSemaphoreTake(semaCInbox, 1/portTICK_RATE_MS) == 1){
				RingBufWriteOne(&ComInBuffer, message);
			}xSemaphoreGive(semaCInbox);

			if(xSemaphoreTake(semaA, 1/portTICK_RATE_MS) == 1){
				if( RingBufFree(&BufferA) == 0) RingBufReadOne(&BufferA);
				RingBufWriteOne(&BufferA, message);
			}xSemaphoreGive(semaA);

			if(xSemaphoreTake(semaB, 1/portTICK_RATE_MS) == 1){
				if( RingBufFree(&BufferB) == 0) RingBufReadOne(&BufferB);
				RingBufWriteOne(&BufferB, message);
			}xSemaphoreGive(semaB);

			if(xSemaphoreTake(semaC, 1/portTICK_RATE_MS) == 1){
				if( RingBufFree(&BufferC) == 0) RingBufReadOne(&BufferC);
				RingBufWriteOne(&BufferC, message);
			}xSemaphoreGive(semaC);
}


//*****************************************************************************
//
//! This creates and manages all the tests tasks.
//!
//! \param *pvParameters
//!
//! If item in Test Queue it will start the test else it will wait one second.
//! Resets tests back to default settings. Then flushes buffers and starts test tasks according to
//! the queue item. Waits intill tests tasks are completed and after transmitters tests results back
//! to the computer(UART0). Tests are char 'U' unknown, 'F' Failed and 'P' Passed. idd are test queue item identification.
//!
//! \return None.
//
//*****************************************************************************
void vTestManager( void *pvParameters )
{
	tests bank;
	 char SendToMessenger[] = "<            >";
//	printf("Testmanager\n");
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
//		printf("Starting testmanager\n");
		if( xQueueReceive( xTask, &bank, 0) != errQUEUE_EMPTY){
			//printf("starting TEST");

			test1Done = true;
			test2Done = true;
			test3Done = true;
			test4Done = true;

			test5Done = true;
			test6Done = true;
			test7Done = true;
			test8Done = true;
			test9Done = true;

			/* UART Tests */
			test1Mark = 'U';//U = Unknown; P = Pass; F = Fail
			test2Mark = 'U';
			test3Mark = 'U';
			test4Mark = 'U';
			/* Interrupt Test */
			test5Mark = 'U';
			test6Mark = 'U';
			test7Mark = 'U';
			test8Mark = 'U';
			test9Mark = 'U';

		//	SendToMessenger = "<UUUUUUUUU>";
			SendToMessenger[1] = 'U';
			SendToMessenger[2] = 'U';
			SendToMessenger[3] = 'U';
			SendToMessenger[4] = 'U';
			SendToMessenger[5] = 'U';
			SendToMessenger[6] = 'U';
			SendToMessenger[7] = 'U';
			SendToMessenger[8] = 'U';
			SendToMessenger[9] = 'U';

			//flush buffers
			if(xSemaphoreTake(semaCInbox, 10/portTICK_RATE_MS) == 1){
				RingBufFlush(&ComInBuffer);
			}xSemaphoreGive(semaCInbox);

			if(xSemaphoreTake(semaRInbox, 10/portTICK_RATE_MS) == 1){
				RingBufFlush(&RigInBuffer);
			}xSemaphoreGive(semaRInbox);

			if(xSemaphoreTake(semaCOutbox, 10/portTICK_RATE_MS) == 1){
				RingBufFlush(&ComOutBuffer);
			}xSemaphoreGive(semaCOutbox);

			if(xSemaphoreTake(semaROutbox, 10/portTICK_RATE_MS) == 1){
				RingBufFlush(&RigOutBuffer);
			}xSemaphoreGive(semaROutbox);

			//////////////////////////////////////////////UART TESTS
			/* Mirror message TX to UUT */
			if( bank.test1 == 'a'){
				test1Done = false;
				const signed char* Test1Name = "Mirror message TX to UUT";
				if( xTaskCreate( vTestStatus, Test1Name, 240, (void*)pcTextForTask2, 2, &xHandletest1 ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
					//printf("%s FAILED\n", Test1Name);
					vTaskFault();
				}
			}
			/*Mirror message RX by ESTR*/
			if( bank.test2 == 'a'){
				test2Done = false;
				const signed char* Test2Name = "Mirror message RX by ESTR";
				if( xTaskCreate( vTestStatus, Test2Name, 240, (void*)pcTextForTask2, 2, &xHandletest2 ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
					//printf("%s FAILED\n", Test2Name);
					vTaskFault();
				}
			}
			/*Message Check*/
			if( bank.test3 == 'a'){
				test3Done = false;
				const signed char* Test3Name = "Message Check";
				if( xTaskCreate( vTestStatus, Test3Name, 240, (void*)pcTextForTask2, 2, &xHandletest3 ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
					//printf("%s FAILED\n", Test3Name);
					vTaskFault();
				}
			}
			/*UUT processor clock Variation*/
			if( bank.test4 == 'a'){
				test4Done = false;
				const signed char* Test4Name = "UUT processor clock Variation";
				if( xTaskCreate( vTestStatus1, Test4Name, 240, (void*)pcTextForTask2, 2, &xHandletest4 ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
					//printf("%s FAILED\n", Test4Name);
					vTaskFault();
				}
			}
			///////////////////////////////////Interrupt TESTS
			/*Variable Frequency*/
			if( bank.test5 == 'a'){
				test5Done = false;
				const signed char* Test5Name = "Variable Frequency";
				if( xTaskCreate( vTestStatus, Test5Name, 240, (void*)pcTextForTask2, 2, &xHandletest5 ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
					//printf("%s FAILED\n", Test5Name);
					vTaskFault();
				}
			}
			/*Randomly occurring 2 or 3 pulse*/
			if( bank.test6 == 'a'){
				test6Done = false;
				const signed char* Test6Name = "Random 2 or 3 pulse";
				if( xTaskCreate( vTestStatus, Test6Name, 240, (void*)pcTextForTask2, 2, &xHandletest6 ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
					//printf("%s FAILED\n", Test6Name);
					vTaskFault();
				}
			}
			/*Generate Synchronous Events*/
			if( bank.test7 == 'a'){
				test7Done = false;
				const signed char* Test7Name = "Genrate Synchronous Events";
				if( xTaskCreate( vTestStatus, Test7Name, 240, (void*)pcTextForTask2, 2, &xHandletest7 ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
					//printf("%s FAILED\n", Test7Name);
					vTaskFault();
				}
			}
			/*Generate Asynchronous Events*/
			if( bank.test8 == 'a'){
				test8Done = false;
				const signed char* Test8Name = "Generate Asynchronous Events";
				if( xTaskCreate( vTestStatus, Test8Name, 240, (void*)pcTextForTask2, 2, &xHandletest8 ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
					//printf("%s FAILED\n", Test8Name);
					vTaskFault();
				}
			}
			/*Transponder Message */
			if( bank.test9 == 'a'){
				test9Done = false;
				const signed char* Test9Name = "Transponder message";
				if( xTaskCreate( vTestStatus, Test9Name, 240, (void*)pcTextForTask2, 2, &xHandletest9 ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){
					//printf("%s FAILED\n", Test9Name);
					vTaskFault();
				}
			}

			//this waits intill all tests are completed
			while(test1Done == false || test2Done == false || test3Done == false || test4Done == false || test5Done == false || test6Done == false || test7Done == false || test8Done == false || test9Done == false){
				vTaskDelay(10/portTICK_RATE_MS);
			}
			//This Transmits test results back to the computer
			if(xSemaphoreTake(semaCInbox, 5/portTICK_RATE_MS) == 1){

				SendToMessenger[1] = bank.idd[0];
				SendToMessenger[2] = bank.idd[1];
				SendToMessenger[3] = bank.idd[2];
				SendToMessenger[4] = test1Mark;
				SendToMessenger[5] = test2Mark;
				SendToMessenger[6] = test3Mark;
				SendToMessenger[7] = test4Mark;
				SendToMessenger[8] = test5Mark;
				SendToMessenger[9] = test6Mark;
				SendToMessenger[10] = test7Mark;
				SendToMessenger[11] = test8Mark;
				SendToMessenger[12] = test9Mark;
				RingBufWrite(&ComInBuffer, &SendToMessenger,14 );
			}xSemaphoreGive(semaCInbox);
			//printf("Finishing TEST\n");
		}
		vTaskDelay(1000/portTICK_RATE_MS);
	}
}
//*****************************************************************************
//
//! Basic Status test (Used for testing purposes)
//!
//! \param *pvParameters
//!
//! This is a Basic test, sends 's' to UUT and checks reception of message using commonESTR.h method FSM
//!
//! \return None.
//
//*****************************************************************************
void vTestStatus( void *pvParameters )
{
	unsigned char message;
	unsigned countt = 0;
	char hold[38];
	int istrue = 0;
	char hold1[] = "ST as:&&&&&& t:&&&&& v:&&&&& alt:&&&&&";//38ST as:     0 t:  438 v:  671 alt:26001
	while(1){

				if(xSemaphoreTake(semaROutbox, 8/portTICK_RATE_MS) == 1){
					//sends 's' to UUT
					if(xSemaphoreTake(semaRInbox, 8/portTICK_RATE_MS) == 1){
						RingBufWriteOne(&RigInBuffer, 's');
					}xSemaphoreGive(semaRInbox);
					//reads Status message
					vTaskDelay(1/portTICK_RATE_MS);
						while( countt < 38){

							if( RingBufEmpty(&RigOutBuffer) != 1){
								message = RingBufReadOne(&RigOutBuffer);

								if(message == 'S' || istrue == 1){
									hold[countt++] = message;
									istrue = 1;
								}

							}else{

								vTaskDelay(10/portTICK_RATE_MS);
							}
						}
				}xSemaphoreGive(semaROutbox);
				//checks if test was successful
					if (FSM(hold1, hold) == 1){
						test3Mark = 'P';
					}else{
						test3Mark = 'F';
					}

			test3Done = true;
			//printf("test3 success %c\n", test3Mark);
			vTaskDelete(NULL);
		}
}
//*****************************************************************************
//
//! Basic test does NOTHING.
//!
//! \param *pvParameters
//!
//! Does some delays
//!
//! \return None.
//
//*****************************************************************************
void vTestStatus1( void *pvParameters )
{
	unsigned countt = 0;
	while(1){
		if(xSemaphoreTake(semaRInbox, 10/portTICK_RATE_MS) == 1){
			RingBufWriteOne(&RigInBuffer, 's');
		}
		xSemaphoreGive(semaRInbox);
		//if(xSemaphoreTake(semaA, 1/portTICK_RATE_MS) == 1){
			while( countt++ < 10000);
			vTaskDelay(10/portTICK_RATE_MS);
			while( countt++ < 10000);

		//}xSemaphoreGive(semaA);
		while( countt++ < 10000);
		vTaskDelay(10/portTICK_RATE_MS);
		while( countt++ < 10000);
		vTaskDelay(10/portTICK_RATE_MS);		//printf("test1 starting\n");
		while( countt++ < 10000);

		if (2 == 1){
			test4Mark = 'P';
		}else{
			test4Mark = 'F';
		}

		test4Done = true;
		//printf("test3 success %c\n", test4Mark);
		vTaskDelete(NULL);

	}//end while
}
//*****************************************************************************
//
//! This handles commands from the computer
//!
//! \param None.
//!
//! The commands from the computer are represented in < commmand > format
//! if command is <reset> flushes buffers and sets a pin low then raises high to reset UUT
//! if command is <Transponder> raises a pin from low to high, used to trigger Transponder message on UUT
//! if command is <#idd_____________> will put in to test queue.
//!
//! \return None.
//
//*****************************************************************************
void vControlCommand(void){
	unsigned char cmd[20];
	unsigned char message;
	unsigned int count= 0;
	tests store;

	while(RingBufEmpty(&ComOutBuffer) != 1){
		message = RingBufReadOne(&ComOutBuffer);
		if( message == '>') break;
		cmd[count++] = message;
	}
	//printf(":::%s:::\n", cmd);
	if( strcmp(cmd, "reset8") == 0){
		//flush buffers
		if(xSemaphoreTake(semaCInbox, 10/portTICK_RATE_MS) == 1){
			RingBufFlush(&ComInBuffer);
		}xSemaphoreGive(semaCInbox);

		if(xSemaphoreTake(semaRInbox, 10/portTICK_RATE_MS) == 1){
			RingBufFlush(&RigInBuffer);
		}xSemaphoreGive(semaRInbox);

		if(xSemaphoreTake(semaCOutbox, 10/portTICK_RATE_MS) == 1){
			RingBufFlush(&ComOutBuffer);
		}xSemaphoreGive(semaCOutbox);

		if(xSemaphoreTake(semaROutbox, 10/portTICK_RATE_MS) == 1){
			RingBufFlush(&RigOutBuffer);
		}xSemaphoreGive(semaROutbox);

		//set UUT reset LOW
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5,0x00 );
		int delay = 0;
		while(delay++ < 20000 );
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5,0xFF );

		if(xSemaphoreTake(semaCInbox, 10/portTICK_RATE_MS) == 1){
			RingBufWrite(&ComInBuffer, "  RESET DONE  ", sizeof("  RESET DONE  "));
		}xSemaphoreGive(semaCInbox);
		return;
	}
	if( strcmp(cmd,"Transponder")  == 0){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7,0xFF );
		int delay = 0;
		while(delay++ < 2000 );
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7,0x00 );
		return;
	}
	if( cmd[0]  == '#'){

		//printf("%s", cmd);
		store.idd[0] = cmd[1];
		store.idd[1] = cmd[2];
		store.idd[2] = cmd[3];

		store.test1 = cmd[4];
		store.test2 = cmd[5];
		store.test3 = cmd[6];
		store.test4 = cmd[7];
		store.test5 = cmd[8];
		store.test6 = cmd[9];
		store.test7 = cmd[10];
		store.test8 = cmd[11];
		store.test9 = cmd[12];

		//printf("added test id %d to queue\n", hold1);
		xQueueSendToBack(xTask, &store, 0);
	}

}
//*****************************************************************************
//
//! Acts as a simple relay and send infomation between UUT and ESTR.
//!
//! \param *pvParameters
//!
//! This function first reads RigOutBuffer and writes to ComInBuffer
//! Next this function reads ComOutBuffer and writes to RigInBuffer
//! this implements every 50 milliseconds
//!
//! \return None.
//
//*****************************************************************************
void vControl(void *pvParameters ){
	unsigned char message;
	while(1){
/* Reads from UUT and writes to Computer */
		if(xSemaphoreTake(semaROutbox, 10/portTICK_RATE_MS) == 1){
			if(xSemaphoreTake(semaCInbox, 10/portTICK_RATE_MS) == 1){
				if(xSemaphoreTake(semaUART1, 10/portTICK_RATE_MS) == 1){
				while(RingBufEmpty(&RigOutBuffer) != 1){
					message = RingBufReadOne(&RigOutBuffer);
					//printf("vControl to Computer : %c\n", message);
					RingBufWriteOne(&ComInBuffer, message);
				}
				xSemaphoreGive(semaUART1);
				}
				xSemaphoreGive(semaCInbox);
			}
			xSemaphoreGive(semaROutbox);
		}
/* Reads from computer and Writes to UUT*/
		if(xSemaphoreTake(semaCOutbox, 10/portTICK_RATE_MS) == 1){
			if(xSemaphoreTake(semaRInbox, 10/portTICK_RATE_MS) == 1){
				while(RingBufEmpty(&ComOutBuffer) != 1){
					message = RingBufReadOne(&ComOutBuffer);
					/* if recieves commands implements vControlCommand */
					if( message == '<') vControlCommand();
					else RingBufWriteOne(&RigInBuffer, message);
				}
				xSemaphoreGive(semaRInbox);
			}
			xSemaphoreGive(semaCOutbox);
		}
		vTaskDelay(50/portTICK_RATE_MS);
	}
}
//*****************************************************************************
//
//! Reads from RigInBuffer and writes to the UART1.
//!
//! \param *pvParameters
//!
//! Every 10 milliseconds it checks if ComInbuffer isn't empty  and writes ComInbuffer items to UART0
//!
//! \return The byte read from the ring buffer.
//
//*****************************************************************************
void vTaskRig( void *pvParameters )
{
	unsigned char message;
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		if(xSemaphoreTake(semaRInbox, 10/portTICK_RATE_MS) == 1){
			if(xSemaphoreTake(semaUART1, 10/portTICK_RATE_MS) == 1){
					while(RingBufEmpty(&RigInBuffer) != 1){
						message = RingBufReadOne(&RigInBuffer);
						while(UARTBusy(UART1_BASE) == 1){
							vTaskDelay(1/portTICK_RATE_MS);
						//	printf("waiting2\n");
						}
						UARTCharPut(UART1_BASE, message);
					}
			xSemaphoreGive(semaUART1);
		}
			xSemaphoreGive(semaRInbox);
		}
		vTaskDelay(10/portTICK_RATE_MS);
	}
}
//*****************************************************************************
//
//! Reads from ComInBuffer and writes to the UART0.
//!
//! \param *pvParameters
//!
//! Every 10 milliseconds it checks if ComInbuffer isn't empty  and writes ComInbuffer items to UART0
//!
//! \return None.
//
//*****************************************************************************
void vTaskCom( void *pvParameters )
{
	unsigned char message;
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		if(xSemaphoreTake(semaCInbox, 10/portTICK_RATE_MS) == 1){
			while(RingBufEmpty(&ComInBuffer) != 1){
				message = RingBufReadOne(&ComInBuffer);
				while(UARTBusy(UART0_BASE) == 1){
					vTaskDelay(1/portTICK_RATE_MS);
					//printf("waiting1\n");
				}
				UARTCharPut(UART0_BASE, message);
			}
			xSemaphoreGive(semaCInbox);
		}
		vTaskDelay(10/portTICK_RATE_MS);
	}
}
//*****************************************************************************
//
//! Interrupt Service Routine (ISR) to recieve interrupts from the UART1(UUT), and store them into the Rig(UUT) Output Buffer(RigOutBuffer)
//!
//! \param None.
//!
//! When a UART1 interrupt occurs, the vRigInterrupt will read from UART1 buffer and write to RigOutBuffer(type: Circular Buffer).
//!
//! \return None.
//
//*****************************************************************************
void vRigInterrupt(void){
    unsigned long ulStatus;
	unsigned char bank;
    //
    // Get the interrrupt status.
    //
    ulStatus = UARTIntStatus(UART1_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART1_BASE, ulStatus);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(UARTCharsAvail(UART1_BASE))
    {

    	bank = UARTCharGetNonBlocking(UART1_BASE);
    	RingBufWriteOne(&RigOutBuffer, bank);
    }
}
//*****************************************************************************
//
//! Interrupt Service Routine (ISR) to recieve interrupts from the UART0, and store them into the Computer Output Buffer(ComOutBuffer)
//!
//! \param None.
//!
//! When a UART0 interrupt occurs, the vComInterrupt will read from UART0 buffer and write to ComOutBuffer(type: Circular Buffer).
//!
//! \return None.
//
//*****************************************************************************
void vComInterrupt(void){
    unsigned long ulStatus;
	unsigned char bank;

    //
    // Get the interrrupt status.
    //
    ulStatus = UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART0_BASE, ulStatus);

    //
    // Loop while there are characters in the receive FIFO.
    //

    while(UARTCharsAvail(UART0_BASE))
    {
    	bank = UARTCharGetNonBlocking(UART0_BASE);
    	RingBufWriteOne(&ComOutBuffer, bank);
    }
}
void vSemaphoreFault(void){
	/* Semaphore creation fault has occurred */
	for(;;);
}
void vTaskFault(void){
	/* Task creation fault has occurred */
	for(;;);
}
void vQueueFault(void){
	/* Queue creation fault has occurred */
	for(;;);
}
void vApplicationMallocFailedHook( void )
{
	/* This function will only be called if an API call to create a task, queue
	or semaphore fails because there is too little heap RAM remaining. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	/* This function will only be called if a task overflows its stack.  Note
	that stack overflow checking does slow down the context switch
	implementation. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* This example does not use the idle hook to perform any processing. */
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This example does not use the tick hook to perform any processing. */
}


