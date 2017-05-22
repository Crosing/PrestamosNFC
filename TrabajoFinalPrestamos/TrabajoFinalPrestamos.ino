/*
  PINOUT:
  RC522 MODULE    MEGA
  SDA             D48
  SCK             D52
  MOSI            D51
  MISO            D50
  IRQ             N/A
  GND             GND
  RST             D49
  3.3V            3.3V
*/
#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
//Definicion de variables y constantes
const byte filas = 4;                    //define las 4 filas del teclado
const byte columnas = 4;                 //define las 3 columnas del teclado
char teclas[filas][columnas] =           //inicializa filas y columnas
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte pinesfilas[filas] = {2, 3, 4, 5}; //defines los pines digitales de las cuatro fila
byte pinescolumnas[columnas] = {6, 7, 8, 9}; //defines los pines digitales de las tres columnas
Keypad teclado = Keypad( makeKeymap(teclas), pinesfilas,  pinescolumnas, filas, columnas );
LiquidCrystal lcd(44, 43, 42, 41, 40, 39); //Inicializa la libreria y define los pines digitales para el LCD
int t = 0;
String vari[10];

#define ETHNET_CS 10
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0F, 0x3A, 0xDC
};
char server[] = "www.mundoviceversa.com";
//IPAddress server(74, 208, 236, 116);
//IPAddress ip(192,168,2,8); //
EthernetClient client;

/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
#define SDA_DIO 48
#define RESET_DIO 49
/* Create an instance of the RFID library */
MFRC522 mfrc522(SDA_DIO, RESET_DIO);


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  //Inicializacion display
  //pinMode(47, OUTPUT);
  //digitalWrite(47, 0);
  //Inicializacion Ethernet
  lcd.begin(20, 4);

  setupEth();
  SPI.begin();
  spiSelect(SDA_DIO);
  mfrc522.PCD_Init();
}


void loop() {
  Serial.println("entró");
  bienvenida();
  t = numeros();
  Serial.println(t);
  if (t == 0) {
    prestar();
  }
  if (t == 0) {
    devolver();
  }
  if (t == 2) {
    registrarP();
  }
  if (t == 3) {
    registrarI();
  }
}

void bienvenida() {
  lcd.clear();
  lcd.setCursor(3, 0);                //Ubica el cursor en la fila 7 de la columna 0
  lcd.print("SISTEMA DE");               //Muestra el mensaje "GUIA EN"
  lcd.setCursor(3, 1);                //Ubica el cursor en la fila 7 de la columna 1
  lcd.print("PRESTAMOS");
}

void php(String funcion) {
  spiSelect(ETHNET_CS);
  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.print("GET ");
    client.print(funcion);
    client.println(" HTTP/1.1");
    client.println("Host: www.mundoviceversa.com");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("connection failed");
  }
  delay(1000);
  String aux = "";
  char c;
  int cont = 0;
  while (client.available()) {
    c = client.read();
    if (c == '#') {
      while (c != '&') {
        c = client.read();
        aux = aux + c;
      }
      aux.replace("&", "");
      vari[cont] = aux;
      cont++;
      aux = "";
    }
    Serial.print(c);
  }
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

}

int numeros()                            //Funcion teclados
{
  char k;
  int val;
  k = teclado.getKey();                    /*Llamar la función del teclado y almacenar el valor                                      digitado en una variable (k) tipo caracter. Si no se oprime
                                         ninguna tecla el teclado retornara el carácter nulo.*/

  while ((k == '\0') || (k == '#') || (k == '*') || (k == 'A') || (k == 'B') || (k == 'C') || (k == 'D')) //Si no se oprime ninguna tecla
  {
    k = teclado.getKey();                   //Sigue llamando al teclado
  }

  val = k - 48;                           //Poner el valor de k menos 48 a val
  return val;
}
void imp(String a, String b, String c, String d ) {
  lcd.clear();
  lcd.setCursor((20 - a.length()) / 2, 0);            //Ubica el cursor en la fila 7 de la columna 0
  lcd.print(a);               //Muestra el mensaje "GUIA EN"
  lcd.setCursor((20 - b.length()) / 2, 1);            //Ubica el cursor en la fila 7 de la columna 1
  lcd.print(b);
  lcd.setCursor((20 - c.length()) / 2, 2);            //Ubica el cursor en la fila 7 de la columna 0
  lcd.print(c);               //Muestra el mensaje "GUIA EN"
  lcd.setCursor((20 - d.length()) / 2, 3);            //Ubica el cursor en la fila 7 de la columna 1
  lcd.print(d);
}
void prestar() {
  imp("PONGA SU CARNÉ", "", "", "");
  Serial.println("PONGA SU CARNÉ");
  String uid = UID(5000);
  if (uid != "tiempoAgotado") {
    Serial.println(uid);
    String f = "/EDocNomxUID.php?UID=";
    uid = espaciosurl(uid);
    Serial.println(f + uid);
    php(f + uid);
    Serial.println(vari[0]);
    Serial.println(vari[1]);
    f = "/APrest.php?Doc="+vari[0]+"&Ref=1&Cant=400&Prest=1";
    Serial.println(f);
    php(f);
    
  }
  else {
    Serial.println("TIEMPO AGOTADO");
    return;
  }


  return;
}
void devolver() {
  imp("PONGA SU CARNÉ", "", "", "");
}
void registrarP() {
  imp("INGRESE CÉDULA", "Y", "PONGA SU CARNÉ", "");
}
void registrarI() {
  imp("INGRESE REFERENCIA", "Y", "PONGA SU TAG", "");
}

void setupEth() {
  spiSelect(ETHNET_CS);
  if (Ethernet.begin(mac) == 0) {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("SIN CONEXION");
    Serial.println("Failed to configure Ethernet using DHCP");
    while (true) {};
    //Ethernet.begin(mac, ip);
  }
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("CONECTANDO...");
  Serial.println("connecting...");
  delay(1000);
}
String espaciosurl(String u) {
  u.replace(" ", "%20");
  return u;
}

void spiSelect(int CS) {
  // disable all SPI
  pinMode(SDA_DIO, OUTPUT);
  pinMode(ETHNET_CS, OUTPUT);
  digitalWrite(SDA_DIO, HIGH);
  digitalWrite(ETHNET_CS, HIGH);
  // enable the chip we want
  digitalWrite(CS, LOW);
}

String UID(int tiempo) {
  spiSelect(SDA_DIO);
  unsigned long int tIni = millis();

  while (millis() - tIni < tiempo) {

    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

      String ret = "";
      //Serial.println("UID: ");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (i > 0) {
          ret.concat(" ");
        }
        if (mfrc522.uid.uidByte[i] < 0x10) {
          ret.concat(F("0"));
        }
        //Serial.print(F(" "));
        ret.concat(String(mfrc522.uid.uidByte[i], HEX));
        //Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
      ret.toUpperCase();

      return ret;
    }

  }
  return "tiempoAgotado";
}



