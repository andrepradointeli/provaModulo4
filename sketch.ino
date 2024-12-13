#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul

const int buttonPin = 18;  // O numero do pino do botão
int buttonState = 0;  // Variavel para verificar status do botão

const int ldrPin = 4;  // O numedo do pino do sensor LDR
int threshold=600;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(buttonPin, INPUT); // Inicializa o pino do botao como entrada (INPUT)
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  int ldrstatus=analogRead(ldrPin);

  if(ldrstatus<=threshold){
    Serial.print("its dark turn on led");
    Serial.println(ldrstatus);

  }else{
    Serial.print("its bright turn off light");
    Serial.println(ldrstatus);
  }
}

void loop() {
  // Leitura do valor do LDR
  int ldrstatus = analogRead(ldrPin);
  Serial.println(ldrstatus);

  // Verifica se está claro
  if (ldrstatus < threshold) {
    Serial.println("Está claro! Ativando modo convencional...");
    
    // Semáforo convencional: alterna entre verde, amarelo e vermelho
    digitalWrite(led_verde, HIGH);
    delay(3000); // LED verde aceso por 3 segundos
    digitalWrite(led_verde, LOW);

    digitalWrite(led_amarelo, HIGH);
    delay(2000); // LED amarelo aceso por 2 segundos
    digitalWrite(led_amarelo, LOW);

    digitalWrite(led_vermelho, HIGH);
    delay(5000); // LED vermelho aceso por 5 segundos
    digitalWrite(led_vermelho, LOW);
  } 
  // Caso contrário, ativa o modo noturno
  else {
    Serial.println("Está escuro! Ativando modo noturno...");
    
    digitalWrite(led_amarelo, HIGH);
    delay(1000); // LED amarelo piscando
    digitalWrite(led_amarelo, LOW);
    delay(1000);
  }
}