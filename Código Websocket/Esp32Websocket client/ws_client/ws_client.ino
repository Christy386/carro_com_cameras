#include <ArduinoWebsockets.h>

const char* ssid = "Casa"; // Nome da rede
const char* password = "casalopes3002"; // Senha da rede
const char* websockets_server_host = "192.168.1.5"; // IP do servidor websocket
const int websockets_server_port = 8080; // Porta de conexão do servidor

// Utilizamos o namespace de websocket para podermos utilizar a classe WebsocketsClient
using namespace websockets;

// Objeto websocket client
WebsocketsClient client;

// motor 1
int in1 = 22; 
int in2 = 21;

//motor 2
int in3 = 18;
int in4 = 19;

const int freq = 256;

const int ledChannel1 = 0;
const int ledChannel2 = 1;
const int ledChannel3 = 2;
const int ledChannel4 = 3;

const int resolution = 8;

String cmd = "stop";

void setup() 
{
    // Iniciamos a serial com velocidade de 115200
    Serial.begin(115200);

    // Definimos o pino como saída
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);

    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    //configuração de pwm
    ledcSetup(ledChannel1, freq, resolution);
    ledcSetup(ledChannel2, freq, resolution);
    ledcSetup(ledChannel3, freq, resolution);
    ledcSetup(ledChannel4, freq, resolution);
    
    ledcAttachPin(in1, ledChannel1);
    ledcAttachPin(in2, ledChannel2);
    ledcAttachPin(in3, ledChannel3);
    ledcAttachPin(in4, ledChannel4);

    // Conectamos o wifi
    WiFi.begin(ssid, password);

    // Enquanto não conectar printamos um "."
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    // Exibimos "WiFi Conectado"
    Serial.println("Connected to Wifi, Connecting to server.");

    // Tentamos conectar com o websockets server
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/");

    // Se foi possível conectar
    if(connected) 
    {
        // Exibimos mensagem de sucesso
        Serial.println("Connected!");
        // Enviamos uma msg "Hello Server" para o servidor
        client.send("Hello Server");
        //colocar a função de enviar json aqui
        client.send("{\"cmd\":\"" + cmd + "\"}");// fazer depois a verificação
    }   // Se não foi possível conectar
    else 
    {
        // Exibimos mensagem de falha
        Serial.println("Not Connected!");
        return;
    }
    
    // Iniciamos o callback onde as mesagens serão recebidas
    client.onMessage([&](WebsocketsMessage message)
    {        
        // Exibimos a mensagem recebida na serial
        Serial.print("Got Message: ");
        Serial.println(message.data());

        // Ligamos/Desligamos o led de acordo com o comando
        if(message.data().equalsIgnoreCase("front")){
            cmd = "front";
            Serial.println(cmd);

            ledcWrite(ledChannel1, 0);
            ledcWrite(ledChannel2, 220);

            ledcWrite(ledChannel3, 0);
            ledcWrite(ledChannel4, 220);
            
            /*digitalWrite(in1, LOW);
            digitalWrite(in2, HIGH);

            digitalWrite(in3, LOW);
            digitalWrite(in4, HIGH);*/
            
            //send_json(porta, luz);
            //colocar a função aqui
        }if(message.data().equalsIgnoreCase("back")){
            cmd = "back";
            Serial.println(cmd);

            ledcWrite(ledChannel1, 220);
            ledcWrite(ledChannel2, 0);

            ledcWrite(ledChannel3, 220);
            ledcWrite(ledChannel4, 0);
            
            /*digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);

            digitalWrite(in3, HIGH);
            digitalWrite(in4, LOW);*/
            
            //send_json(porta, luz);
            //colocar a função aqui
        }if(message.data().equalsIgnoreCase("right")){
            cmd = "right";
            Serial.println(cmd);

            ledcWrite(ledChannel3, 0);
            ledcWrite(ledChannel4, 220);
            
            /*digitalWrite(in3, LOW);
            digitalWrite(in4, HIGH);*/
            
            //ledcWrite(ledChannel3, 100);
            //ledcWrite(ledChannel4, 0);
      
            //send_json(porta, luz);
            //colocar a função aqui
        }if(message.data().equalsIgnoreCase("left")){
            cmd = "left";
            Serial.println(cmd);

            ledcWrite(ledChannel1, 0);
            ledcWrite(ledChannel2, 220);
            
            /*digitalWrite(in1, LOW);
            digitalWrite(in2, HIGH);*/
            
            //send_json(porta, luz);
            //colocar a função aqui
        }if(message.data().equalsIgnoreCase("stop")){
            cmd = "stop";
            Serial.println(cmd);
            ledcWrite(ledChannel1, 0);
            ledcWrite(ledChannel2, 0);

            ledcWrite(ledChannel3, 0);
            ledcWrite(ledChannel4, 0);
            
            /*digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
            
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);*/
            //ledcWrite(ledChannel3, 0);
            //ledcWrite(ledChannel4, 0);
            
            //send_json(porta, luz);
            //colocar a função aqui
        }
    });
}

void loop() 
{
    //  De tempo em tempo, o websockets client checa por novas mensagens recebidas
    if(client.available()) {
        client.poll();
    }
    
    delay(50);
}

void send_json(String ps, String ls){
  //client.send("{\"porta\":\"" + ps + "\", \"luz\":\"" + ls + "\"}");// fazer depois a verificação
}
