#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

// Configuración de la cámara (ESP32-CAM con OV2640)
/*#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM      5
#define PCLK_GPIO_NUM    22
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
*/

#define PWDN_GPIO_NUM  32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  0
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27

#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    21
#define Y4_GPIO_NUM    19
#define Y3_GPIO_NUM    18
#define Y2_GPIO_NUM    5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22

//#define BOTON 13

// Crear servidor web en puerto 80
WebServer server(80);

void handleCapture() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "Error al capturar la foto");
    return;
  }

  WiFiClient client = server.client();

  // Cabeceras HTTP
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: image/jpeg");
  client.println("Content-Length: " + String(fb->len));
  client.println("Connection: close");
  client.println();

  // Enviar la imagen
  client.write(fb->buf, fb->len);

  esp_camera_fb_return(fb);
}



void setup() {
  //pinMode(BOTON, INPUT_PULLUP);
  Serial.begin(115200);

  // Configuración de la cámara
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
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_XGA; // resolución
  config.jpeg_quality = 8;
  config.fb_count = 2;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Error al inicializar la cámara");
    return;
  }
  
  sensor_t * s = esp_camera_sensor_get();
  s->set_brightness(s, 0);   // -2 a 2
  s->set_contrast(s, 2);     // -2 a 2
  s->set_saturation(s, 0);   // -2 a 2
  s->set_gainceiling(s, GAINCEILING_32X);

  // Crear red WiFi propia
  WiFi.softAP("ESP32-CAM", "12345678");
  Serial.println("Access Point iniciado");
  Serial.println(WiFi.softAPIP()); // normalmente 192.168.4.1

  // Definir ruta del servidor
  /*server.on("/", [](){
      server.send(200, "text/html", "<h1>Hola</h1>");    
    });*/
  server.on("/", handleCapture);  
  server.begin();
}

void loop() {
  server.handleClient();
  /*if(digitalRead(BOTON) == LOW){
    handleCapture();
  }*/
}
