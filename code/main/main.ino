#include <XboxSeriesXControllerESP32_asukiaaa.hpp>



#define PWM1_Ch    0
#define PWM2_Ch    1

#define PWM_Res   10
#define PWM_Freq  1000


#define ENA   13
#define ENB   25

#define IN1 12
#define IN2 14
#define IN3 27
#define IN4 26

#define sentido_A 1
#define sentido_B 1


#define dead_zone 0.1 //porcentagem de 0 a 1

// Required to replace with your xbox address
 XboxSeriesXControllerESP32_asukiaaa::Core xboxController("9c:aa:1b:22:8c:b8");
//XboxSeriesXControllerESP32_asukiaaa::Core xboxController; // any xbox controller



int PWM1_DutyCycle = 0;
int PWM2_DutyCycle = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting NimBLE Client");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ENA, OUTPUT);//Definimos o pino 2 (LED) como saída.
  pinMode(IN1, OUTPUT);//Definimos o pino 2 (LED) como saída.
  pinMode(IN2, OUTPUT);//Definimos o pino 2 (LED) como saída.

  pinMode(ENB, OUTPUT);//Definimos o pino 2 (LED) como saída.
  pinMode(IN3, OUTPUT);//Definimos o pino 2 (LED) como saída.
  pinMode(IN4, OUTPUT);//Definimos o pino 2 (LED) como saída.


  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);

  ledcAttachPin(ENA, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM_Freq, PWM_Res);

  ledcAttachPin(ENB, PWM2_Ch);
  ledcSetup(PWM2_Ch, PWM_Freq, PWM_Res);




  
  xboxController.begin();
}

void loop() {
  xboxController.onLoop();
  if (xboxController.isConnected()) {
     digitalWrite(LED_BUILTIN, HIGH);

     
    /*Serial.print(xboxController.xboxNotif.toString());
    unsigned long receivedAt = xboxController.getReceiveNotificationAt();
    uint16_t joystickMax = XboxControllerNotificationParser::maxJoy;
    Serial.print("joyLHori rate: ");
    Serial.println((float) xboxController.xboxNotif.joyLHori / joystickMax);
    Serial.print("joyLVert rate: ");
    Serial.println((float) xboxController.xboxNotif.joyLVert / joystickMax);
    Serial.println("received at " + String(receivedAt));*/

   // Serial.println((float) xboxController.xboxNotif.joyLHori);
    int joyLHori = xboxController.xboxNotif.joyLHori;
    int joyLVert = xboxController.xboxNotif.joyLVert;

    
    if (joyLVert > (32768 * (1 + dead_zone))){ //Frente
      digitalWrite(IN1, sentido_A);
      digitalWrite(IN2, !sentido_A);

      digitalWrite(IN3, sentido_B);
      digitalWrite(IN4, !sentido_B);
      
      PWM1_DutyCycle = map(joyLVert, (32768* (1 + dead_zone)), 65535, 0, 1023);   
      PWM2_DutyCycle = map(joyLVert, (32768* (1 + dead_zone)), 65535, 0, 1023);    
      
    }
    else if (joyLVert < (32768 * (1 - dead_zone))){ //Atras
      digitalWrite(IN1, !sentido_A);
      digitalWrite(IN2, sentido_A);

      digitalWrite(IN3, sentido_B);
      digitalWrite(IN4, !sentido_B);

      
      PWM1_DutyCycle = map(joyLVert, (32768* (1 - dead_zone)), 0, 0, 1023);  
      PWM2_DutyCycle = map(joyLVert, (32768* (1 - dead_zone)), 0, 0, 1023);  
      
    }else if (joyLHori > (32768 * (1 + dead_zone))){ //direita
      digitalWrite(IN1, sentido_A);
      digitalWrite(IN2, !sentido_A);

      digitalWrite(IN3, !sentido_B);
      digitalWrite(IN4, sentido_B);
      
      PWM1_DutyCycle = map(joyLHori, (32768* (1 + dead_zone)), 65535, 0, 1023);   
      PWM2_DutyCycle = map(joyLHori, (32768* (1 + dead_zone)), 65535, 0, 1023);    
      
    }
    else if (joyLHori < (32768 * (1 - dead_zone))){//esquerda
      digitalWrite(IN1, sentido_A);
      digitalWrite(IN2, !sentido_A);

      digitalWrite(IN3, sentido_B);
      digitalWrite(IN4, !sentido_B);

      
      PWM1_DutyCycle = map(joyLHori, (32768* (1 - dead_zone)), 0, 0, 1023);  
      PWM2_DutyCycle = map(joyLHori, (32768* (1 - dead_zone)), 0, 0, 1023);  
      
    }
    else {
      PWM1_DutyCycle = 0  ;
      PWM2_DutyCycle = 0  ;
    }


    
       
    ledcWrite(PWM1_Ch, PWM1_DutyCycle);
    ledcWrite(PWM2_Ch, PWM2_DutyCycle);
        
  } else {
    Serial.println("not connected");
    digitalWrite(LED_BUILTIN, LOW);
  }
 // Serial.println(" at " + String(millis()));
  
  //delay(500);
}
