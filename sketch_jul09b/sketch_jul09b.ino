#include <Servo.h>

Servo leftMotor;
Servo rightMotor;

const int serialPeriod = 250;       // this limits output to the console to once every 1/4 second
unsigned long timeSerialDelay = 0;

const int loopPeriod = 20;          // this sets how often the sensor takes a reading to 20ms, which is a frequency of 50Hz
unsigned long timeLoopDelay   = 0;

// this assigns the TRIG and ECHO functions to the pins on the Arduino. Make adjustments to the numbers here if you connected differently
const int ultrasonic2TrigPin = 8;
const int ultrasonic2EchoPin = 9;

int ultrasonic2Distance;
int ultrasonic2Duration;

// this defines the two possible states for the robot: driving forward or turning left
#define DRIVE_FORWARD     0
#define TURN_LEFT         1

int state = DRIVE_FORWARD; // 0 = drive forward (DEFAULT), 1 = turn left

void setup()
{
    Serial.begin(9600);
  
    // these sensor pin configurations
    pinMode(ultrasonic2TrigPin, OUTPUT);
    pinMode(ultrasonic2EchoPin, INPUT);
    
    // this assigns the motors to the Arduino pins
    leftMotor.attach(12);
    rightMotor.attach(13);
}


void loop()
{
    if(digitalRead(2) == HIGH) // this detects the kill switch
    {
        while(1)
        {
            leftMotor.write(90);
            rightMotor.write(90);
        }
    }

    debugOutput(); // this prints debugging messages to the serial console
    
    if(millis() - timeLoopDelay >= loopPeriod)
    {
        readUltrasonicSensors(); // this instructs the sensor to read and store the measured distances
        
        stateMachine();
        
        timeLoopDelay = millis();
    }
}


void stateMachine()
{
    if(state == DRIVE_FORWARD) // if no obstacles detected
    {
        if(ultrasonic2Distance > 6 || ultrasonic2Distance < 0) // if there's nothing in front of the robot. ultrasonicDistance will be negative for some ultrasonics if there is no obstacle
        {
            // drive forward
            rightMotor.write(180);
            leftMotor.write(0);
        }
        else //  if there's an object in front of us
        {
            state = TURN_LEFT;
        }
    }
    else if(state == TURN_LEFT) // if an obstacle is detected, turn left
    {
        unsigned long timeToTurnLeft = 500; // it takes around .5 seconds to turn 90 degrees. You may need to adjust this if your wheels are a different size than the example
        
        unsigned long turnStartTime = millis(); // save the time that we started turning

        while((millis()-turnStartTime) < timeToTurnLeft) // stay in this loop until timeToTurnLeft has elapsed
        {
            // turn left, remember that when both are set to "180" it will turn.
            rightMotor.write(180);
            leftMotor.write(180);
        }
        
        state = DRIVE_FORWARD;
    }
}


void readUltrasonicSensors()
{
    // this is for ultrasonic 2. You may need to change these commands if you use a different sensor.
    digitalWrite(ultrasonic2TrigPin, HIGH);
    delayMicroseconds(10);                  // keeps the trig pin high for at least 10 microseconds
    digitalWrite(ultrasonic2TrigPin, LOW);
    
    ultrasonic2Duration = pulseIn(ultrasonic2EchoPin, HIGH);
    ultrasonic2Distance = (ultrasonic2Duration/2)/29;
}

// the following is for debugging errors in the console.
void debugOutput()
{
    if((millis() - timeSerialDelay) > serialPeriod)
    {
        Serial.print("ultrasonic2Distance: ");
        Serial.print(ultrasonic2Distance);
        Serial.print("cm");
        Serial.println();
        
        timeSerialDelay = millis();
    }
}
