//Video Demo link - https://youtu.be/SDQrIWQ7g5k

#include<Wire.h>

//Slave address
const int MPU6050_addr=0x68;
//Accel meter start address
const int accAddr=0x3B;
//Slave register address contain slave reset bit
const int pwrMgmt1Addr=0x6B;
//LED to indicate position match
const int LED=PIN_LED_13;

// Reference or expected pattern array
int arr [] = {1,2,3};
//Array element variable
int a=0;

//Flag to indicate position change
int cp=0;
//Variable to indicate previous position state
int pp=0;
//Variable to indicate the present position state
int pos=0;
//Variable for number samples (time given for pattern input)
int j; 
//Variable used to store acc reading
int16_t AccX,AccY,AccZ;
//Variable to store acc after precision adjusted
int16_t X=0,Y=0,Z=0;

//Some strings to display messages
String start;
String msg1="Press enter to start"; 
String msg2="To go to initial statement, press enter";

void setup() {
  // put your setup code here, to run once:
  Serial.println(F("Initializing I2C devices..."));
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(pwrMgmt1Addr);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);
  //Waiting for the start 
  Serial.println(msg1);
  while(Serial.available()==0){
    
  }
  start=Serial.readString();
  //for Loop for number of samples
  for(j=0;j<100;j++){
      //Serial.print("Iteration");Serial.println(j);
      Wire.beginTransmission(MPU6050_addr);
      Wire.write(accAddr);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU6050_addr,6,true);
      AccX=Wire.read()<<8|Wire.read();
      AccY=Wire.read()<<8|Wire.read();
      AccZ=Wire.read()<<8|Wire.read();
      X = AccX/100;
      Y = AccY/100;
      Z = AccZ/100;
  
     //Serial.println(pos);
     Serial.print("AccX = "); Serial.print(X);
     Serial.print(" || AccY = "); Serial.print(Y);
     Serial.print(" || AccZ = "); Serial.println(Z);
     
     //if statements to decide position state 
     if((-160<X && X<-140) && (-10<Y && Y<20) && (-10<Z && Z<10)){
         pos = 1;
         Serial.print("position= ");
         Serial.println(pos);
         cp = 0;
         
         //if statement to check current state with previous state
         if (pp != pos){
            cp = 1;
            pp = pos;
         }
      }

      else if((-20<X && X<50) && (-30<Y && Y<0) && (120<Z && Z<200)){
         Serial.println(pos);
         pos = 2;
         Serial.print("position= ");
         Serial.println(pos);
         cp = 0;

         //if statement to check current state with previous state
         if (pp != pos){
          cp = 1;
          pp = pos;
         }
      }
      
      else if((-10<X && X<30) && (-200<Y && Y<-150) && (-20<Z && Z<0)){
         pos = 3;
         Serial.print("position= ");
         Serial.println(pos);
         cp = 0;
         
         //if statement to check current state with previous state
         if (pp != pos){
          cp = 1;
          pp = pos;
         }
      }
      
      else{
        Serial.print("position =");
        Serial.println("current_position");
        cp = 0;
      }

      //Pattern Checking
      if(cp == 1){
          if(pos == arr[a]){

              //Reference array last element matched
              if(a == 2){
                  Serial.println("Pattern matched");
                  digitalWrite(LED, LOW);   
                  Serial.println(msg2);
                  while(Serial.available()==0){
    
                  }
                  break;//To come out of for loop, ie j is 50
              }
              else{
                  Serial.println("position matched");
                  a = a+1;
              }
          }
          //Pattern is not matched condition
          else{
              Serial.println("Pattern not matched");
              while(Serial.available()==0){
    
              }
              break;//To come out of for loop, ie j is 50
          }
      }
      delay(100);
      cp = 0;
  } 
  //Initialize flags and variables
  Serial.println("Time out");
  a = 0;
  pp = 0;
  cp = 0;
  pos = 0;
}
