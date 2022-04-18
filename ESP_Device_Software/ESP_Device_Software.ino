 #include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include "Global.h"
#include "Configuration.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <PubSubClient.h>         //https://github.com/knolleary/pubsubclient

#include <NTPClient.h>
#include <WiFiUdp.h>




WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org");

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

//****************************************************************************************************
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //clean config for testing 
  SPIFFS.remove("/config.json");
  
  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonDocument json(1024);
        
        auto error =deserializeJson(json, buf.get());
        serializeJson(json, Serial);
        if (!error) {
          Serial.println("\nparsed json");
          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_user, json["mqtt_user"]);
          strcpy(mqtt_pass, json["mqtt_pass"]);
          strcpy(device_type, json["device_type"]);
          strcpy(device_name, json["device_name"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read



  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqtt_user, 20);
  WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", mqtt_pass, 20);
  WiFiManagerParameter custom_device_name("device name", "device name", device_name, 40);
  WiFiManagerParameter custom_device_type(device_type_menu);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

// Reset Wifi settings for testing  
  wifiManager.resetSettings();

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  //wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  //add all your parameters here
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_pass);
  wifiManager.addParameter(&custom_device_name);
  wifiManager.addParameter(&custom_device_type);
  

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimum quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);
 
  wifiManager.setShowInfoUpdate(false);
  wifiManager.setShowInfoErase(false);
  wifiManager.setMenu(wm_menu);
 
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(mqtt_user, custom_mqtt_user.getValue());
  strcpy(mqtt_pass, custom_mqtt_pass.getValue());
  strcpy(device_name, custom_device_name.getValue());
  strcpy(device_type, custom_device_type.getValue());
 

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonDocument json(1024);
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["mqtt_user"] = mqtt_user;
    json["mqtt_pass"] = mqtt_pass;
    json["device_name"] = device_name;
    json["device_type"] = device_type;
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

 
    serializeJson(json, Serial);
    serializeJson(json, configFile);
    
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  const uint16_t mqtt_port_x = atoi(mqtt_port); 
  client.setServer(mqtt_server, mqtt_port_x);
  itoa(ESP.getChipId(),device_id,10);
  (const char*)device_id;
  
  strcpy(will_Topic,device_id);
  strcat(will_Topic,"/");
  strcat(will_Topic,"device_status");
  (const char*)will_Topic;
  
  strcpy(device_type_topic,device_id);
  strcat(device_type_topic,"/");
  strcat(device_type_topic,"device_type_topic");
  (const char*)device_type_topic;
  
  strcpy(device_name_topic,device_id);
  strcat(device_name_topic,"/");
  strcat(device_name_topic,"device_name_topic");
  (const char*)device_name_topic;
  
  strcpy(device_timestamp,device_id);
  strcat(device_timestamp,"/");
  strcat(device_timestamp,"device_timestamp");
  (const char*)device_timestamp;
  
  timeClient.begin();
  timeClient.setTimeOffset(GMT_offset*3600);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect(device_id, mqtt_user, mqtt_pass, will_Topic, will_QoS, will_Retain, will_Message, clean_Session)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//********************************************************************************************************************************

bool publish_meta_data(){
  timeClient.update();
  if(client.publish(device_type_topic,device_type,device_type_retain) || client.publish(device_name_topic,device_name,device_name_retain) || client.publish(will_Topic,device_status,will_Retain) || client.publish(device_timestamp,(const char *)timeClient.getFormattedTime().c_str(),device_timestamp_retain)){
    return true;
  }
  else{
    return false;
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  


  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    publish_meta_data();

  }
}
