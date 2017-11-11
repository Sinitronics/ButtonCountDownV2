int SER_Pin = 8;   //pin 14 on the 75HC595
int RCLK_Pin = 9;  //pin 12 on the 75HC595
int SRCLK_Pin = 10; //pin 11 on the 75HC595

//How many of the shift registers - change this
#define number_of_74hc595s 2 

//do not touch
#define numOfRegisterPins number_of_74hc595s * 8

boolean registers[numOfRegisterPins];

//declare constant pins
const int ledPin = 13;
const int buttonPin = 3;
const int camTrigger = 4;
const int camWake = 5;

int buttonState = 0; // Declare button state variable.

void setup(){
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // initialize the camera triggers as outputs:
  pinMode(camTrigger, OUTPUT);
  pinMode(camWake, OUTPUT);

  //reset all register pins
  clearRegisters();
  writeRegisters();
}               


//set all register pins to LOW
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 


//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void writeRegisters(){

  digitalWrite(RCLK_Pin, LOW);

  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);

}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}

//Declare multidimentianal number array
int num[10][8] = {  {1,2,3,5,6,7},    //0
                    {1,7},            //1
                    {2,3,4,6,7},      //2
                    {1,2,4,6,7},      //3
                    {1,4,5,7},        //4
                    {1,2,4,5,6},      //5
                    {1,2,3,4,5,6},    //6
                    {1,6,7},          //7
                    {1,2,3,4,5,6,7},  //8
                    {1,2,4,5,6,7}     //9
                  };

void loop(){

  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH){
  
    // set LED pin low
    digitalWrite(ledPin, LOW);
    // wakeup camera
    digitalWrite(camWake, HIGH);
    delay(100);
    digitalWrite(camWake, LOW);
    
    for(int t = 4; t >= 0; t-- ){
   
      for(int s = 5; s >= 0; s-- )  {
    
        for(int i = 0; i <= 7; i++ ){
    
        setRegisterPin(num[s][i], HIGH);
        setRegisterPin(0, LOW);
    
        setRegisterPin(num[t][i]+8, HIGH);
        setRegisterPin(0+8, LOW);
        
        writeRegisters(); 
        }

       clearRegisters();
     
       //check if s == 0 and trigger camera to take photo
       if(s == 0){
         delay(250);
         digitalWrite(camTrigger, HIGH);
         delay(100);
         digitalWrite(camTrigger, LOW);
       }
       
       delay(1000);
       
      }
  
    }
    
  } else {
      
    digitalWrite(ledPin, HIGH);
    
  }
  
}
