#define SEQUENCE_START 2
#define SEQUENCE_MAX 32
#define END_SEQUENCE -1

int buttonInputs[] = { 2, 12 };
int nInputs = 2;

int ledOutputs[] = { 5, 10 };
int nOutputs = 2;

int beepOutput = 9;

int sequence[SEQUENCE_MAX];

void setup()
{
  Serial.begin(9600);
  Serial.println("Comecou");

  // Inicializar botoes
  for (int i=0; i < nInputs; i++)
  {
    pinMode(buttonInputs[i], INPUT);
    digitalWrite(buttonInputs[i], HIGH);
  }

  //Inicializar leds
  for (int i=0; i < nOutputs; i++)
  {
    pinMode(ledOutputs[i], OUTPUT);
  }
}

void initSequence()
{
  // Inicializar sequencia
  for (int i=0; i < SEQUENCE_MAX; i++)
  {
    sequence[i] = END_SEQUENCE;
  }

  // Seed randomica
  randomSeed(analogRead(0));
  for (int i=0; i < SEQUENCE_START; i++)
  {
    sequence[i] = random() % nInputs;
  }
}

// Função para rapidamente piscar os LEDs
void blinkLeds(int t)
{
    digitalWrite(ledOutputs[0], HIGH);
    digitalWrite(ledOutputs[1], HIGH);
    delay(t/2);
    digitalWrite(ledOutputs[0], LOW);
    digitalWrite(ledOutputs[1], LOW);
    delay(t/2);
}

void loop()
{
  // Esperar inicio
  delay(1000);

  Serial.println("Esperando aperto para iniciar");
  while (digitalRead(buttonInputs[0]) == HIGH &&
        digitalRead(buttonInputs[1]) == HIGH)
  {
    blinkLeds(300);
  }

  // Indicador de resposta correta
  bool correct = true;

  // Cria uma nova sequencia
  initSequence();

  // Temos certeza do inicio do jogo aqui
  while (correct)
  {    
    delay(500);
    digitalWrite(ledOutputs[0], LOW);
    digitalWrite(ledOutputs[1], LOW);
    delay(500);

    // Toca uma sequencia
    int pos = 0;
    while (sequence[pos] != END_SEQUENCE)
    {
      int led = sequence[pos];

      digitalWrite(ledOutputs[led], HIGH);
      delay(1000);
      digitalWrite(ledOutputs[led], LOW); 
      delay(1000);

      pos += 1;
      Serial.print("Toca: ");
      Serial.println(led);
    }
    
    int nPressed = 0;
    
    // Fazendo a leitura da resposta
    while (nPressed < pos && correct)
    {
      delay(1000);
      digitalWrite(ledOutputs[0], LOW);
      digitalWrite(ledOutputs[1], LOW);

       while (digitalRead(buttonInputs[0]) == HIGH &&
              digitalRead(buttonInputs[1]) == HIGH)
      {
          delay(200);
      }
  
      // Primeiro botão pressionado e correto
      if (digitalRead(buttonInputs[0]) == LOW && sequence[nPressed] == 0)
      {
        digitalWrite(ledOutputs[0], HIGH);
        Serial.println("Correto");
        correct = true;
      }
      // Segundo botão pressionado e correto
      else if (digitalRead(buttonInputs[1]) == LOW && sequence[nPressed] == 1)
      {
        digitalWrite(ledOutputs[1], HIGH);
        Serial.println("Correto 2");
        correct = true;
      }
      else
      {
        digitalWrite(ledOutputs[0], HIGH);
        digitalWrite(ledOutputs[1], HIGH);

        Serial.println("Errado");
        correct = false;
      }

      nPressed += 1;
    }

    // Se correto aumente a sequencia
    if (correct)
    {
      sequence[pos] = random() % nInputs;
    }
  }
}

