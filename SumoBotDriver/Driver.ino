 

/*========================================CONSTANTS AND DEFINITIONS=============================================*/
//Direction of robot movement
const unsigned int  cui_FORWARD     = 1;
const unsigned int  cui_BACKWARD    = 2;
const unsigned int  cui_SHARP_LEFT  = 3;
const unsigned int  cui_SHARP_RIGHT = 4;
const unsigned int  cui_F_LEFT      = 5;
const unsigned int  cui_F_RIGHT     = 6;
const unsigned int  cui_B_LEFT      = 7;
const unsigned int  cui_B_RIGHT     = 8;
const unsigned int  cui_NEITHER     = 0;


//Direction of wheel movement
const unsigned int  cui_pwmSTOP     = 127;
const unsigned int  cui_pwmFRONT    = 220; /* The motors were slowed down. The max value is 240 */
const unsigned int  cui_pwmBACK     = 20;  /* The motors were slowed down. The max value is 0 */


/*---------------- Pins ----------------------*/
//Analog pins for bridge
const unsigned int cui_digRightMotors =3;
const unsigned int cui_digLeftMotors = 5;
const unsigned int cui_pwmLeftMotors = 9;
const unsigned int cui_pwmRightMotors =6;

//Line sensor Pins
const unsigned int cui_LSFR = 7;
const unsigned int cui_LSFL = 4;
const unsigned int cui_LSBR = 2;
const unsigned int cui_LSBL = 8;


//IR sensor pins
const unsigned int cui_IRF = 1;
const unsigned int cui_IRR = 0;
const unsigned int cui_IRL = 2;

#define DISTANCE_BOUNDARIES 82


//Location of the enemy relative to the robot

bool b_isClockwise = false;

unsigned int g_uiPaintValue        =   500;
unsigned int g_uiWhiteBorder       =   HIGH;
#define LINE_SENSOR_WAIT 10

/*==================================SETUP AND INITIALIZATION=========================================*/
void setup() {
  pinMode(cui_pwmLeftMotors, OUTPUT);
  pinMode(cui_pwmRightMotors, OUTPUT);
  pinMode(cui_digRightMotors,OUTPUT);
  pinMode(cui_digLeftMotors, OUTPUT);
}

/*=======================================MAIN FUNCTION===============================================*/
void loop() 
{
  digitalWrite(cui_digRightMotors, HIGH);
  digitalWrite(cui_digLeftMotors, HIGH);
    /* -----------------------Check if robot is crossing the line---------------------------------*/

  /*Front Left or Right Line Sensors*/
  
if(LineDetection(cui_LSFR) || LineDetection(cui_LSFL) )
  {
      if(LineDetection(cui_LSFR) && LineDetection(cui_LSFL))
          {
            motorDrive(cui_BACKWARD,cui_pwmRightMotors, cui_pwmLeftMotors);
          }
      else if(LineDetection(cui_LSFR))
          {
            motorDrive(cui_BACKWARD,cui_pwmRightMotors, cui_pwmLeftMotors);
          }
      else if(LineDetection(cui_LSFL))
          {
            motorDrive(cui_BACKWARD ,cui_pwmRightMotors, cui_pwmLeftMotors);
          }
  }
  
  //Back Left or Right Line Sensors
  else if   (LineDetection(cui_LSBR) || LineDetection(cui_LSBL) ) 
  {
      if (LineDetection(cui_LSBR) && LineDetection(cui_LSBL))
          {
            motorDrive(cui_FORWARD,cui_pwmRightMotors, cui_pwmLeftMotors);
          }
          
      else if   (LineDetection(cui_LSBR))
          {
          motorDrive(cui_F_LEFT,cui_pwmRightMotors, cui_pwmLeftMotors);   
          }
        
      else if   (LineDetection(cui_LSBL))
          {
          motorDrive(cui_F_RIGHT,cui_pwmRightMotors, cui_pwmLeftMotors);   
          }
  }
  
  //Right Side Sensors
  else if   (LineDetection(cui_LSBR) || LineDetection(cui_LSFR))
  {
    motorDrive(cui_F_RIGHT,cui_pwmRightMotors, cui_pwmLeftMotors);
  }
  
    //Left Side Sensors
  else if   (LineDetection(cui_LSBR) || LineDetection(cui_LSFR))
  {
    motorDrive(cui_F_LEFT,cui_pwmRightMotors, cui_pwmLeftMotors);
  }
  
  //-----------------------------------------Check if enemy robot is detected---------------------------//
  
  if   (IsWithinDistance(cui_IRF))
  {
    motorDrive(cui_FORWARD,cui_pwmRightMotors, cui_pwmLeftMotors);
            
    if   (LineDetection(cui_LSFR) || LineDetection(cui_LSFL))
    {
    motorDrive(cui_SHARP_LEFT,cui_pwmRightMotors, cui_pwmLeftMotors);   
    }
  }
  
  else if   (IsWithinDistance(cui_IRR))
  {
    b_isClockwise = true;
    motorDrive(cui_F_RIGHT,cui_pwmRightMotors, cui_pwmLeftMotors);
  }
  else if   (IsWithinDistance(cui_IRL))
  {
    b_isClockwise = false; 
    motorDrive(cui_F_LEFT,cui_pwmRightMotors, cui_pwmLeftMotors);
  }
  else 
  {
    if(b_isClockwise)
      motorDrive(cui_SHARP_RIGHT,cui_pwmRightMotors, cui_pwmLeftMotors);
    else
      motorDrive(cui_SHARP_LEFT,cui_pwmRightMotors, cui_pwmLeftMotors);
  }
}

/*========================================CYTRON MDD10A H BRIDGE============================================*/

/*========================================FUNCTION DEFINITIONS==============================================*/

/*************************************************************************************************************
Function Name: motorDrive
Parameters: ui_direction    desired movement of the robot
            ui_FRMotor      Front right motor's pwm pin in the motor's driver
            ui_FLMotor      Front left motor's pwm pin in the motor's driver
            ui_BRMotor      Back right motor's pwm pin in the motor's driver
            ui_BLMotor      Back left motor's pwm pin in the motor's driver

Return value: boolean, true if successful false otherwise
Comments: -
*************************************************************************************************************/
bool motorDrive(
          unsigned int ui_direction,
          unsigned int ui_RightMotors,
          unsigned int ui_LeftMotors)
{
  switch(ui_direction)
  {
    case cui_FORWARD:
      analogWrite(ui_RightMotors, cui_pwmBACK);
      analogWrite(ui_LeftMotors, cui_pwmFRONT);
      break;

    case cui_BACKWARD:
      analogWrite(ui_RightMotors, cui_pwmFRONT);
      analogWrite(ui_LeftMotors, cui_pwmBACK);
      break;

    case cui_SHARP_RIGHT:
      analogWrite(ui_RightMotors, cui_pwmFRONT);
      analogWrite(ui_LeftMotors, cui_pwmFRONT);
      break;

    case cui_SHARP_LEFT:
      analogWrite(ui_RightMotors, cui_pwmBACK);
      analogWrite(ui_LeftMotors, cui_pwmBACK);
      break;

    case cui_F_RIGHT:
      analogWrite(ui_RightMotors, cui_pwmSTOP);
      analogWrite(ui_LeftMotors, cui_pwmFRONT);
      break;

    case cui_F_LEFT:
      analogWrite(ui_RightMotors, cui_pwmBACK);
      analogWrite(ui_LeftMotors, cui_pwmSTOP);
      break;

    case cui_B_RIGHT:
      analogWrite(ui_RightMotors, cui_pwmSTOP);
      analogWrite(ui_LeftMotors, cui_pwmBACK);
      break;

    case cui_B_LEFT:
      analogWrite(ui_RightMotors, cui_pwmFRONT);
      analogWrite(ui_LeftMotors, cui_pwmSTOP);
      break;

    default:
      return false;
  }
  return true;
}
/*========================================IRSENSOR==========================================================*/
/*__________________________________________________________________________________________________________*/
/* The minimum distance the ir's have to detect will be at 0.5 Volts (ie: about 2cm. Thus, an analog value
 *  over 102 is acceptable.
 *  The maximum distance the ir's have to detect will be at 0.5 Volts as well (ie: about 140cm). This is due
 *  to the physical properties of the sensor.
 It is reccommended to look at the GP2Y0A02 datasheet for further consultation on why this value was taken.*/
//========================================FUNCTION DEFINITIONS==============================================


/*************************************************************************************************************
Function Name:LineDetection


Parameters:   in uiAnalogPin        The analog pin connected to the IR sensor


Return value: true if an object is within the specifed boundaries, false otherwise


Comments:     The global variable DISTANCE_BOUNDARIES can be modified to change the sensitivity.
*************************************************************************************************************/
bool IsWithinDistance(
                    unsigned int uiAnalogPin)
{
    unsigned int uiAvgDistance     = 0;
    unsigned int uiRead1           = 0;
    unsigned int uiRead2           = 0;
    unsigned int uiRead3           = 0;
    int          iDiffCompare1     = 0;
    int          iDiffCompare2     = 0;
    int          iDiffCompare3     = 0;


    uiRead1 = analogRead (uiAnalogPin);
    uiRead2 = analogRead (uiAnalogPin);
    uiRead3 = analogRead (uiAnalogPin);

    //Get all possible combinations of differences between reads
    iDiffCompare1 = abs(uiRead1 - uiRead2);
    iDiffCompare2 = abs(uiRead1 - uiRead3);
    iDiffCompare3 = abs(uiRead2 - uiRead3);

    //Check which has the smallest difference
    if      (iDiffCompare1 <= iDiffCompare2    &&
            iDiffCompare1 <= iDiffCompare3)
    {
        uiAvgDistance = (uiRead1 + uiRead2)/2;
    }

    else if (iDiffCompare2 <= iDiffCompare1    &&
            iDiffCompare2 <= iDiffCompare3)
    {
        uiAvgDistance = (uiRead1 + uiRead3)/2;
    }

    else if (iDiffCompare3 <= iDiffCompare1    &&
            iDiffCompare3 <= iDiffCompare2)
    {
        uiAvgDistance = (uiRead2 + uiRead3)/2;
    }

    //Return true if within specified boundaries
    if ( uiAvgDistance > DISTANCE_BOUNDARIES )
    {
        return true;
    }

    //false otherwise
    else
    {
        return false;
    }
}

/*========================================LINE SENSOR==========================================================*/
/*_____________________________________________________________________________________________________________*/

/*========================================FUNCTION DEFINITIONS==============================================*/


/*************************************************************************************************************
Function Name:LineDetection


Parameters:   in uiLineSensorIO        Sensor to read from


Return value: true if white border is detected, false otherwise


Comments:     The global variable g_uiPaintValue can be modified to change the sensitivity.
*************************************************************************************************************/

bool LineDetection (unsigned int uiLineSensorIO)
{
  /*Initiate */
  unsigned int uiSensorValue         =   LOW;
  
  /*Send pulse to the sensor*/
  pinMode(uiLineSensorIO,OUTPUT);
  digitalWrite(uiLineSensorIO,HIGH);

  /*Wait and read value from sensor*/
  delayMicroseconds(LINE_SENSOR_WAIT);
  pinMode(uiLineSensorIO,INPUT);

  /*Start "timer"*/
  long liTimeStart = micros();

  /*Detect the amount of time it takes for the capacitor to charge
  //If the sensor is exposed to a lot of light, it will discharge at a high rate
  //and output HIGH causing the time difference to be max value.*/
  while (digitalRead(uiLineSensorIO) == g_uiWhiteBorder && (micros() - liTimeStart < 3000) );

  /*Detect amount of time since start */ 
  unsigned int uiTimeDiff = micros() - liTimeStart;
  uiSensorValue = uiTimeDiff;

  /* If the sensor is exposed to a lot of light, return true, otherwise false */
  if(uiTimeDiff > g_uiPaintValue)
  {
      return false;
  }
  else if(uiTimeDiff <= g_uiPaintValue)
  {
    return true;
  }
}
