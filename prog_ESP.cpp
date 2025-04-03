//Programa webserver (datalogger ESP32 + LM35)
#include  <Arduino.h>
#include  <WiFi.h>
#include  <WebServer.h>
#include  <driver/adc.h>
#include  "esp_adc_cal.h"

#define pinosensor    ADC1_CHANNEL_6
#define DEFAULT_VREF  3300
esp_adc_cal_characteristics_t adc_chars;

#define SSID  "Smart 4.0 (3)"
#define SENHA "Smart4.0"

WebServer servidor(80);

void  funcaoSensor(){
  uint16_t leitura = adc1_get_raw(pinosensor);
  uint16_t tensao_mv = esp_adc_cal_raw_to_voltage(leitura,&adc_chars);
  float     temp = tensao_mv/10;
  servidor.send(200,"text/plain",String(temp));
}

void  setup(){
  Serial.begin(9600);
  pinMode(26,OUTPUT);
  digitalWrite(26,1);

  WiFi.begin(SSID,SENHA);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("Conectado com sucesso");
  Serial.println(WiFi.localIP());

  servidor.begin();
  servidor.on("/sensor",funcaoSensor);

  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(pinosensor,ADC_ATTEN_DB_11);
  esp_adc_cal_characterize(ADC_UNIT_1,ADC_ATTEN_DB_11,ADC_WIDTH_12Bit,DEFAULT_VREF,&adc_chars);
}

void  loop(){
  servidor.handleClient();
}