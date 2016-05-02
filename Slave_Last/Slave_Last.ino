int waardenlast[]={0,0};
int tel=0;
int tel2=0;
int checkk =0;
int nr=0;
int r=0;
  
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
  pinMode(3,OUTPUT);
  
}

void loop() {
  digitalWrite(3,LOW);
  
  if(Serial1.available()){
    int s = Serial1.read();
    Serial.println(s);

    
    if (s==255||s==254){
      tel++;
    }
  
    if (tel==3 && s==10){
      checkk=1;
    }

    if(tel==3 && checkk==1 &&tel2==1){
      nr=s/1000;
      tel2++;
    }

    if(tel==3 && checkk==1 &&tel2==2){
      r=s/1000;
      tel2=0;
    }
    
    delay(200);
    if(tel==6 && checkk==1){
      delay(1000);
      Serial.println("INLEZEN");
      waardenlast[1]=nr;
      waardenlast[2]=r;
      Serial.print("Niet-regelbaar:");
      Serial.println(nr);
      Serial.print("regelbaar: ");
      Serial.print(r);
      
      delay(100);
      checkk=0;
    }

    if(tel==6){
    tel=0;
    }
    
  }
}

