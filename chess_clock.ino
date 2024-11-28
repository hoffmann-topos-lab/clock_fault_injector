#include <LiquidCrystal.h>

// Inicializa o display (RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Mapeamento dos botões
const int btnRight = 0;
const int btnUp = 1;
const int btnDown = 2;
const int btnLeft = 3;
const int btnSelect = 4;

// Variáveis globais
int selectedTime = 1;  // Tempo inicial no menu (em minutos)
int whiteTime = 0;     // Tempo restante das brancas (em segundos)
int blackTime = 0;     // Tempo restante das pretas (em segundos)
bool isGameRunning = false;
bool isWhiteTurn = true; // Indica se é a vez das brancas
bool menuConfirmed = false; // Confirmação do menu

unsigned long previousMillis = 0;

// Funções auxiliares
void displayMenu() {
  lcd.clear();
  lcd.print("Ritmo: ");
  lcd.setCursor(7, 0);
  lcd.print(selectedTime);
  lcd.print(" min");
}

void displayGame() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("B: ");
  lcd.print(whiteTime / 60);      // Minutos
  lcd.print(":");
  if (whiteTime % 60 < 10) lcd.print("0");
  lcd.print(whiteTime % 60);

  lcd.setCursor(0, 1);
  lcd.print("P: ");
  lcd.print(blackTime / 60);      // Minutos
  lcd.print(":");
  if (blackTime % 60 < 10) lcd.print("0");
  lcd.print(blackTime % 60);
}

void checkWinner() {
  if (whiteTime <= 0) {
    lcd.clear();
    lcd.print("Vit. das Pretas!");
    while (true);  // Fim do programa
  } else if (blackTime <= 0) {
    lcd.clear();
    lcd.print("Vit. das Brancas!");
    while (true);  // Fim do programa
  }
}

// Configuração inicial
void setup() {
  lcd.begin(16, 2);
  displayMenu();
}

// Função de leitura simplificada dos botões
int readButtons() {
  int adcKeyVal = analogRead(A0); // Substitua por digitalRead se usar botões digitais
  if (adcKeyVal < 50) return btnRight;
  if (adcKeyVal < 195) return btnUp;
  if (adcKeyVal < 380) return btnDown;
  if (adcKeyVal < 555) return btnLeft;
  if (adcKeyVal < 790) return btnSelect;
  return -1; // Nenhum botão pressionado
}

// Loop principal
void loop() {
  int button = readButtons();

  if (!menuConfirmed) {
    // Configurando o menu
    if (button == btnUp) {
      selectedTime++;
      if (selectedTime > 10) selectedTime = 1; // Limita a 10 minutos
      displayMenu();
      delay(200); // Para evitar leituras múltiplas
    }
    if (button == btnDown) {
      selectedTime--;
      if (selectedTime < 1) selectedTime = 10; // Limita a 1 minuto
      displayMenu();
      delay(200); // Para evitar leituras múltiplas
    }
    if (button == btnSelect) {
      whiteTime = blackTime = selectedTime * 60; // Converte minutos para segundos
      menuConfirmed = true;
      lcd.clear();
      lcd.print("Pressione Start!");
      delay(500);
    }
  } else if (!isGameRunning) {
    // Esperando o jogo começar
    if (button == btnSelect) {
      isGameRunning = true;
      displayGame();
      previousMillis = millis(); // Inicia o temporizador
    }
  } else {
    // Jogo em execução
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      if (isWhiteTurn) {
        whiteTime--;
      } else {
        blackTime--;
      }
      displayGame();
      checkWinner();
    }

    // Alternância de turno
    if (button == btnLeft) {
      isWhiteTurn = false; // Passa para as pretas
      delay(200);
    }
    if (button == btnRight) {
      isWhiteTurn = true; // Passa para as brancas
      delay(200);
    }
  }
}



