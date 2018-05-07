//#Programa: NodeMCU e MQTT - Controle e Lâmpada IoT

#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient


#define TOPICO_SUBSCRIBE "lightwifi/1"     //tópico MQTT de escuta
#define TOPICO_PUBLISH   "lightwifi/1"    //tópico MQTT de envio de informações para Broker
#define ID_MQTT  "clientId-adrianobeserra0"     //id mqtt (para identificação de sessão)

//mapeamento de pinos do NodeMCU
#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1


// WIFI
const char* SSID = "ADRNET";
const char* PASSWORD = "12345678adr";

// MQTT
const char* BROKER_MQTT = "broker.mqttdashboard.com";
int BROKER_PORT = 1883;

WiFiClient espClient;
PubSubClient MQTT(espClient); 
char EstadoSaida = '0';  
//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
void InitOutput(void);



void setup()
{
  //inicializações:
  InitOutput();
  initSerial();
  initWiFi();
  initMQTT();
}

void initSerial()
{
  Serial.begin(115200);
}

void initWiFi()
{
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");

  reconectWiFi();
}

void initMQTT()
{
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
  MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
  Serial.println("Callback chamado");
  String msg;

  //obtem a string do payload recebido
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    msg += c;
  }

  if (msg.equals("L"))
  {
    digitalWrite(D0, LOW);
    EstadoSaida = '1';
  }

  //verifica se deve colocar nivel alto de tensão na saída D0:
  if (msg.equals("D"))
  {
    digitalWrite(D0, HIGH);
    EstadoSaida = '0';
  }
  Serial.println(msg);

}

void reconnectMQTT()
{
  while (!MQTT.connected())
  {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT))
    {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_SUBSCRIBE);
    }
    else
    {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Havera nova tentatica de conexao em 2s");
      delay(2000);
    }
  }
}

void reconectWiFi()
{
  //se já está conectado a rede WI-FI, nada é feito.
  //Caso contrário, são efetuadas tentativas de conexão
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());
}

void VerificaConexoesWiFIEMQTT(void)
{
  if (!MQTT.connected())
    reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita

  reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

void EnviaEstadoOutputMQTT(void)
{
  if (EstadoSaida == '0') { }
    //MQTT.publish(TOPICO_PUBLISH, "D");

  if (EstadoSaida == '1') { }
    //MQTT.publish(TOPICO_PUBLISH, "L");

  Serial.println("- Estado da saida D0 enviado ao broker!");
  delay(1000);
}

void InitOutput(void)
{
  pinMode(D0, OUTPUT);
  digitalWrite(D0, HIGH);
}


//programa principal
void loop()
{
  VerificaConexoesWiFIEMQTT();
  EnviaEstadoOutputMQTT();
  MQTT.loop();
}

