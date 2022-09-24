#include <XboxSeriesXControllerESP32_asukiaaa.hpp>



#define PWM1_Ch    0
#define PWM2_Ch    1

#define PWM_Res   10
#define PWM_Freq  1000


//PONTE H 1
#define EN1A  13
#define IN1A  12
#define IN2A  14
#define IN3A  27
#define IN4A  26
#define EN1B  25

//PONTE H 2
#define EN2A  19
#define IN1B  18
#define IN2B  5
#define IN3B  17
#define IN4B  16
#define EN2B  4

#define sentido_1A 1
#define sentido_1B 1

#define sentido_2A 1
#define sentido_2B 1


#define buzzerPin 15

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
  pinMode(EN1A, OUTPUT); // PONTE H 
  pinMode(IN1A, OUTPUT); // PONTE H 
  pinMode(IN2A, OUTPUT); // PONTE H 
  pinMode(IN3A, OUTPUT); // PONTE H 
  pinMode(IN4A, OUTPUT); // PONTE H 
  pinMode(EN1B, OUTPUT); // PONTE H 
  
  pinMode(EN2A, OUTPUT); // PONTE H 
  pinMode(IN1B, OUTPUT); // PONTE H 
  pinMode(IN2B, OUTPUT); // PONTE H 
  pinMode(IN3B, OUTPUT); // PONTE H 
  pinMode(IN4B, OUTPUT); // PONTE H 
  pinMode(EN2B, OUTPUT); // PONTE H  


  digitalWrite(IN1A, 0);
  digitalWrite(IN2A, 0);
  digitalWrite(IN3A, 0);
  digitalWrite(IN4A, 0);
 

  digitalWrite(IN1B, 0);
  digitalWrite(IN2B, 0);
  digitalWrite(IN3B, 0);
  digitalWrite(IN4B, 0);


  ledcAttachPin(EN1A, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM_Freq, PWM_Res);

  ledcAttachPin(EN1B, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM_Freq, PWM_Res);


  ledcAttachPin(EN2A, PWM2_Ch);
  ledcSetup(PWM2_Ch, PWM_Freq, PWM_Res);
  
  ledcAttachPin(EN2B, PWM2_Ch);
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
    int joyLHori = xboxController.xboxNotif.joyLHori - 32767;
    int joyLVert = xboxController.xboxNotif.joyLVert - 32767;
    int trigRT = xboxController.xboxNotif.trigRT;

    int aceleracao = (int) sqrt((sq(joyLHori)+sq(joyLVert)));
    int diferencial = joyLHori;
 

    //Serial.print(joyLHori);
    //Serial.print("---");
    //Serial.print(joyLVert);
    //Serial.print("---");
    //Serial.println(trigRT);




       if (aceleracao >32767)
        aceleracao = 32767;

      





    
    
    if (trigRT > (1023 * dead_zone)){ //FREIO
      Serial.println(trigRT);
      digitalWrite(IN1A, 1);
      digitalWrite(IN2A, 1);

      digitalWrite(IN3A, 1);
      digitalWrite(IN4A, 1);

      digitalWrite(IN1B, 1);
      digitalWrite(IN2B, 1);

      digitalWrite(IN3B, 1);
      digitalWrite(IN4B, 1);

      

      
    }
    else if (joyLVert > 32768 * dead_zone){ //Frente
      digitalWrite(IN1A, sentido_1A);
      digitalWrite(IN2A, !sentido_1A);

      digitalWrite(IN3A, sentido_2A);
      digitalWrite(IN4A, !sentido_2A);

      digitalWrite(IN1B, sentido_1B);
      digitalWrite(IN2B, !sentido_1B);

      digitalWrite(IN3B, sentido_2B);
      digitalWrite(IN4B, !sentido_2B);


      if (diferencial > 32768 * dead_zone){ //direita
        PWM1_DutyCycle = map(aceleracao, 0, 32767, 0, 1023);   
        PWM2_DutyCycle = map(aceleracao-diferencial, 0, 32767, 0, 1023);  
        
      }else if (diferencial < 32768 * dead_zone *(-1)){//esquerda
        PWM1_DutyCycle = map(aceleracao + diferencial , 0, 32767, 0, 1023);   
        PWM2_DutyCycle = map(aceleracao, 0, 32767, 0, 1023);  
        
      }else{
        PWM1_DutyCycle = map(aceleracao, 0, 32767, 0, 1023);   
        PWM2_DutyCycle = map(aceleracao, 0, 32767, 0, 1023);   
      }

        
      
    }
    else if (joyLVert <  32768 * dead_zone * (-1)){ //Atras
      digitalWrite(IN1A, !sentido_1A);
      digitalWrite(IN2A, sentido_1A);

      digitalWrite(IN3A, !sentido_2A);
      digitalWrite(IN4A, sentido_2A);

      digitalWrite(IN1B, !sentido_1B);
      digitalWrite(IN2B, sentido_1B);

      digitalWrite(IN3B, !sentido_2B);
      digitalWrite(IN4B, sentido_2B);


      if (diferencial > 32768 * dead_zone){ //direita
        PWM1_DutyCycle = map(aceleracao, 0, 32767, 0, 1023);   
        PWM2_DutyCycle = map(aceleracao-diferencial, 0, 32767, 0, 1023);  
        
      }else if (diferencial < 32768 * dead_zone *(-1)){//esquerda
        PWM1_DutyCycle = map(aceleracao + diferencial , 0, 32767, 0, 1023);   
        PWM2_DutyCycle = map(aceleracao, 0, 32767, 0, 1023);  
        
      }else{
        PWM1_DutyCycle = map(aceleracao, 0, 32767, 0, 1023);   
        PWM2_DutyCycle = map(aceleracao, 0, 32767, 0, 1023);   
      }


      
    }else if (joyLHori >  32768 * dead_zone){ //direita
      digitalWrite(IN1A, !sentido_1A);
      digitalWrite(IN2A, sentido_1A);

      digitalWrite(IN3A, !sentido_2A);
      digitalWrite(IN4A, sentido_2A);

      digitalWrite(IN1B, sentido_1B);
      digitalWrite(IN2B, !sentido_1B);

      digitalWrite(IN3B, sentido_2B);
      digitalWrite(IN4B, !sentido_2B);


      PWM1_DutyCycle = map(aceleracao, 0, 32767, 0, 1023);   
      PWM2_DutyCycle = map(aceleracao, 0, 32767, 0, 1023); 

      
    }
    else if (joyLHori < 32768 * dead_zone * (-1)){//esquerda
      digitalWrite(IN1A, sentido_1A);
      digitalWrite(IN2A, !sentido_1A);

      digitalWrite(IN3A, sentido_2A);
      digitalWrite(IN4A, !sentido_2A);

      digitalWrite(IN1B, !sentido_1B);
      digitalWrite(IN2B, sentido_1B);

      digitalWrite(IN3B, !sentido_2B);
      digitalWrite(IN4B, sentido_2B);

      
      PWM1_DutyCycle = map(aceleracao, 0, 32767, 0, 1023);   
      PWM2_DutyCycle = map(aceleracao, 0, 32767, 0, 1023);  

    }else {
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
