const int clockPin = 9; // Pino de saída do clock
bool delayEnabled = false; // Estado inicial do atraso

void setup() {
  pinMode(clockPin, OUTPUT);

  // Configuração inicial: clock normal (16 MHz)
  configureClock(16);
  Serial.begin(9600);
  Serial.println("Clock Injection Ready!");
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 'A') { // Ativar atraso
      delayEnabled = true;
      configureClock(12); // Reduz para 12 MHz
      Serial.println("Clock delay activated!");
    } else if (command == 'D') { // Desativar atraso
      delayEnabled = false;
      configureClock(16); // Retorna a 16 MHz
      Serial.println("Clock delay deactivated!");
    }
  }
}

void configureClock(int frequencyMHz) {
  // Configuração do Timer1 para a frequência desejada
  TCCR1A = (1 << COM1A0); // Toggle OC1A on Compare Match
  TCCR1B = (1 << WGM12) | (1 << CS10); // Modo CTC, prescaler = 1

  if (frequencyMHz == 16) {
    OCR1A = 0; // Para 16 MHz
  } else if (frequencyMHz == 12) {
    OCR1A = 1; // Para ~12 MHz
  }
}



