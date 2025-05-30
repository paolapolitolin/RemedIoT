#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Wi-Fi
const char* ssid = "WIFI";
const char* password = "SENHA";

// MQTT Broker HiveMQ (público, sem autenticação)
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopicSensor = "remediot/sensor/status";
const char* mqttTopicAlarme = "remediot/alarme/status";

// Hardware
const int sensorPin = 26;
const int buzzer = 15;

// NTP
const long utcOffsetInSeconds = -3 * 3600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// Controle
bool alarmeTocando = false;
bool alarmeTocadoNoMinuto = false;
unsigned long lastMqttPublish = 0;
unsigned long tempoInicioAlarme = 0;  

// Para controlar impressão da hora
int ultimoMinutoImpresso = -1;

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    String clientId = "RemedIoT-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando de novo em 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(sensorPin, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" conectado");

  client.setServer(mqttServer, mqttPort);

  timeClient.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado, reconectando...");
    WiFi.begin(ssid, password);
    delay(5000);
    return;
  }

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();

  timeClient.update();

  int hora = timeClient.getHours();
  int minuto = timeClient.getMinutes();

  // Imprime hora e minuto somente quando o minuto muda
  if (minuto != ultimoMinutoImpresso) {
    Serial.printf("Hora atual: %02d:%02d\n", hora, minuto);
    ultimoMinutoImpresso = minuto;
    alarmeTocadoNoMinuto = false; // reset flag a cada minuto novo
  }

  int estadoSensor = digitalRead(sensorPin);

  unsigned long now = millis();
  if (now - lastMqttPublish > 2000) {
    lastMqttPublish = now;
    String msg = (estadoSensor == LOW) ? "FECHADO" : "ABERTO";
    client.publish(mqttTopicSensor, msg.c_str());
    Serial.printf("Status sensor publicado: %s\n", msg.c_str());
  }

  // Dispara alarme a cada 2 minutos mas pode ser organizado de forma que apite qualque hora definida.
  //Seja de 6 em 6 horas ou até as 14 horas.
  if ((minuto % 2 == 0) && !alarmeTocadoNoMinuto) {
    alarmeTocando = true;
    alarmeTocadoNoMinuto = true;
    tempoInicioAlarme = millis();
    Serial.println("Iniciando alarme de 2 em 2 minutos...");
    client.publish(mqttTopicAlarme, "ON");  // Publica que o alarme iniciou
  }

  // Para alarme se porta abrir
  if (alarmeTocando && estadoSensor == HIGH) {
    alarmeTocando = false;
    Serial.println("Alarme parado pois porta abriu.");
    noTone(buzzer);
    client.publish(mqttTopicAlarme, "OFF"); // Publica que o alarme parou
  }

  // Para alarme após 20 segundos
  //Pode modular isso para o quanrto de tempo achar necessário
  if (alarmeTocando && (millis() - tempoInicioAlarme >= 20000)) {
    alarmeTocando = false;
    Serial.println("Alarme parado após 20 segundos.");
    noTone(buzzer);
    client.publish(mqttTopicAlarme, "OFF"); // Publica que o alarme parou
  }



  // Toca buzzer em 200 Hz enquanto alarme ativo e porta fechada
  //Colocamos um som tecnicamente baixo mas para o idoso ouvir recomendamos 2000Hz
  //Seria um som bem chato, que faria o idoso abrir a gaveta e tomar seu remédio
  if (alarmeTocando && estadoSensor == LOW) {
    tone(buzzer, 200);
    delay(100);
    noTone(buzzer);
    delay(100);
  } else {
    noTone(buzzer);
  }
}
