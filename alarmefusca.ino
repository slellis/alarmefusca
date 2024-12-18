/*
  Conexões elétricas:
  - MFRC522:
    RST (Reset) -> GPIO 4
    SDA (SS)    -> GPIO 5
    MOSI        -> GPIO 23
    MISO        -> GPIO 19
    SCK         -> GPIO 18
    VCC         -> 3.3V
    GND         -> GND

  - HC-SR04:
    VCC         -> 3.3V
    GND         -> GND
    TRIG        -> GPIO 25
    ECHO        -> GPIO 26

  - Buzzer:
    VCC         -> GPIO 27
    GND         -> GND

  - LED (simulando alarme):
    Anodo (+)   -> GPIO 13
    Catodo (-)  -> GND
*/

#include <SPI.h>
#include <MFRC522.h>

// Definições dos pinos
#define RST_PIN 4
#define SS_PIN 5
#define LED_PIN 13
#define BUZZER_PIN 27
#define TRIG_PIN 25
#define ECHO_PIN 26

// Configuração do módulo RC522
MFRC522 rfid(SS_PIN, RST_PIN);

// UID autorizado
byte authorizedUID[] = {0x36, 0xAC, 0xB3, 0xF9};

// Variáveis de estado
bool isLEDOn = false;
bool isBuzzerOn = false;
bool motionDetected = false;

void setup() {
  Serial.begin(115200);
  SPI.begin(); 
  rfid.PCD_Init(); 

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(LED_PIN, LOW); 
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("Sistema inicializado. Passe a tag RFID para ligar/desligar o alarme.");
}

void loop() {

  Serial.print("  isLEDOn: ");
  Serial.print(isLEDOn);
  Serial.print("   isBuzzerOn: ");
  Serial.println(isBuzzerOn);

  // Verifica se uma nova tag foi apresentada
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    if (checkAuthorizedUID()) {
      toggleLED();
      if (!isLEDOn) {
        deactivateBuzzer(); // Desliga o buzzer ao desativar o alarme
      } else {
        waitForActivation(); // Espera antes de ativar o alarme
      }
      delay(1000); // Pequeno delay para evitar leituras repetidas
    }
    rfid.PICC_HaltA(); // Finaliza a comunicação com a tag
  }

  // Se o LED estiver aceso, verifica aproximação
  if (isLEDOn) {
    detectMotionAndTriggerAlarm();
  }
}

// Função para verificar se a tag lida é autorizada
bool checkAuthorizedUID() {
  if (rfid.uid.size != sizeof(authorizedUID)) {
    return false;
  }

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] != authorizedUID[i]) {
      return false;
    }
  }
  return true;
}

// Função para alternar o estado do LED
void toggleLED() {
  isLEDOn = !isLEDOn;
  digitalWrite(LED_PIN, isLEDOn ? HIGH : LOW);
  Serial.print("Alarme ");
  Serial.println(isLEDOn ? "ativado." : "desativado.");

  if (!isLEDOn) {
    Serial.println("Alarme desativado. Bip curto.");
    tone(BUZZER_PIN, 1000, 200); // Emite um bip curto
  }
}

// Função para esperar antes de ativar o alarme
void waitForActivation() {
  for (int i = 0; i < 15; i++) {
    tone(BUZZER_PIN, 1000, 200);
    delay(1000);
  }
  tone(BUZZER_PIN, 1000, 2000); // Bip longo para indicar ativação
  Serial.println("Alarme ativado e pronto para monitorar movimentos.");
}

// Função para detectar movimento com o HC-SR04 e acionar o buzzer
void detectMotionAndTriggerAlarm() {
  long duration;
  int distance;

  // Envia um pulso de 10 microsegundos pelo pino TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Lê o tempo do pulso no pino ECHO
  duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout de 30 ms

  // Calcula a distância em centímetros
  distance = duration * 0.034 / 2;

  // Imprime a distância no monitor serial
  if (duration > 0) {
    Serial.print("Distância: ");
    Serial.print(distance);
    Serial.println(" cm");
    Serial.print("motionDetected: ");
    Serial.print(motionDetected);
  }

  // Verifica se um objeto está dentro do limite e se o movimento já foi detectado
  if (distance >= 10 && distance <= 50 && !motionDetected) {

    Serial.print("motionDetected: ");
    Serial.println(motionDetected);
    
    motionDetected = true;

    Serial.print("Distância: ");
    Serial.print(distance);
    Serial.println(" cm");
    Serial.print("motionDetected: ");
    Serial.println(motionDetected);

    Serial.println("Movimento detectado!");

    // Emite 10 bips antes de ativar o buzzer permanentemente
    for (int i = 0; i < 10; i++) {
     tone(BUZZER_PIN, 1000, 200);
     delay(1000);
    }
    activateBuzzer();
  }
}


// Função para ativar o buzzer
void activateBuzzer() {
  if (!isBuzzerOn) { // Verifica se o buzzer já não está ativado
    isBuzzerOn = true;
    //digitalWrite(BUZZER_PIN, HIGH);
    tone(BUZZER_PIN, 1000); // Gere um tom contínuo de 1000 Hz
    Serial.println("Buzzer ativado permanentemente.");
  }
}


// Função para desativar o buzzer
void deactivateBuzzer() {
  isBuzzerOn = false;
  motionDetected = false;
  //digitalWrite(BUZZER_PIN, LOW);
  noTone(BUZZER_PIN); // Pare o tom contínuo
  Serial.println("Buzzer desativado.");
}
