#include <Servo.h> // this adds the "Servo" library to the program

// the following creates two servo objects
Servo leftMotor;
Servo rightMotor;

void setup()
{
    leftMotor.attach(12); // if you accidentally switched up the pin numbers for your servos, you can swap the numbers here
    rightMotor.attach(13);
}


void loop()
{
  if(digitalRead(2) == HIGH) // this registers when the button is pressed on pin 2 of the Arduino
{
    while(1)
    {
        leftMotor.write(90); // "90" is neutral position for the servos, which tells them to stop turning
        rightMotor.write(90);
    }
}
    leftMotor.write(180); // with continuous rotation, 180 tells the servo to move at full speed "forward."
    rightMotor. write(0); // if both of these are at 180, the robot will go in a circle because the servos are flipped. "0," tells it to move full speed "backward."
}
