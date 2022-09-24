#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(8, 10); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

int R_PWM1 = 3;
int L_PWM1 = 5;
int R_ENA1 = 1; 
int L_ENA1 = 2;

int R_PWM2 = 6;
int L_PWM2 = 9;
int R_ENA2 = 4; 
int L_ENA2 = 7;

int stat = 1; //1:sistem jalan, 2:sistem stop

#define limit A0
#define pump  A1
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00201 -> Receiver
  radio.openReadingPipe(1, addresses[1]); // 00202 -> Transmitter
  radio.setPALevel(RF24_PA_MIN);

  pinMode(R_PWM1,OUTPUT);
  pinMode(L_PWM1,OUTPUT);
  pinMode(R_ENA1,OUTPUT);
  pinMode(L_ENA1,OUTPUT);

  pinMode(R_PWM2,OUTPUT);
  pinMode(L_PWM2,OUTPUT);
  pinMode(R_ENA2,OUTPUT);
  pinMode(L_ENA2,OUTPUT);

  pinMode(limit, INPUT_PULLUP);
  pinMode(pump, OUTPUT);
  
  
  Serial.println("=======================================================================");
  Serial.println("                          PERSIAPAN SISTEM                             ");
  Serial.println("=======================================================================");
  Serial.println("\n\n\n");
  
  digitalWrite(pump, LOW); //pump aktif
  Serial.println("pump aktif");
  delay(5000);
  
  Serial.println("\n\n\n");
  Serial.println("=======================================================================");
  Serial.println("                          PEMBERSIHAN DIMULAI                             ");
  Serial.println("=======================================================================");
  Serial.println("\n\n\n");
  
  putar(255);
  Serial.println("kain aktif");
  Serial.println("\n\n\n");

  delay(5000);
  
}

void loop(){
  if(digitalRead(limit) == LOW){
    Serial.println("\n\n\n");
    stop_();
    Serial.println("roda berhenti");
    berhenti_putar();
    Serial.println("kain berhenti");
    digitalWrite(pump, HIGH);
    Serial.println("pump berhenti");
    Serial.println("\n\n\n");
    

    Serial.println("=======================================================================");
    Serial.println("                         PEMBERSIHAN SELESAI                           ");
    Serial.println("=======================================================================");
    Serial.println("\n\n\n");
    
    while(1){
      radio.startListening();
      const char text[] = "";
      if (radio.available()) {
        while (radio.available()){
          radio.read(&text, sizeof(text));
        }
      }

      if(digitalRead(limit) == HIGH){
        putar(255);

        Serial.println("kain aktif kembali");
        digitalWrite(pump, LOW);
        Serial.println("pompa aktif kembali");
        delay(300);
        
        Serial.println("\n\n\n");
        Serial.println("=======================================================================");
        Serial.println("                          PEMBERSIHAN DIMULAI KEMBALI                             ");
        Serial.println("=======================================================================");
        Serial.println("\n\n\n");
        
        break;
      }
    }
  }
  else{
    radio.startListening();
    const char text[] = "";
    if (radio.available()) {
      while (radio.available()){
        radio.read(&text, sizeof(text));
      }
      if(text[0] == 'X'){
        //Serial.println(text);  
        if(stat == 1){
          Serial.println("\n\n");
          Serial.println(". -> STOP");
          stat = 0;
        }
        else{
          Serial.println("\n\n");
          Serial.println(". -> START");
          Serial.println("\n\n");
          stat = 1;          
        }
      }
    }

    if(stat == 1){
      maju(255);
      Serial.println("Roda aktif");
    }
    else{
      stop_();
    }
  }
}
//----------------------------------------------------------------------------------//
void maju(int pwm){
  digitalWrite(R_ENA1,HIGH);        
  digitalWrite(L_ENA1,HIGH);        
  analogWrite (L_PWM1,pwm);         
  analogWrite (R_PWM1,0);           
}
void mundur(int pwm){
  digitalWrite(R_ENA1,HIGH);        
  digitalWrite(L_ENA1,HIGH);        
  analogWrite (L_PWM1,0);           
  analogWrite (R_PWM1,pwm);         
}
void stop_(){
  digitalWrite(R_ENA1,HIGH);       
  digitalWrite(L_ENA1,HIGH);        
  analogWrite (L_PWM1,0);          
  analogWrite (R_PWM1,0);          
}
//----------------------------------------------------------------------------------//
void putar(int pwm){
  digitalWrite(R_ENA2,HIGH);
  digitalWrite(L_ENA2,HIGH);
  analogWrite(L_PWM2,pwm);
  analogWrite(R_PWM2,0); 
}
void berhenti_putar(){
  digitalWrite(R_ENA2,HIGH);
  digitalWrite(L_ENA2,HIGH);
  analogWrite(L_PWM2,0);
  analogWrite(R_PWM2,0); 
}
