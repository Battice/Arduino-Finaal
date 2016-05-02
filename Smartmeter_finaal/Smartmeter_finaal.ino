
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
  pinMode(3,OUTPUT);
  
}

void loop() {
  antwoordstroom();
  
}


int antwoordstroom(){

  int tel=0;
  int antwoord[] = {253,253,253,3,252,252,252};
  int checkk =0;

  digitalWrite(3,LOW);
  while(tel<6){
    if(Serial1.available()){
      int s = Serial1.read();
      Serial.println(s);
  
      
      if (s==255||s==254){
        tel++;
      }
    
      if (tel==3 && s==0){
        checkk=1;
      }
  
      delay(200);
      if(tel==6 && checkk==1){
        delay(1000);
        Serial.println("STUREN");
        digitalWrite(3,HIGH);
        delay(100);
        
        for(int j=0; j<7;j++){
          Serial1.write(antwoord[j]);
          delay(5);
        }
      }
      
    }
  }
}

