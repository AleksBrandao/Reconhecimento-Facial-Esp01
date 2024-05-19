#include <ESP8266WiFi.h>
#include <espnow.h>
//MAC: 8c:aa:b5:7c:16:62

const int relayPin = 0; // Pinagem para o relé
const int ledPin = 2;   // LED embutido na ESP-01, geralmente conectado ao GPIO 2

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len);

void setup() {
    Serial.begin(115200);
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, HIGH); // Assume que HIGH significa relé desligado

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); // Desliga o LED (ativo baixo no ESP-01)

    WiFi.mode(WIFI_STA); // Configura o ESP como Station

    if (esp_now_init() != 0) {
        Serial.println("Erro na inicialização do ESP-NOW");
        return;
    }

    uint8_t peerAddress[] = {0xEC, 0x64, 0xC9, 0x85, 0xAE, 0xB4}; // Substitua pelo MAC correto
    esp_now_add_peer(peerAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

    esp_now_register_recv_cb(OnDataRecv); // Registra função de callback para recebimento de dados
    Serial.println("ESP-NOW inicializado e pronto para receber mensagens.");
}

void loop() {
    digitalWrite(ledPin, LOW);  // Acende o LED
    delay(500);
    digitalWrite(ledPin, HIGH); // Apaga o LED
    delay(500);
    Serial.print(".");
}

// Função para tratar dados recebidos via ESP-NOW
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
    Serial.print("Mensagem recebida: ");
    Serial.println((char*)incomingData);

    if (strcmp((char*)incomingData, "open") == 0) {
        digitalWrite(relayPin, LOW); // Ativa o relé
        delay(1000);                   // Mantém o relé ativado por 500ms
        digitalWrite(relayPin, HIGH);  // Desativa o relé
    }
}
