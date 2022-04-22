
//============ MQTT SERVER Default Values========================
#define mqtt_server               "xxx.cloudmqtt.com"   //mqtt server
#define mqtt_port                 "12345"               //mqtt port number usually 8123
#define mqtt_user                 "Username"           //mqtt username
#define mqtt_pass                 "Password"           //mqtt password
#define device_type               "Power Plug"          //device type
#define device_name               "Device0"             //device id
#define device_status             "Online"              //device status
#define will_QoS                  0                     //will QoS level 
#define will_Retain               true                  //will Retain flag
#define will_Message              "Offline"             //will message 
#define device_type_retain        true                  //device type retain flag
#define device_name_retain        true                  //device name retain flag
#define device_timestamp_retain   true                  //device last timestamp retain flag
#define clean_Session             true                  //set device to clean session since its only publishing
#define GMT_offset                1                     //timezone offset from GMT
