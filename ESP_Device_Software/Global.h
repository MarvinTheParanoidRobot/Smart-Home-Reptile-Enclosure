

//flag for saving data
bool shouldSaveConfig = false;

const char* device_type_menu = "<br/><label for='device_type'>Choose the device type:</label><br/><select name='device_type' id='device_type'><br/><option value='Power Plug'>Power Plug</option><br/><option value='12V Switch'>12V Switch</option><br/><option value='LED Lights' selected>LED Lights</option></select>";
std::vector<const char *> wm_menu  = {"wifi", "exit"};

 long lastMsg = 0;

char*  device_id;
char* will_Topic;
char* device_type_topic;
char* device_name_topic;
char* device_timestamp;
//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
