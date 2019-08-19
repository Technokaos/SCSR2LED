//Compila apenas se MASTER não estiver definido no arquivo principal
#ifndef MASTER

//Contador que irá servir como o dados que o Slave irá enviar
int count = 0;

//Tempo do último envio
long lastSendTime = 0;

void setup(){
    Serial.begin(115200);
    //Chama a configuração inicial do display
    setupDisplay();
    //Chama a configuração inicial do LoRa
    setupLoRa();
    display.clear();
    display.drawString(0, 0, "Slave esperando...");
    display.display();
}

void loop()
{
  //Tenta ler o pacote
  /*int packetSize = LoRa.parsePacket();

  //Verifica se o pacote possui a quantidade de caracteres que esperamos
  if (packetSize == GETDATA.length())
  {
    String received = "";

    //Armazena os dados do pacote em uma string
    while(LoRa.available())
    {
      received += (char) LoRa.read();
    }

    if(received.equals(GETDATA))
    {
      //Simula a leitura dos dados
      String data = readData();
      Serial.println("Criando pacote para envio");
      
      //Cria o pacote para envio
      LoRa.beginPacket();
      LoRa.print(SETDATA + data);
      
      //Finaliza e envia o pacote
      LoRa.endPacket();
      
      //Mostra no display
      display.clear();
      display.drawString(0, 0, "Enviou: " + String(data));
      display.display();
    }
  }

  receiveCont();*/

  //Tentamos ler o pacote
  int packetSize = LoRa.parsePacket();
  
  //Verificamos se o pacote tem o tamanho mínimo de caracteres que esperamos
  if (packetSize > CONT.length())
  {
    String received = "";
   
    //Armazena os dados do pacote em uma string
    while(LoRa.available())
    {
      received += (char) LoRa.read();
    }
    
    //Verifica se a string possui o que está contido em "SETDATA"
    int index = received.indexOf(CONT);
    
    if(index >= 0)
    {
      //Recuperamos a string que está após o "SETDATA",
      //que no caso serão os dados de nosso interesse
      String data = received.substring(CONT.length());
      
      //Mostra no display os dados e o tempo que a operação demorou
      display.clear();
      display.drawString(0, 0, "Contador: " + data);
      display.display();
    }
  }  
}

//Função onde se faz a leitura dos dados que queira enviar
//Poderia ser o valor lido por algum sensor por exemplo
//Aqui vamos enviar apenas um contador para testes
//mas você pode alterar a função para fazer a leitura de algum sensor
String readData(){
  return String(count++);
}

void receiveCont()
{
  //Tentamos ler o pacote
  int packetSize = LoRa.parsePacket();
  
  //Verificamos se o pacote tem o tamanho mínimo de caracteres que esperamos
  if (packetSize > CONT.length())
  {
    String received = "";
   
    //Armazena os dados do pacote em uma string
    while(LoRa.available())
    {
      received += (char) LoRa.read();
    }
    
    //Verifica se a string possui o que está contido em "SETDATA"
    int index = received.indexOf(CONT);
    
    if(index >= 0)
    {
      //Recuperamos a string que está após o "SETDATA",
      //que no caso serão os dados de nosso interesse
      String data = received.substring(CONT.length());
      
      //Tempo que demorou para o Master criar o pacote, enviar o pacote,
      //o Slave receber, fazer a leitura, criar um novo pacote, enviá-lo
      //e o Master receber e ler
      String waiting = String(millis() - lastSendTime);
      
      //Mostra no display os dados e o tempo que a operação demorou
      display.clear();
      display.drawString(0, 0, data);
      display.display();

      Serial.println("Recebeu Contador: " + data);
    }
  }
}

#endif
