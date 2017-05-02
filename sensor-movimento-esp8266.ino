//Programa : Teste MPU6050 & ESP8266
//Baseado no programa original de FlipFlop

//Inclui as bibliotecas para comunicação I2C e Wifi 
#include<Wire.h>
#include <ESP8266WiFi.h>
 
//Endereço do MPU6050
const int MPU = 0x68;  

//Pino I2C SDA
const int sda_pin = D5; 

//Pino I2C SCL
const int scl_pin = D6; 

//variável para guardar o estado do LED do Node
bool led_sensor = false;

//Variável para armazenar o valor de aceleração do eixo X
int AcX;

//credencias de acesso da rede wi-fi
const char* SSID = "SSID_STRING";
const char* PASSWORD = "PASSWORD_STRING";

// do servidor remoto que receberá os dados
const char* servidor_remoto = "ioteste.pythonanywhere.com";  

//instacia a classe wifi
WiFiClient client;

//função para se conectar na rede wifi
void conecta_wifi(){
  delay(10);
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");
  
  //verifica se já não está conectado
  if(WiFi.status() == WL_CONNECTED){
    return;

  }else{

    WiFi.begin(SSID, PASSWORD);
 
    while(WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
    }
  
    Serial.println();
    Serial.print("Conectado com sucesso na rede: ");
    Serial.println(SSID);
    Serial.print("Seu IP: ");
    Serial.println(WiFi.localIP());
  }  
}

//função para enviar os dados para o servidor remoto
void envia_dado(int movimento){
  //Inicia um cliente para o envio dos dados
  if (client.connect(servidor_remoto,80)) {
    String dado ="movimento=";
           dado += String(movimento);
           dado += "\r\n\r\n";
     client.print("POST /init/default/index HTTP/1.1\n");
     client.print("Host: ioteste.pythonanywhere.com\n");
     client.print("Connection: close\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(dado.length());
     client.print("\n\n");
     client.print(dado);
     Serial.print("Dado enviado com sucesso!");
  }else{
    Serial.print("Não foi possível enviar o dado!");
  }
  client.stop();
}

//função para leitura do MPU6050
void le_sensor(){
    //Acessa o MPU para leitura de dados
    Wire.beginTransmission(MPU);
    //começa a ler do registrador 0x3B ACCEL_XOUT
    Wire.write(0x3B);
    //evita de fechar a comunicação após a escrita
    Wire.endTransmission(false);
    //Solicita os dados do sensor
    Wire.requestFrom(MPU,14,true);  
  
    //Armazena o valor do sensor de aceleração eixo X na variável AcX
    AcX = Wire.read() << 8 | Wire.read();  
     
    //Envia o valor para serial
    Serial.print("AcX = "); Serial.print(AcX);
    Serial.println();

    //envia os dados para o servidor
    envia_dado(AcX);
       
    //altera o estado do led
    led_sensor = !led_sensor;
    //escreve o valor no LED_BUILTIN da placa 
    digitalWrite(LED_BUILTIN, led_sensor);
}

void setup(){
  //inicia a comunicação serial, necessário apenas
  //para ver as saídas no terminal serial
  Serial.begin(115200);
  
  //seta o pino do led do NodeMCU como output
  pinMode(LED_BUILTIN, OUTPUT);

  //conecta-se a rede wifi
  conecta_wifi();

  //inicia a comunicação com o MPU-6050
  Wire.begin(sda_pin, scl_pin);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0); 
  Wire.endTransmission(true);
}


void loop(){
  //executa a função de leitura do MPU6050
  le_sensor();
  //Aguarda 1 segundo e repete o processo
  delay(1000);
}
