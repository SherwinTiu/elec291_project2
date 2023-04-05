/*#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 9); // RX, TX

void setup() {
  Serial.begin(9600); // Serial monitor
  BTSerial.begin(9600); // Bluetooth module
}

void loop() {
  if (BTSerial.available()) {
    char c = BTSerial.read();
    Serial.write(c);
  }
  if (Serial.available()) {
    char c = Serial.read();
    BTSerial.write(c);
  }
}
*/
/*
#define ledPin 7
int state = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(38400); // Default communication rate of the Bluetooth module
}

void loop() {
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port
 }

 if (state == '0') {
  digitalWrite(ledPin, LOW); // Turn LED OFF
  Serial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
  state = 0;
 }
 else if (state == '1') {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED: ON");;
  state = 0;
 } 
}
*/
/*
#include <SoftwareSerial.h>
//Using softwareSerial as the hardware one cause BT port being busy when connected to processing
SoftwareSerial BTSerial(10, 11); // RX | TX

String incomingString; // for incoming serial data
int action;

void setup() {
  BTSerial.begin(115200);

  //Comment out below if not printing to arduino serial monitor
  //Serial.begin(115200); // opens serial port, sets data rate to 9600 bps
   
  //while (!Serial) {
    //; // wait for serial port to connect. Needed for Native USB only
  //}
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // send data only when you receive data:
  if (BTSerial.available() > 0) {
    
    // read the incoming byte:
    incomingString = BTSerial.readString();

    // print string to Arduino software monitor
    //Serial.print("String received: ");
    //Serial.println(incomingString);

    // convert the incoming to action code
    // all incoming is hard coded in processing
    action = incomingString.toInt();
        
switch (action) {
 
  case 1:
    //do something when var equals 1
    Serial.println("Turn ON LED");
    digitalWrite(LED_BUILTIN, HIGH);
    break;
    
  case 2:
    //do something when var equals 2
    Serial.println("Turn OFF LED");
    digitalWrite(LED_BUILTIN, LOW);
    break;
    
  default:
    // if nothing else matches, do the default
    // default is optional
    Serial.println("!! Not sure what you want");
    break;
}
  }
}
*/


// =============================================================================


//#include <SoftwareSerial.h>


/*Code to control an LED via bluetooth through a phone app.
Project  by MukeshArvindh. Code by MukeshArvindh.*/

/*If you are going to copy and  paste the code, then do not forget 
to delete the void setup() and void loop()  function from the 
sketch you are using before doing so,as the functions already
exist  in this sketch. Copy-pasting the comments will not
cause any changes in the outcome  or code.*/

/*Note:This code has been compiled and checked multiple times,  and has 
proven to be accurate. The product also works as intended.*/

//Bluetooth  uses serial communication. So, we use many serial functions 
//in this sketch.  


// const int LED = 7;

// char switchstate;
// /*declaring that  there is a variable called switchstate, which will 
// hold a character value. This  is due to programming of the app, which
// will send  a text value to arduino. If  we use 'int' instead of 
// 'char' the code will not work properly.*/
// void setup()  {//Here the code only runs once.
// Serial.begin(9600);
// /*To start serial communication  at a rate of 9600 bits per second. This  
// is the default rate anyways.*/
// pinMode(LED,  OUTPUT);
// //Declaring that the LED is an output.
// pinMode(10, INPUT);
// pinMode(11, OUTPUT);
// }
// void loop() {//This code  repeats. This is our main code.
// while(Serial.available()>0){ 
  
// switchstate = digitalRead(10);

// //Serial.read() is to read the value coming from  app.
// Serial.print(switchstate);
// //This will print the value onto the Serial  monitor.
// Serial.print("\
// ");
// //This moves to the next line after every  new line printed.
// delay(15);
// /*Gives a break of 15 milliseconds. Delay is  for human eye, and for
// speed of some computers, as some will crash at high speeds.*/
// if(switchstate  == '1'){//Checking if the value from app is '1'
//  digitalWrite(LED, HIGH);
//  //If  it is, write the component on pin 5(LED) high. 
// }
// else if(switchstate == '0'){//Else,  if the vaue from app is '0',
//  digitalWrite(LED, LOW);//Write the component on pin  5(LED) low.
// }
// }
// }


#define LED 12


char Incoming_value = 0;
                
void setup() 
{
  Serial.begin(9600);         
  pinMode(LED, OUTPUT);       
}

void loop()
{
  if(Serial.available() > 0)  
  {
    Incoming_value = Serial.read();      
    Serial.print(Incoming_value);        
    Serial.print("\n");        
    if(Incoming_value == '1')             
      digitalWrite(LED, HIGH);  
    else if(Incoming_value == '0')       
      digitalWrite(LED, LOW);   
  }                

} 

