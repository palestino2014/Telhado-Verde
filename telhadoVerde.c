#include <Ultrasonic.h>
#include <SPI.h>
#include <Ethernet.h>

//Define os parametros para o sensor ultrasonico HC-SR04
#define PINO_TRIGGER  6  //Porta ligada ao pino Trigger do sensor
#define PINO_ECHO     7  //Porta ligada ao pino Echo do sensor

#define PINO_TRIGGER1  4  //Porta ligada ao pino Trigger do sensor
#define PINO_ECHO1     5  //Porta ligada ao pino Echo do sensor

#define PINO_TRIGGER2  3  //Porta ligada ao pino Trigger do sensor
#define PINO_ECHO2     2  //Porta ligada ao pino Echo do sensor

//Inicializa os 3 sensores ultrasonic
Ultrasonic ultrasonic(PINO_TRIGGER, PINO_ECHO);
Ultrasonic ultrasonic1(PINO_TRIGGER1, PINO_ECHO1);
Ultrasonic ultrasonic2(PINO_TRIGGER2, PINO_ECHO2);

//Pinos dos reles 
String readString;
int Pin1 = 8; //  Pino digital que aciono o rele
int Pin2 = 9; //  Pino digital que aciono o rele
int Pin3 = 1 ; //  Pino digital que aciono o rele

//Definicoes de IP, mascara de rede e gateway
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192,168,0,199); //Define o endereco IP
IPAddress gateway(192,168,0,1); //Define o gateway
IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede

//Inicializa o servidor web na porta 80
EthernetServer server(80);

void setup()
{
  //Inicializa a interface de rede
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  
  pinMode(Pin1, HIGH);
  pinMode(Pin2, HIGH);
  pinMode(Pin3, HIGH); 
  
}

void loop() {
  
   //declaracao dos pinos que aciona as valvulas  
   pinMode(Pin1, OUTPUT);
   pinMode(Pin2, OUTPUT);
   pinMode(Pin3, OUTPUT);
  
  //configuracao dos sensores ultrassonico
  float cmMsec , cmMsec1 , cmMsec2;  
  long microsec = ultrasonic.timing();
  //Le e armazena as informacoes do sensor ultrasonico 1
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  long microsec1 = ultrasonic1.timing();
  //Le e armazena as informacoes do sensor ultrasonico 2
  cmMsec1 = ultrasonic1.convert(microsec1, Ultrasonic::CM); 
  
  long microsec2 = ultrasonic2.timing();
  //Le e armazena as informacoes do sensor ultrasonico 3
  cmMsec2 = ultrasonic2.convert(microsec2, Ultrasonic::CM); 
 
    EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
       if (readString.length() < 100) {
          readString += c;             
        }

        if (c == '\n') {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
    
           // A partir daqui começa o código html.
    
          client.println("<HTML>");
          client.println("<BODY>");
          client.println("<H1>PROJETO TELHADO VERDE</H1>");                                      
                          
          client.println("<br />");
          
          //Mostra as informacoes lidas pelos sensores ultrasonico
          client.print("Sensor Ultrasonico 1: ");
          client.print(cmMsec);
          client.print(" cm");        
          client.print("<br>");           
          client.print("Sensor Ultrasonico 2: ");
          client.print(cmMsec1);
          client.print(" cm");
          client.print("<br>");                    
          client.print("Sensor Ultrasonico 3: ");
          client.print(cmMsec2);
          client.print(" cm");
          client.print("<br>"); 
          client.println("<hr>");
          
          client.println("<a href=\"/liga1/On\"\">DESLIGAR 1</a>");
          client.println("<br />");
          client.println("<a href=\"/liga1/Off\"\">LIGAR 1</a><br />");   
          client.println("<hr />");
          
          client.println("<a href=\"/liga2/On\"\">DESLIGAR 2</a>");
          client.println("<br />");          
          client.println("<a href=\"/liga2/Off\"\">LIGAR 2</a><br />");   
          client.println("<hr />");
          
          client.println("<a href=\"/liga3/On\"\">DESLIGAR 3</a>");
          client.println("<br />");
          client.println("<a href=\"/liga3/Off\"\">LIGAR 3</a><br />");   
          client.println("<hr />");
                  
          client.println("<meta http-equiv=refresh content=15;URL='//192.168.0.199/'>");
                               
          client.println("</BODY>");
          client.println("</HTML>");
          
          delay(1);
          client.stop();
          
          if(readString.indexOf("liga1/On") > 0)
          {
            digitalWrite(Pin1, HIGH);  // Liga          
          }
          else {
            if(readString.indexOf("liga1/Off") > 0)
            {
              digitalWrite(Pin1, LOW);  // Desliga              
            }
          }
          
          
             if(readString.indexOf("liga2/On") > 0)
          {
            digitalWrite(Pin2, HIGH);  // Liga          
          }
          else {
            if(readString.indexOf("liga2/Off") > 0)
            {
              digitalWrite(Pin2, LOW);  // Desliga              
            }
          }
          
              if(readString.indexOf("liga3/On") > 0)
          {
            digitalWrite(Pin3, HIGH);  // Liga          
          }
          else {
            if(readString.indexOf("liga3/Off") > 0)
            {
              digitalWrite(Pin3, LOW);  // Desliga              
            }
          }
          
          
          
          readString="";    
        }
      }
    }
  } 
 }   
 


