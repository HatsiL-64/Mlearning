#include <WiFi.h>
#include <WebServer.h>
#include <esp_camera.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

//const char* serverUrl="https://love-unhalting-adhesively.ngrok-free.dev/procesar";
//Se sacaron de los ejemplos de AI thinker
/*
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define LED_GPIO_NUM      33
const int PIN_BOTON=13;
//Declarar el flash
const int FLASH_PIN =4;
/*Creo que esto ya no se ocupa pq voy a tratar de hacer una aplicacion
const char indexHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Procesador de Imágenes</title>
</head>
<body>
  <h1 id="papa">Hola, soy homero chino </h1>
  <button onclick="cambiar()">Cambiar texto</button>
  <script>
    function cambiar(){
      document.getElementById('papa')
      papa="Ya no soy homero chino"
    }
  </script>
</body>
</html>
)rawliteral";
*/
WebServer server(80);
void setup() {
  Serial.begin(115200);
  
  WiFi.softAP("esp32");
  IPAddress IP = WiFi.softAPIP(); 
  Serial.println(IP);

  server.on("/", []() {
    server.send(500, "text/html", indexHTML);
  });
  server.on("/camina", HTTP_GET, [](){
    camera_fb_t *fb = esp_camera_fb_get();
    if(!fb)
    {
      Serial.println("FALLA");
      return;
    }
    server.send_P(200, "image/jpeg", (const char*)fb->buf, fb->len);
    esp_camera_fb_return(fb);
  });
  server.begin();
  }
 /*
void enviarFoto()
{
  Serial.println("Foto");
  //camera_fb_t es el contenedor que guarda lo que captura el sensor de la cámara.
  //Se obtiene con el _get() el contenido de la foto: fb->but y el tamaño de la foto: fb->len
  camera_fb_t * fb = esp_camera_fb_get();
  //Se usa el * fb para guardar la foto en la PSRAM para saber en que parte de la memoria se encuentra la foto
  // el get() Pide la foto

  if(!fb)
  {
    //En caso de no guardarse la foto
    Serial.println("FALLA");
    return;
  }
  //se crea un objeto llamado http
  HTTPClient http;
  //Se da la IP y la ruta
  http.begin(serverUrl);
  http.addHeader("COntent-Type","image/jpeg");
  //Mandar al Flask se envian estas al teléfono
  int httpCodigo = http.POST(fb->buf,fb->len);
  if(httpCodigo>0)
  {
    String respuesta=http.getString();
    Serial.println(respuesta);
  }
  else
  {
    Serial.println(httpCodigo);
  }
  //Se cierra la conexión de WIFI 
  http.end();
  //Se borra de la memoria RAM
  esp_camera_fb_return(fb);
}*/
const int botones = 34; //Para la prueba en mi esp32 nodemcu-32s use este pin, pero para el otro debe cambiar

void loop() {
  //Maneja el servidor
  //server.handleClient();

  //Lee la entrada analogica
  int tension34 = analogRead(botones);
  Serial.println(tension34);
  //Determina el boton pulsado, si no se pulsa nada la señal deberia ser 0, ponemos usn rango de 100
  //el boton A tiene dos resistencias de 10Kohm y el boton B una resistencia de 10Kohm y otra de 5KOHM
  //señalA = (3.3V (10000ohm/21500)ohm) * (4095 / 3.3V) = 1905, con un rango de 100
  //señalB = (3.3V (10000ohm/16000)ohm) * (4095 / 3.3V) = 2559, con un rango de 100
  if(tension34 < 100){
    Serial.println("Ningun boton pulsado");
  }
  else if(tension34 <= 2005 && tension34 >= 1805){
    Serial.println("BotonA oprimido");
  }
  else if(tension34 <= 2659 && tension34 >= 2459){
    Serial.println("BotonB oprimido");
  }

  delay(500);
}