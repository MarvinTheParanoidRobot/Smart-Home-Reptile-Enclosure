

//flag for saving data
bool shouldSaveConfig = false;




long lastMsg = 0;

String root_will="Home/";
String root_type="Home/";
String root_name="Home/";
String root_timestamp="Home/";

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

  const char* device_type_menu =  "<p>Please select Device configuration:</p>"
                                    "<input style='width: auto; margin: 0 10px 0 10px;' type='radio' id='choice1' name='config_selection' value='Power Plug' checked='checked' ><label for='choice1'>Power Plug</label><br>"
                                    "<input style='width: auto; margin: 0 10px 0 10px;' type='radio' id='choice2' name='config_selection' value='12V Switch'><label for='choice2'>12V Switch</label><br>"
                                    "<input style='width: auto; margin: 0 10px 0 10px;' type='radio' id='choice3' name='config_selection' value='LED Lights'><label for='choice3'>LED Lights</label><br>";
String EspId="No ID";
const char*  device_id;
const char* will_Topic;
const char* device_type_topic;
const char* device_name_topic;     
const char* device_timestamp_topic;
