#include <Arduino.h>
#include <WiFi.h>
#include "FS.h"
#include <LITTLEFS.h>
#include <LittleDB.h>
#include "ESPDateTime.h"

const char* ssid = "";
const char* password = "";

int Timezone = -5; // Eastern USA Time Zone (-5).
String Hour, Minute, Seconds, Day, Month, Year;

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
  
  Serial.println(DateTime.toString());
  Serial.println(DateTime.toISOString());
  Serial.println(DateTime.toUTCString());
  Serial.println("--------------------");
  Serial.println(DateTime.format(DateFormatter::COMPAT));
  Serial.println(DateTime.format(DateFormatter::DATE_ONLY));
  Serial.println(DateTime.format(DateFormatter::TIME_ONLY));
  Serial.println("--------------------");
  DateTimeParts p = DateTime.getParts();
  Serial.printf("%04d/%02d/%02d %02d:%02d:%02d %ld %+05d\n", p.getYear(),
                p.getMonth(), p.getMonthDay(), p.getHours(), p.getMinutes(),
                p.getSeconds(), p.getTime(), p.getTimeZone());

  Hour = p.getHours();
  Minute = p.getMinutes();
  Seconds = p.getSeconds();
  Day = p.getMonthDay();
  Month = p.getMonth();
  Year = p.getYear();
  Timezone = p.getTimeZone();
  
  Serial.println("--------------------");
  time_t t = DateTime.now();
  Serial.println(DateFormatter::format("%Y/%m/%d %H:%M:%S", t));
  Serial.println(DateFormatter::format("%x - %I:%M %p", t));
  Serial.println(DateFormatter::format("Now it's %F %I:%M%p.", t));
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
  
  execQuery("create db test1");
  execQuery("use db test1");
  
  //execQuery("drop table test_tbl");
  
  String schem = "id id, date id, time id, location id, temperature int, humidity int, pressure int, sinlgleDigit tinyint";
  schem.trim();
  execQuery("create table test_tbl (" + schem + ")");
  
  int8_t it;
  String insert = "insert into test_tbl values (";
  insert += 1;
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

  
  it = execQuery(insert);
  //Serial.println(insertValues);
  //Serial.print("it: ");
  //Serial.println(it);
  //printInsertData();
  
  insert = "insert into test_tbl values (";
  insert += 2;
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
  
  
  it = execQuery(insert);
  //Serial.println(insertValues);
  //Serial.print("it: ");
  //Serial.println(it);
  //printInsertData();

  insert = "insert into test_tbl values (";
  insert += 3;
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
  
  
  it = execQuery(insert);
  //Serial.println(insertValues);
  //Serial.print("it: ");
  //Serial.println(it);
  //printInsertData();
  
  insert = "insert into test_tbl values (";
  insert += 4;
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
  
  
  it = execQuery(insert);
  //Serial.println(insertValues);
  //Serial.print("it: ");
  //Serial.println(it);
  //printInsertData();
  
  listDir(LITTLEFS, "/", 2);
/*
  execQuery("update test_tbl set name=text-12 where id=1");
  execQuery("update test_tbl set name=text-13 where id=1");

  Serial.println("Befor compact");
  listDir(LITTLEFS, "/", 2);
  delay(2000);
  Serial.println("=======================================");

  int8_t ct = execQuery("compact table test_tbl");
  Serial.print("ct: ");
  Serial.println(ct);

  listDir(LITTLEFS, "/", 2);
  */
  delay(2000);
  
  int id = 1;
  String select = "select from test_tbl where id=";
  select = select + id;
  //Serial.println(select);
  Serial.println("=======================================");
  execQuery(select);
  //printSelectData();

  get_data();
  
  
  id++; 
  //String room = "Livingroom";
  select = "select from test_tbl where id=";
  select = select + id;
  //Serial.println(select);
  Serial.println("=======================================");
  execQuery(select);
  //printSelectData();

  get_data();
  
  id++;
  //String Date = "2021-05-06";
  select = "select from test_tbl where id=";
  select = select + id;
  //Serial.println(select);
  Serial.println("=======================================");
  execQuery(select);
  //printSelectData();

  get_data();
  
  
  id++;
  //int Temperature = 62;
  select = "select from test_tbl where id=";
  select = select + id;
  //Serial.println(select);
  Serial.println("=======================================");
  execQuery(select);
  //printSelectData();

  get_data();
  Serial.println("=======================================");
  
  
/*
  it = execQuery("insert into test_tbl values (1234567890abcde, 10, 1, text-1234567890abcde)");
  Serial.print("it: ");
  Serial.println(it);

  int8_t it1 = execQuery("insert into test_tbl values (1234567890abc, 20, 1, text-1234567890abc)");
  Serial.print("it1: ");
  Serial.println(it1);

  int8_t it2 = execQuery("insert into test_tbl values (1234567, 8, 30, text-1234567)");
  Serial.print("it2: ");
  Serial.println(it2);
  
  int8_t it3 = execQuery("insert into test_tbl values (abcdef, 40, 1, text-abcdef)");
  Serial.print("it3: ");
  Serial.println(it3);
  */
  
  String tblPath = prefix + CONNECTED_DB + "/test_tbl";
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
