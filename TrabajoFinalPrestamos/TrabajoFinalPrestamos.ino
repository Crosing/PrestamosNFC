#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
//Definicion de variables y constantes
const byte filas = 4;                    //define las 4 filas del teclado 
const byte columnas = 3;                 //define las 3 columnas del teclado
char teclas[filas][columnas] =           //inicializa filas y columnas
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte pinesfilas[filas] = {21,20,19,18};    //defines los pines digitales de las cuatro fila
byte pinescolumnas[columnas] = {17,16,15}; //defines los pines digitales de las tres columnas
Keypad teclado = Keypad( makeKeymap(teclas), pinesfilas,  pinescolumnas, filas,columnas );
LiquidCrystal lcd(48, 49, 45, 44, 43, 42); //Inicializa la libreria y define los pines digitales para el LCD

byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0F, 0x3A, 0xDC
};
char server[] = "www.mundoviceversa.com";
//IPAddress server(74, 208, 236, 116);
//IPAddress ip(192,168,2,8); //
EthernetClient client;


void setup() {
  Serial.begin(9600);
  //Inicializacion display
  pinMode(47, OUTPUT);
  digitalWrite(47, 0);
  //Inicializacion Ethernet
  lcd.begin(16, 2);
  if (Ethernet.begin(mac) == 0) {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("SIN CONEXION");
    Serial.println("Failed to configure Ethernet using DHCP");
    while(true){};
    //Ethernet.begin(mac, ip);
  }
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("CONECTANDO...");
  Serial.println("connecting...");
  delay(1000);

}


void loop() {
  bienvenida();
  Serial.println(teclados());
}

void bienvenida() {
  lcd.clear();
  lcd.setCursor(3, 0);                //Ubica el cursor en la fila 7 de la columna 0
  lcd.print("SISTEMA DE");               //Muestra el mensaje "GUIA EN"
  lcd.setCursor(3, 1);                //Ubica el cursor en la fila 7 de la columna 1
  lcd.print("PRESTAMOS");
  delay(2000);
  lcd.clear();
}

void prueba() {
  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("GET /APrest.php?Doc=8163847&Ref=1&Cant=120&Prest=1 HTTP/1.1");
    client.println("Host: www.mundoviceversa.com");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("connection failed");
  }
  delay(1000);

  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  while (true) {};
}

int teclados()                            //Funcion teclados
{
  char k;
  int val;  
  k=teclado.getKey();                    /*Llamar la función del teclado y almacenar el valor 
                                         digitado en una variable (k) tipo caracter. Si no se oprime 
                                         ninguna tecla el teclado retornara el carácter nulo.*/

  while((k=='\0')||(k=='#')||(k=='*'))   //Si no se oprime ninguna tecla 
  {
    k=teclado.getKey();                     //Sigue llamando al teclado
  }

  val=k-48;                               //Poner el valor de k menos 48 a val 
  return val;
}

