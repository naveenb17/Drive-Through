#include<Servo.h>
Servo servo_1; 
Servo servo_2;

//US 1
#define trigPin_1 3   //setting trigPin for ultrasonic distance sensor
#define echoPin_1 2    //setting echoPin for ultrasonic distance sensor

//US 2
#define trigPin_2 7   //setting trigPin for ultrasonic distance sensor
#define echoPin_2 6    //setting echoPin for ultrasonic distance sensor


float duration, distance;
int pos = 0;  
int servoPin_1 = 13;
int servoPin_2 = 8;

//For PIR
int pir_1 = 12;    //For PIR 1
int pir_2 = 11;    //For PIR 2
int pir_3 = 10;    //For PIR 3
int pir_4 = 9;     //For PIR 4

//to read input from IR
int pir_pin_1 = LOW, pir_pin_2 = LOW, pir_pin_3 = LOW, pir_pin_4 = LOW;
 
int led_r = A0;  //Menu item A
int led_b = A1;	 //Menu item B
int led_g = A2;  //Menu item C
int led_y = A3;  //Menu item D

/* using this bool checker so that the exit barricade function only works
after an item has been chosen from the menu*/
bool checker = false;

void openBarricade()
{
  //Make servo go to 90 degrees 
  servo_1.write(90); 
  delay(4000);  //open barricade for 3secs
      
  //Make servo go to 0 degrees 
  servo_1.write(0); 
}

void closeBarricade()
{
  //Make servo go to 90 degrees 
  servo_2.write(90); 
  delay(4000);  //open barricade for 3secs
      
  //Make servo go to 0 degrees 
  servo_2.write(0); 
}

void menuSelect()
{
    pir_pin_1 = digitalRead(pir_1);
  	pir_pin_2 = digitalRead(pir_2);
  	pir_pin_3 = digitalRead(pir_3);
  	pir_pin_4 = digitalRead(pir_4);
    delay(500);
  	//To light up Red LED if MENU 1 is chosen
  	if(pir_pin_1 == HIGH){
    	digitalWrite(led_r, HIGH);
  		digitalWrite(led_b, LOW);
    	digitalWrite(led_g, LOW);
  		digitalWrite(led_y, LOW);
      	checker = true;
  	}
    //To light up Blue LED if MENU 2 is chosen
  	else if(pir_pin_2 == HIGH){
    	digitalWrite(led_r, LOW);
  		digitalWrite(led_b, HIGH);
    	digitalWrite(led_g, LOW);
  		digitalWrite(led_y, LOW);
        checker = true;
  	}
    //To light up green LED if MENU 3 is chosen
  	else if(pir_pin_3 == HIGH){
    	digitalWrite(led_r, LOW);
  		digitalWrite(led_b, LOW);
    	digitalWrite(led_g, HIGH);
  		digitalWrite(led_y, LOW);
      	checker = true;
  	}
    //To light up yellow LED if MENU 4 is chosen
  	else if(pir_pin_4 == HIGH){
    	digitalWrite(led_r, LOW);
  		digitalWrite(led_b, LOW);
    	digitalWrite(led_g, LOW);
  		digitalWrite(led_y, HIGH);
        checker = true;
  	}
}

void setup()
{
  Serial.begin(9600);
  
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  
  servo_1.attach(servoPin_1);
  servo_1.write(pos);     //Setting the barricade to be closed initially
  servo_2.attach(servoPin_2);
  servo_2.write(pos);     //Setting the barricade to be closed initially
  
  pinMode(pir_1, INPUT);  //input from IR1
  pinMode(pir_2, INPUT);  //input from IR2
  pinMode(pir_3, INPUT);  //input from IR3
  pinMode(pir_4, INPUT);  //input from IR4
  pinMode(led_r, OUTPUT); //Menu item A
  pinMode(led_b, OUTPUT); //Menu item B
  pinMode(led_g, OUTPUT); //Menu item C
  pinMode(led_y, OUTPUT); //Menu item D
}

void loop()
{
  /*Use a flag so that the menuSelect function only runs when the 
  barricade has been opned once*/
  int flag = 0; 
  
  //Setting all LED's to LOW 
  digitalWrite(led_r, LOW);
  digitalWrite(led_b, LOW);
  digitalWrite(led_g, LOW);
  digitalWrite(led_y, LOW);
  
  //Using Ultrasonic sensor to detect a car coming through
  digitalWrite(trigPin_1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_1, HIGH); //sending pulse from trigPin
  delayMicroseconds(10);
  digitalWrite(trigPin_1,LOW);
  duration = pulseIn(echoPin_1, HIGH); //recieving duration of pulse in echoPin
  distance = duration*0.034/2;   //distance from sensor
  Serial.println(distance);
  
  //Condition for opening barricade
  if(distance <= 150)
   { 
    openBarricade();
    flag = 1;
    //after car moves on to window 1
     delay(2000);
   }
  
  //to select menu and light the LED corresponding to chosen menu
  if(flag == 1)
  {
    while(!checker)
    {
      menuSelect();
    }
    delay(1000);
    
    /*this while loop ensures that the code doesnt go back to the top of void loop
    before the car has exited*/
    while(checker){
    	//Using Ultrasonic sensor to detect a car coming through
  		digitalWrite(trigPin_2, LOW);
  		delayMicroseconds(2);
  		digitalWrite(trigPin_2, HIGH); //sending pulse from trigPin
  		delayMicroseconds(10);
  		digitalWrite(trigPin_2,LOW);
  		duration = pulseIn(echoPin_2, HIGH); //recieving duration of pulse in echoPin
  		distance = duration*0.034/2;   //distance from sensor
    
    	//Condition for opening barricade for car to exit
  		if(distance <= 150)
   		{ 
   			closeBarricade();
    		flag = 0; 
            checker = false;
    		delay(1000);  
      	}
    }
   }
  
  //reseting all PIR pins to low
  pir_pin_1 = LOW;
  pir_pin_2 = LOW;
  pir_pin_3 = LOW;
  pir_pin_4 = LOW;
 }