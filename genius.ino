#define SEQUENCE_START 2
#define SEQUENCE_MAX 32
#define END_SEQUENCE -1

int buttonInputs[] = { 2, 3, 4, 5 };
int nInputs = 4;

int ledOutputs[] = { 9, 10, 11, 12 };
int nOutputs = 4;

int beepOutput = 6;

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

// Retorna o indice do botao apertado ou -1 se nenhum foi apertado
int checkButtons()
{
   int button = -1;

   // Testar todos botoes
   for (int i=0; i < nInputs && button == -1; i++)
   {
     if (digitalRead(buttonInputs[i]) == LOW)
     {
       button = i;
     }
   }

   return button;
}

int noteToFreq(int note)
{
  return (int) pow(2, (double)note/12) * 440;
}

// Toca uma melodia num alto falate ou buzzer
void playMelody(int *mel, int *dur, int n)
{
  for(int i=0; i < n; i++)
  {
    tone(beepOutput, noteToFreq(mel[i]), dur[i]*100);
    delay(dur[i]*100);
    noTone(beepOutput);
  }
}

void playStart()
{
  int n = 7;
  int mel[] = {30, 34, 37, 42, 37, 34, 30};
  int dur[] = {1, 1, 1, 2, 1, 1, 4};
  playMelody(mel, dur, n);
}

void playEnd()
{
  int n = 12;
  int mel[] = {42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31};
  int dur[] = { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  8};
  playMelody(mel, dur, n);
}

// Função para rapidamente piscar os LEDs
void blinkLeds(int t)
{
    for (int i=0; i < nInputs; i++)
    {
      digitalWrite(ledOutputs[i], HIGH);
    }
    delay(t/2);
    for (int i=0; i < nInputs; i++)
    {
      digitalWrite(ledOutputs[i], LOW);
    }
    delay(t/2);
}

// Faz um beep com a frequencia do botao apertado
void beep(int button)
{
  int freqs[] = {30, 32, 34, 37};
  int freq = noteToFreq(freqs[button]);
  tone(beepOutput, freq, 1000);
  delay(1000);
  noTone(beepOutput);
}

void loop()
{
  // Esperar inicio
  delay(1000);

  Serial.println("Esperando aperto para iniciar");
  while (checkButtons() == -1)
  {
    blinkLeds(300);
  }

  // Indicador de resposta correta
  bool correct = true;

  // Cria uma nova sequencia
  initSequence();
  // Melodia do inicio
  playStart();

  // Temos certeza do inicio do jogo aqui
  while (correct)
  {
    delay(500);
    // Reseta todos os botoes
    for (int i=0; i < nOutputs; i++)
      digitalWrite(ledOutputs[i], LOW);
    delay(500);

    // Toca uma sequencia
    int pos = 0;
    while (sequence[pos] != END_SEQUENCE)
    {
      int led = sequence[pos];

      digitalWrite(ledOutputs[led], HIGH);

      pos += 1;
      Serial.print("Toca: ");
      Serial.println(led);
      beep(led);
      digitalWrite(ledOutputs[led], LOW);
      delay(1000);
    }

    int nPressed = 0;

    delay(1000);

    // Fazendo a leitura da resposta
    while (nPressed < pos && correct)
    {
      for (int i=0; i < nOutputs; i++)
        digitalWrite(ledOutputs[i], LOW);

      int button = checkButtons();

      while (button == -1)
      {
          delay(200);
          button = checkButtons();
      }

      if (button == sequence[nPressed])
      {
        digitalWrite(ledOutputs[button], HIGH);
        Serial.println("Correto");
        beep(button);
        correct = true;
      }
      else
      {
        for (int i=0; i < nOutputs; i++)
          digitalWrite(ledOutputs[i], HIGH);

        Serial.println("Errado");
        correct = false;
        // Melodia do fim
        playEnd();
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

