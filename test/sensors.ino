#include <Arduino.h>  // Arduino built in
#include <WiFi.h>     // Arduino built in
#include "FS.h"       // Arduino built in
#include <LITTLEFS.h> // Arduino built in
#include <LittleDB.h> // https://github.com/pouriamoosavi/LittleDB
#include <ezTime.h>   // Arduino built in

const char* ssid = "";
const char* password = "";
String Timestamp, Date, Time;

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
  waitForSync();

  Serial.println("UTC: " + UTC.dateTime());
  
  Timezone America;
  America.setLocation("America/New_York");
  //America.setPosix("EST--5EDT,M3.2.0,M11.1.0/2");
  Serial.println("EST time: " + America.dateTime());
  Serial.println("Time now is:" + America.dateTime("l ~t~h~e jS ~o~f F Y, g:i A") );
                                          //Saturday the 25th of August 2018, 2:23 PM
  Serial.println(" " + America.dateTime("mdyHi") );                                       
  Timestamp = " " + America.dateTime("mdyHi");
  Serial.println(" " + America.dateTime("mdy") );                                       
  Date = " " + America.dateTime("mdy");
  Serial.println(" " + America.dateTime("Hi") );                                       
  Time = " " + America.dateTime("Hi");
  
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
  insert += Date;
  insert += ",";
  insert += Time;
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
  insert += Date;
  insert += ",";
  insert += Time;
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
  insert += Date;
  insert += ",";
  insert += Time;
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
  insert += Date;
  insert += ",";
  insert += Time;
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
  insert += Date;
  insert += ",";
  insert += Time;
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

  String Date = "050821";
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
  
  id = Timestamp;
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

 }  // End of Setup



 
void loop() {

  events();

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
