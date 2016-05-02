int pin = 3;
int max_net = 10; // in stroom
int maxwaarde = 2;
int streefwaarde = 1.8;
int betaallijst[] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 3};
int vast[]= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int variabel[] = {1, 1, 1, 1, 4, 1, 1, 1, 1, 1};
int gevraagd[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int vraaginfo[] = {255, 255, 255, 0, 254, 254, 254}; // vraagt na verbruik
int huizen=1;


void setup() {
 Serial.begin(9600);
 Serial1.begin(9600);
 pinMode(pin, OUTPUT);
}


void loop() {

  // eerste verbruiksaanvraag
  
  digitalWrite(pin, HIGH);
  
  
  for(int i=0; i<huizen; i++){
    gevraagd[i]=waardehuis(i);
   
  }

  for(int i=0; i<huizen; i++){
    Serial.println("varibale last: ");
    Serial.println(variabel[i]);
    while(gevraagd[i]> streefwaarde && variabel[i] != 0){
      if(variabel[i]>0){
        variabel[i] = (variabel[i]-0.25);
        Serial.println("COMMANDO REGELBARE LAST");
        Serial.println(variabel[i]*4);
        commandolast(i, vast [i], variabel[i]);
      }

      gevraagd[i]=waardehuis(i); 
    }

    if(gevraagd[i]>maxwaarde){
      Serial.println("COMMANDO NIET REGELBARE LAST");
      vast[i]=0;
      commandolast(i, vast[i], variabel[i]);
      
    }
  }


  int w=1;
  int som=1;
  while( sumvraag() > max_net && sumvraagvar() != 0){
      int index[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int a = 0;

      for(int j=0; j<huizen; j++){
        if(betaallijst[j]==w){
          index[a]=j;
          a+1;
        }
      }
      
      while (sumvraag()>max_net && som !=0){
        int maxi=0;
        int maxindex=0;

        for (int k=0; k<a;k++){
          if(variabel[index[k]]){
            if(gevraagd[index[k]]>maxi){
              maxi = gevraagd[index[k]];
              maxindex = index[k];
            }
          }
        }

        variabel[maxindex]=(variabel[maxindex]-0.25);
        commandolast(maxindex, vast[maxindex], variabel[maxindex]);
        gevraagd[maxindex]=waardehuis(maxindex);

        som = 0;
        for(int l=0; l<a; l++){
          som = som + variabel[index[l]];
        }
        
      }
    }

  som=1;
  w=1;
  while( sumvraag() > max_net && sumvraagvast() != 0){
      int index[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int a = 0;

      for(int j=0; j<10; j++){
        if(betaallijst[j]==w){
          index[a]=j;
          a+1;
        }
      }
      
      while (sumvraag()>max_net && sumvraagvast() !=0){
        int maxi=0;
        int maxindex=0;

        for (int k=0; k<a;k++){
          if(vast[index[k]]){
            if(gevraagd[index[k]]>maxi){
              maxi = gevraagd[index[k]];
              maxindex = index[k];
            }
          }
        }

        vast[maxindex]=vast[maxindex]-1;
        commandolast(maxindex, vast[maxindex], variabel[maxindex]);
        gevraagd[maxindex]=waardehuis(maxindex);

        som = 0;
        for(int l=0; l<a; l++){
          som = som + vast[index[l]];
        }
        
      }
     
    }
    
  }
  
  
 



int waardehuis(int i){
    int vraaginfo[] = {255, 255, 255, 0, 254, 254, 254}; // vraagt na verbruik
    int k=0;
    int tel=0;
    int waarde =0;
    vraaginfo[3]=i;
    int result;
    
    for(int j=0; j<7; j++){
      Serial1.write(vraaginfo[j]);
      delay(5);
    }

    digitalWrite(pin, LOW);
     while(k==0){
        if(Serial1.available()){
               
          int s=Serial1.read();
          Serial.println(s);
         
          if(s==253||s==252){
            tel++;
          }
  
          if(tel==3){
            waarde = s;
          }
  
          if(tel==6){
            result=waarde;
            k=1;
          }
        }
    
    }
    k=0;
    digitalWrite(pin,HIGH);
    tel=0;
    delay(50);
    Serial.print("Stroomaanvraag van huis ");
    Serial.print(i);
    Serial.print(" ");
    Serial.println(gevraagd[i]);
    return result;
}

void commandolast (int i, int nr, int r){
   int Protocol[]= {255,255,255, i+10, nr, r*4, 254, 254, 254};
   for(int j=0; j<10; j++){
      Serial1.write(Protocol[j]);
      Serial.write(Protocol[j]);
      delay(5);
    }
  
}

//telt op lengte 10
int sumvraag(){
  int som1 = 0;
  for(int i=0; i<huizen; i++){
    
    som1 = som1 + gevraagd[i];
  }
return som1;
}

int sumvraagvar(){
  int som2=0;
  for(int i=0;i<huizen; i++){
    som2=som2+gevraagd[i];
  }
return som2;
}

int sumvraagvast(){
  int som3=0;
  for(int i=0;i<10; i++){
    som3=som3+gevraagd[i];
  }
return som3;
}
