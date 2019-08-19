#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <SSD1306.h>
#include <Servo.h>

//Comentar se for carregar SLAVE
#define MASTER

// Não usar essas portas
#define SCK 5   // GPIO5  SCK
#define MISO 19 // GPIO19 MISO
#define MOSI 27 // GPIO27 MOSI
#define SS 18   // GPIO18 CS
#define RST 14  // GPIO14 RESET
#define DI00 26 // GPIO26 IRQ(Interrupt Request)

#define BAND 433E6 //Frequência do radio - exemplo : 433E6, 868E6, 915E6

// Define o pino do LED para acender, pino do Botao, pino do Servo, Pino Analogico Sharp
#define LED_BUILTIN 2
#define SERVO       13
#define SHARP       12   //Pino Analogico
#define RELE        19

//Constante para informar ao Slave que queremos os dados
const String GETDATA = "getdata";

//Constante que o Slave retorna junto com os dados para o Master
const String SETDATA = "setdata=";

//Contador de Lampadas
const String CONT = "cont=";

int contador = 0;       // Contador de vezes
int posicao  = 0;       // Posicao que o Servo inicia

//Variável para controlar o display
SSD1306 display(0x3c, 4, 15);

void setupDisplay()
{
  //O estado do GPIO16 é utilizado para controlar o display OLED
  pinMode(16, OUTPUT);
  
  //Reseta as configurações do display OLED
  digitalWrite(16, LOW);
  
  //Para o OLED permanecer ligado, o GPIO16 deve permanecer HIGH
  //Deve estar em HIGH antes de chamar o display.init() e fazer as demais configurações,
  //não inverta a ordem
  digitalWrite(16, HIGH);

  //Configurações do display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

//Configurações iniciais do LoRa
void setupLoRa()
{ 
  //Inicializa a comunicação
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);

  //Inicializa o LoRa
  if (!LoRa.begin(BAND))
  {
    //Se não conseguiu inicializar, mostra uma mensagem no display
    display.clear();
    display.drawString(0, 0, "Erro ao inicializar o LoRa!");
    display.display();
    while (1);
  }

  //Ativa o crc
  LoRa.enableCrc();
  
  //Ativa o recebimento de pacotes
  LoRa.receive();
}
