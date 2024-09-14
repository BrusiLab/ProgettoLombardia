String path = "/macros/s/AKfycbyI1p-j9sF1Y0IOqSKa3MiI7LeiccDpDSj9w3I7vVMoFqTjxeNI2fG8OFgAocO-giMi/exec";
String contentType = "application/json";

char ssid[] = SECRET_SSID;  //nome rete
char pass[] = SECRET_PASS;  //password rete

int status = WL_IDLE_STATUS;

StaticJsonBuffer<200> jsonBuffer; //oggetti JSON
String postData;
JsonObject& root = jsonBuffer.createObject();

String tempconnesione;
String humconnessione;

String ftoa(float number, uint8_t precision, uint8_t size) { //da float a str

  String s = "";

  // Negative 
  if (number < 0.0)  {
    s = "-";
    number = -number;
  }

  double rounding = 0.5;
  for (uint8_t i = 0; i < precision; ++i)    rounding /= 10.0;

  number += rounding;
  s += String(uint16_t(number));  // prints the integer part

  if(precision > 0) {
    s += ".";                // prints the decimal point
    uint32_t frac;
    uint32_t mult = 1;
    uint8_t padding = precision -1;
    while(precision--)     mult *= 10;

    frac = (number - uint16_t(number)) * mult;

    uint32_t frac1 = frac;
    while(frac1 /= 10)    padding--;
    while(padding--)      s += "0";

    s += String(frac,DEC) ;  // prints the fractional part
  }

  if (size>0)                // checks size
    if (s.length()>size)        return("#");
    else while(s.length()<size) s = " "+s;

  return s;
}

void connessione(){
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);  // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    delay(5000);
  }

  Serial.println("You're connected to the network");
}

void convertidati(String chiave, String valore) { //crea JSON
  root[chiave] = valore;
  root.printTo(postData);
}

void trasmetti(){
  
  convertidati("mode", "arduino");

  convertidati("bpm", battitistr);
  convertidati("spo2", sp02str);

  itoa(co, costr, 10);
  convertidati("co2", costr);
  itoa(nh3, nh3str, 10);
  convertidati("nh3", nh3str);
  no2str = ftoa(no2, 2, 4);
  convertidati("no2", no2str);
  ozonostr = ftoa(ozono, 2, 4);
  convertidati("o3", ozonostr);
  
  latstr = ftoa(lat, 6, 10);
  convertidati("lat", latstr);
  lonstr = ftoa(lon, 6, 10);
  convertidati("lon", lonstr);

  tempconnessione = ftoa(temperature, 1, 4);
  convertidati("temp", tempconnessione);
  humconnessione = ftoa(humidity, 1, 4);
  convertidati("hum", humconnessione);

  if(pericolo == true){
    convertidati("alert", "true");
  } else {
    convertidati("alert", "false");
  }
  
  client.post(path, contentType, postData);

  postData = ""; //resetta i dati
}