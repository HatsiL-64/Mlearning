#include "esp_camera.h"
//para que se conecte a las rede inalámbricas
#include <WiFi.h>
//para enviar fotos
#include <HTTPClient.h>
#include "soc/soc.h"           // Librería para control de registros
#include "soc/rtc_cntl_reg.h"
//para conectarse por Hotspot al teléfono 
const char* ssid="vivo Y22s";
const char* password="braulionoc";
const char* serverUrl="http://192.168.1.81:5000/procesar";
//Se sacaron de los ejemplos de AI thinker
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
const int BOTON_PIN =12;

void setup()
{
  //velocidad 
  Serial.begin(115200);
  //este será el botón de prueba
  pinMode(BOTON_PIN,INPUT_PULLUP);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  //configuraciones para la cámara
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size=FRAMESIZE_VGA;
  config.jpeg_quality=10;
  config.fb_count =1;
  //Para detectar errores en la cámara
  //esp_err_t es para guardar mensajes de error el & es para checar los ajustes previos
  esp_err_t err = esp_camera_init(&config);
  if(err != ESP_OK)
  {
    Serial.printf("Error en la cámara 0x%x",err);
    return;
  }
  //Para buscar la red del teléfono 
  WiFi.begin(ssid,password);
  Serial.print("Conectando...");
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print("");
  }
  Serial.println("Conectado");

}
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
}

void loop()
{
  Serial.println("DISPARO AUTOMÁTICO EN 3 SEGUNDOS...");
  delay(3000); 
  enviarFoto();
}
