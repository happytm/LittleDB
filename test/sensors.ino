#include <Arduino.h>
#include <WiFi.h>
#include "FS.h"
#include <LITTLEFS.h>
#include <LittleDB.h>
#include "ESPDateTime.h"

const char* ssid = "";
const char* password = "";

int Timezone = -5; // Eastern USA Time Zone (-5).
String Timestamp, Hour, Minute, Seconds, Day, Month, Year;


void setupDateTime() {
  
 // setup this after wifi connected
 // you can use custom timeZone,server and timeout
   
   DateTime.setTimeZone(Timezone);   // Eastern USA Time Zone (-5).
   DateTime.setServer("us.pool.ntp.org");
   DateTime.begin();  
  // DateTime.begin(15 * 1000);  // NTP time update frequency.
   
  
  if (!DateTime.isTimeValid()) {
   Serial.println("Failed to get time from server.");
  }

  delay(1000);
  
  Serial.println("--------------------");
  time_t t = DateTime.now();
  Serial.println(DateFormatter::format("%m%d%y%H%M%S", t));
  Timestamp = DateFormatter::format("%m%d%y%H%M%S", t);
  Serial.println("--------------------");
}


  
void setup() {
  Serial.begin(115200);
  LITTLEFS.begin(true);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }

  Serial.println("\nConnected to network");
  setupDateTime();
 
  delay(1000);
  LITTLEFS.format();

  int8_t res;
  
  res = execQuery("create db test_db");
  res = execQuery("use db test_db");
  
  res = execQuery("drop table test_tbl");
 
  String schem = "id id, date text, time text, location text, temperature tinyint, humidity tinyint, pressure tinyint, sinlgleDigit tinyint";
  schem.trim();
  res = execQuery("create table test_tbl (" + schem + ")");

  String tblPath = "/test_db/test_tbl";
  String schemPath = "/test_db/s.test_tbl";

  File schemFile = LITTLEFS.open(schemPath);
  if(!schemFile || schemFile.isDirectory()){
   
  }
  if(!schemFile.available()) {
    
  }

  String schemFromFile = schemFile.readString();
  schemFromFile.trim();
  
  schemFile.close();

  String insert = "insert into test_tbl values (";
  insert += Timestamp;
  insert += ",";
  insert += DateTime.format(DateFormatter::DATE_ONLY);
  insert += ",";
  insert += DateTime.format(DateFormatter::TIME_ONLY);
  insert += ", ";
  insert += "Livingroom";
  insert += ", ";
  insert += 59;
  insert += ", ";
  insert += 40;
  insert += ", ";
  insert += 250;
  insert += ")";
  
  res = execQuery(insert);
  
  insert = "insert into test_tbl values (";
  insert += Timestamp;
  insert += ",";
  insert += DateTime.format(DateFormatter::DATE_ONLY);
  insert += ",";
  insert += DateTime.format(DateFormatter::TIME_ONLY);
  insert += ", ";
  insert += "B";
  insert += ", ";
  insert += 60;
  insert += ", ";
  insert += 41;
  insert += ", ";
  insert += 251;
  insert += ")";
  
  res = execQuery(insert);
  
  insert = "insert into test_tbl values (";
  insert += Timestamp;
  insert += ",";
  insert += DateTime.format(DateFormatter::DATE_ONLY);
  insert += ",";
  insert += DateTime.format(DateFormatter::TIME_ONLY);
  insert += ", ";
  insert += "Bedroom";
  insert += ", ";
  insert += 60;
  insert += ", ";
  insert += 41;
  insert += ", ";
  insert += 251;
  insert += ")";
  
  res = execQuery(insert);

  insert = "insert into test_tbl values (";
  insert += Timestamp;
  insert += ",";
  insert += DateTime.format(DateFormatter::DATE_ONLY);
  insert += ",";
  insert += DateTime.format(DateFormatter::TIME_ONLY);
  insert += ", ";
  insert += "Kitchen";
  insert += ", ";
  insert += 61;
  insert += ", ";
  insert += 42;
  insert += ", ";
  insert += 252;
  insert += ")";

  res = execQuery(insert);

  insert = "insert into test_tbl values (";
  insert += Timestamp;
  insert += ",";
  insert += DateTime.format(DateFormatter::DATE_ONLY);
  insert += ",";
  insert += DateTime.format(DateFormatter::TIME_ONLY);
  insert += ", ";
  insert += "Bathroom";
  insert += ", ";
  insert += 62;
  insert += ", ";
  insert += 43;
  insert += ", ";
  insert += 253;
  insert += ")";
  
  res = execQuery(insert);

  Serial.println("=======================================");
  String id = Timestamp;
  String select = "select from test_tbl where id=";
  select = select + id;
  res = execQuery(select);
  get_data();
  Serial.println("=======================================");

  String Date = "2021-05-08";
  select = "select from test_tbl where date=";
  select = select + Date;
  res = execQuery(select);
  get_data();
  Serial.println("=======================================");
 
  String Room = "Bedroom";
  select = "select from test_tbl where location=";
  select = select + Room;
  res = execQuery(select);
  get_data();
  Serial.println("=======================================");
  
  id = "050821130022";
  select = "select from test_tbl where id=";
  select = select + id;
  res = execQuery(select);
  get_data();
  Serial.println("=======================================");
  
  delay(2000);

  //res = execQuery("update test_tbl set location=livingroom where id=1234567890abc");
  //res = execQuery("delete from test_tbl where id=1234567890abc");
  
  tblPath = prefix + CONNECTED_DB + "/test_tbl";
  File tblFile = LITTLEFS.open(tblPath);
  while(tblFile.available()){
    Serial.print(tblFile.read(), HEX);
    Serial.print(" ");
  }
  Serial.println();
  listDir(LITTLEFS, "/", 2);

  
}

void loop() {

  //setupDateTime();
}

void get_data() {
  String date = getText(selectData, "date");
  Serial.print("Date:  "); Serial.println(date); 
  String time = getText(selectData, "time");
  Serial.print("time:  "); Serial.println(time); 
  String location = getText(selectData, "location");
  Serial.print("Location:  "); Serial.println(location); 
  int32_t temperature = getInt32(selectData, "temperature");
  Serial.print("Temperature:  "); Serial.println(temperature); 
  int32_t humidity = getInt32(selectData, "humidity");
  Serial.print("Humidity:  "); Serial.println(humidity);
  int32_t pressure = getInt32(selectData, "pressure");
  Serial.print("Pressure:  "); Serial.println(pressure);
 }
