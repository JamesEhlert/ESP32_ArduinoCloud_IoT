#include "arduino_secrets.h"
/*
   Projeto: Controle de IoT com Arduino e sensor de temperatura e umidade AHT10
   Autor: [James Ehlert Reinard]
   Data: [16/04/2024]
   Descrição: Este código é um exemplo de integração de um sensor de temperatura e umidade AHT10
              com o Arduino IoT Cloud para monitoramento remoto. Ele lê os dados do sensor AHT10,
              envia para o Arduino IoT Cloud e controla um LED remotamente.
*/

#include "thingProperties.h" // Inclui as propriedades do Thing geradas pelo Arduino IoT Cloud
#include <Wire.h> // Biblioteca para comunicação I2C
#include <Adafruit_AHT10.h> // Biblioteca para o sensor AHT10

Adafruit_AHT10 aht; // Criação de uma instância do objeto Adafruit_AHT10

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial com taxa de transmissão de 9600 bps
  
  pinMode(5, OUTPUT); // Define o pino 5 como saída para controle do LED
  
  Wire.begin(21, 22); // Inicia a comunicação I2C com os pinos SDA e SCL
  
  if (!aht.begin()) { // Verifica se o sensor AHT10 foi encontrado
    Serial.println("Falha ao encontrar o sensor AHT!");
    while (1); // Fica preso em um loop infinito se o sensor não for encontrado
  }
  
  delay(1500); // Atraso para aguardar o Monitor Serial

  initProperties(); // Inicializa as propriedades do Thing do Arduino IoT Cloud

  ArduinoCloud.begin(ArduinoIoTPreferredConnection); // Conecta ao Arduino IoT Cloud
  
  setDebugMessageLevel(2); // Define o nível de depuração para exibir mais informações
  ArduinoCloud.printDebugInfo(); // Imprime informações de depuração do Arduino IoT Cloud
}

void loop() {
  ArduinoCloud.update(); // Atualiza o estado das variáveis e mantém a conexão com o Arduino IoT Cloud
  leituraSensor(); // Realiza a leitura dos dados do sensor e atualiza as variáveis do Arduino IoT Cloud
}

void onLedChange()  {
  // Função chamada sempre que o valor da variável 'led' é alterado no Arduino IoT Cloud
  // Controla o LED com base no valor recebido
  if (led == 1){
    digitalWrite(5, HIGH); // Liga o LED se o valor recebido for 1
  }
  else{
    digitalWrite(5, LOW); // Desliga o LED se o valor recebido for 0
  }
}

void leituraSensor() {
  // Função para ler os dados do sensor AHT10 e atualizar as variáveis do Arduino IoT Cloud
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp); // Obtém os dados de temperatura e umidade do sensor AHT10
  temperatura = temp.temperature; // Atualiza a variável de temperatura no Arduino IoT Cloud
  umidade = humidity.relative_humidity; // Atualiza a variável de umidade no Arduino IoT Cloud
  delay(00); // Atraso para evitar leituras muito frequentes
}
