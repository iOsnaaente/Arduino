int direitaA   = 4;    // IN 1
int direitaB   = 5;    // IN 2
int esquerdaA  = 6;    // IN 3
int esquerdaB  = 7;    // IN 4

int triggerPin = 11;
int echoPin    = 12;

void setup () {
  Serial.begin(9600);
  
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  pinMode (direitaA, OUTPUT);
  pinMode (direitaB, OUTPUT);
  pinMode (esquerdaA, OUTPUT);
  pinMode (esquerdaB, OUTPUT);
}

double measureDistanceCm() {
  int duration;
  double distance;
  
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(triggerPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  return distance;
}


void loop () {
  double distance = measureDistanceCm() ;
  Serial.print("1: ");
  Serial.print( distance);
  Serial.println(" cm");

  Serial.println("---");
  if (distance < 5) {
    digitalWrite(direitaA, HIGH);
    digitalWrite(direitaB, LOW);
    digitalWrite(esquerdaA, HIGH);
    digitalWrite(esquerdaB, LOW);
    delay(500);

    digitalWrite(direitaA, LOW);
    digitalWrite(direitaB, LOW);
    digitalWrite(esquerdaA, HIGH);
    digitalWrite(esquerdaB, LOW);
    delay(200);
  }
  else
  {
    digitalWrite(direitaA, LOW);
    digitalWrite(direitaB, HIGH);
    digitalWrite(esquerdaA, LOW);
    digitalWrite(esquerdaB, HIGH);

  }
}
