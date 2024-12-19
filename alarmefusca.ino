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
bool isAlarmActive = false;     // Controla se o alarme está ativo
bool isMotionDetected = false;  // Controla se foi detectado movimento
bool isAlarmTriggered = false;  // Controla se o alarme foi disparado

unsigned long motionStartTime = 0;
const unsigned long motionTimeout = 5000; // 5 segundos para desativar o alarme
unsigned long ledLastToggleTime = 0;
unsigned long ledInterval = 0; // Intervalo padrão do LED
bool ledState = false;

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
  // Verifica leitura de tag
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    if (checkAuthorizedUID()) {
      // Foi passada a tag para ativar ou desativar o alarme
      toggleAlarm();
    }
    rfid.PICC_HaltA();
  }

  // Verifica movimento
  if (isAlarmActive) {
    detectMotion();
    if (isMotionDetected) {
      handleMotion();
    }
  }

  // Gerencia o comportamento do LED
  manageLed();
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

// A tag foi passada no sensor para ativar ou desativar o alarme
void toggleAlarm() {
  isAlarmActive = !isAlarmActive; // Alterna true/false
  //ledInterval = isAlarmActive ? 3000 : 0; // Define o intervalo do LED para alarme ativo ou desligado
  digitalWrite(LED_PIN, LOW); // LED apagado inicialmente

  if (isAlarmActive) {
    Serial.println("Alarme ativado.");
    ledInterval = 3000;                 // LED piscando na frequencia de 3 segundos
    //soundBuzzerPattern(1, 200, 0);       // 1 bip curto
    soundBuzzerPattern(5, 200, 200);    // 5 bips curtos
    soundBuzzerPattern(1, 1000, 0);     // 1 bip longo
  } else {
    Serial.println("Alarme desativado.");
    ledInterval = 0;                    // LED para de piscar com alarme desativado
    soundBuzzer(3, 100, 50);       // 3 bips curtos
    digitalWrite(LED_PIN, LOW);         // Garante que o LED fique apagado
  }
  isMotionDetected = false;
  isAlarmTriggered = false; // Reseta o estado de alarme disparado
}

// Função para detectar movimento com o HC-SR04
void detectMotion() {
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

  if (distance >= 10 && distance <= 50) {
    if (!isMotionDetected) {
      isMotionDetected = true;
      motionStartTime = millis();
      ledInterval = 100; // Piscar frenético do LED
      Serial.println("Movimento detectado! Aguardando desativação.");
      //soundBuzzerPatternWithCheck(5, 200, 200); // 5 bips curtos com verificação de tag
      soundBuzzer(5, 500, 500); 
    }
  }
}

// Função para tratar movimento detectado
void handleMotion() {
  if (millis() - motionStartTime > motionTimeout && !isAlarmTriggered) {
    activatePermanentBuzzer();
    digitalWrite(LED_PIN, HIGH); // LED aceso permanentemente
    ledInterval = 0; // Para o piscar do LED
    isAlarmTriggered = true; // Marca o alarme como disparado
  }
}

// Função para ativar o buzzer permanentemente
void activatePermanentBuzzer() {
  Serial.println("Alarme disparado! Buzzer ativado permanentemente.");
  tone(BUZZER_PIN, 1000); // Bip permanente
}

// Função para emitir padrões de som no buzzer com verificação de tag
void soundBuzzerPatternWithCheck(int times, int duration, int interval) {
  for (int i = 0; i < times; i++) {
    // Verifica se uma tag válida foi lida durante o padrão de som
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial() && checkAuthorizedUID()) {
      toggleAlarm();
      rfid.PICC_HaltA();
      return; // Sai da função se o alarme for desativado
    }
    tone(BUZZER_PIN, 1000, duration);
    delay(duration);       // Duração do bip
    delay(1000 - duration); // Complemento até 1 segundo total
  }
}

// Função para emitir padrões de som no buzzer
void soundBuzzerPattern(int times, int duration, int interval) {
  for (int i = 0; i < times; i++) {
    tone(BUZZER_PIN, 1000, duration);
    digitalWrite(LED_PIN, HIGH);
    delay(duration);       // Duração do bip
    digitalWrite(LED_PIN, LOW);
    delay(1000 - duration); // Complemento até 1 segundo total
  }
}

// Função para emitir som no buzzer sem padrão de frequencia
void soundBuzzer(int times, int duration, int interval) {
  for (int i = 0; i < times; i++) {
    tone(BUZZER_PIN, 3000, duration);
    digitalWrite(LED_PIN, HIGH);
    delay(duration);       // Duração do bip
    digitalWrite(LED_PIN, LOW);
    delay(interval); 
  }
}

// Função para gerenciar o comportamento do LED
void manageLed() {
  if (ledInterval > 0 && millis() - ledLastToggleTime >= ledInterval) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    ledLastToggleTime = millis();
  }
}
