#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"

#define DEVICE_ID "esp32_cam_01" //ID CUA CAM 1
#define NODE_RED_IP "192.168.1.97" //Mo CMD va ghi IPconfig, sau do thay thay IP cua may ban (IPv4)
// ===== WIFI =====
#define WIFI_SSID     "ACOM2"
#define WIFI_PASSWORD "acomdaklak7772"

// ===== AI THINKER PINS =====
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

// ===== STREAM HANDLER =====
static esp_err_t stream_handler(httpd_req_t *req) {

  camera_fb_t * fb = NULL;
  char part_buf[64];

  httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");

  while (true) {

    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      continue;
    }

    size_t hlen = snprintf(part_buf, 64,
      "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n",
      fb->len
    );

    if (httpd_resp_send_chunk(req, part_buf, hlen) != ESP_OK) break;
    if (httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len) != ESP_OK) break;
    if (httpd_resp_send_chunk(req, "\r\n", 2) != ESP_OK) break;

    esp_camera_fb_return(fb);

    delay(30);  // ~30 FPS
  }

  return ESP_OK;
}

// ===== START SERVER =====
void startCameraServer() {

  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 81;

  httpd_handle_t server = NULL;

  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };

  if (httpd_start(&server, &config) == ESP_OK) {
    httpd_register_uri_handler(server, &stream_uri);
    Serial.println("Stream Ready: http://<IP>:81/stream");
  }
}
// ========= HEARTBEAT ==========
#include <HTTPClient.h>

unsigned long lastHeartbeat = 0;

void sendHeartbeat() {

  if (millis() - lastHeartbeat < 5000) return;
  lastHeartbeat = millis();

  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;

  String url = "http://" + String(NODE_RED_IP) + ":1880/heartbeat";

  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  String payload = "{";
  payload += "\"device_id\":\"" + String(DEVICE_ID) + "\",";
  payload += "\"status\":\"online\",";
  payload += "\"temperature\":0";
  payload += "}";

  int code = http.POST(payload);

  Serial.print("Heartbeat: ");
  Serial.println(code);

  http.end();
}


void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  // IP tĩnh
  IPAddress local_IP(192,168,1,201);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);
  WiFi.config(local_IP, gateway, subnet);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
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
  config.pin_pwdn  = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 15;
  config.fb_count = 2;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  startCameraServer();
}

void loop() {
  sendHeartbeat();
  delay(1000);
}
