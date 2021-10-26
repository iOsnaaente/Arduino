
#define ANG_MASK     65472    // 0x1111111111000000: mask to obtain first 10 digits with position info
#define STATUS_MASK     63    // 0x0000000000111111; mask to obtain last 6 digits containing status info

const int ledPin   = 13;
const int clockPin = 12; 
const int CSnPin   = 10;
const int inputPin = 2; 

int inputstream = 0;   

long int packeddata = 0; 
long int angle = 0;   
long int state = 0;

int angulo; 
int status; 


void setup(){
  
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);   // visual signal
  pinMode(clockPin, OUTPUT); // SCK
  pinMode(CSnPin, OUTPUT);   // CSn 
  pinMode(inputPin, INPUT);  // SDA
}

long int printTime = 0 ;

void loop(){
  

  digitalWrite(CSnPin, HIGH);     // CSn high
  digitalWrite(clockPin, HIGH);   // CLK high
  delay(1);                       // wait for 1m seconds 
  
  digitalWrite(ledPin, HIGH);     // signal start of transfer
  digitalWrite(CSnPin, LOW);      // CSn low: start of transfer
  delay(1);
  
  digitalWrite(clockPin, LOW);    // CLK goes low: start clocking
  delay(1);                        
  
  for (int x=0; x <16; x++){  
    digitalWrite(clockPin, HIGH);     
    delay(1); 

    inputstream = digitalRead(inputPin);               // read one bit of data from pin
    packeddata = ( (packeddata << 1) + inputstream );  // left-shift summing variable, add pin value

    digitalWrite(clockPin, LOW);
    delay(1);                        
  }
  
  digitalWrite(ledPin, LOW); 

  angle = (packeddata & ANG_MASK);
  angle = angle >> 6;
  state = packeddata & STATUS_MASK;

  angulo = int(angle);
  status = int(state);
  

  // RESET DATA TO RECALCULATE
  packeddata = 0;
  angle = 0;
}



// SEND THE ANGLE AWAYS THE SERIAL GET A '\n' CHAR  
void serialEvent(){
  while(Serial.available()){
    char inChar = (char)Serial.read();
    if (inChar == '\n'){
      
      union{
        int intValues[2] = { angulo, status };
        byte byteValues[4];
      } p;
      
      for (int i = 0; i < 4 ; i ++)
        Serial.print((char)p.byteValues[i]);
      Serial.println();
    }
  }
}
