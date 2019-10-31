#include "ESP8266WebOTA.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
WiFiMode_t WiFiMode;
ESP8266WebServer Server(80);
ESP8266HTTPUpdateServer Updater;
String Debug = "";
char *DeviceName;
char *DevicePassword;
int LED = 2;

void handleSave()
{
  String SSID = "", PWD = "";
  for (int i = 0; i < Server.args(); i++)
    if (Server.argName(i) == "SSID")
      SSID = Server.arg(i);
    else if (Server.argName(i) == "PWD")
      PWD = Server.arg(i);
  char SSIDc[SSID.length() + 1], PWDc[PWD.length() + 1];
  SSID.toCharArray(SSIDc, SSID.length() + 1);
  PWD.toCharArray(PWDc, PWD.length() + 1);
  Serial.println(SSIDc);
  Serial.println(PWDc);
  WiFi.begin(SSIDc, PWDc);
  Server.send(200, "text/html", "<Meta name=\"viewport\" content=\"width=device-width; initial-scale=1.0\">Your changes have been saved!<br>SSID : " + SSID + "<br>Password : " + PWD);
}

void handleOther()
{
  char temp[400];
  if (WiFiMode == WIFI_STA)
    snprintf(temp, 400, "<script>location.href='/Firmware';</script>");
  else
    snprintf(temp, 400, "<script>location.href='/SSID';</script>");
  Server.send(200, "text/html", temp);
}

void handleSSID()
{
  String WiFiList = "";
  char temp[400];
  snprintf(temp, 400, "<Meta name=\"viewport\" content=\"width=device-width; initial-scale=1.0\">\
            Wi-Fi SSID:<input type=\"text\" value=\"\" id=\"SSID\" /><br>\
            Wi-Fi Password:<input type=\"password\" value=\"\" id=\"PWD\" /><br>\
            <input type=\"button\" value=\"Save Change\" onclick=\"location.href='Save?SSID='+document.getElementById('SSID').value+'&PWD='+document.getElementById('PWD').value;\" />");
  Server.send(200, "text/html", temp);
}

void handleReset()
{
  char temp[400];
  snprintf(temp, 400, "<script>location.href='/Debug';</script>");
  Server.send(200, "text/html", temp);
  delay(1000);
  ESP.restart();
}

void handleDebug()
{
  char SSID[20];
  char MAC[17];
  char temp[500];
  WiFi.SSID().toCharArray(SSID, 20);
  WiFi.macAddress().toCharArray(MAC, 20);
  char debug[Debug.length() + 1];
  Debug.toCharArray(debug, Debug.length() + 1);
  snprintf(temp, 500, "<META http-equiv='refresh' content='2;URL=/Debug'><div>\n<div align=center>%sonnect<br>SSID:%s<br>MAC:%s<br>IP:%d.%d.%d.%d<br>Time:%dm %ds</div>\n%s</div>",
           (WiFi.status() == WL_CONNECTED ? "C" : "Disc"),
           SSID,
           MAC,
           WiFi.localIP()[0],
           WiFi.localIP()[1],
           WiFi.localIP()[2],
           WiFi.localIP()[3],
           ((int)millis() / 60000),
           ((int)millis() / 1000 % 60),
           debug);
  Server.send(200, "text/html", temp);
}

ESP8266WebOTA::ESP8266WebOTA() {}

void ESP8266WebOTA::print(String Content, bool Right)
{
  String Temp = String("<div align=") + (Right ? "right" : "left") + ">" + Content + "</div>\n";
  int index = 0;
  for (int i = 0; i < 9; i++)
  {
    int nindex = Debug.indexOf("\n", index) + 1;
    if (nindex == 0 || Temp.length() > 300)
      break;
    Temp += Debug.substring(index, nindex);
    index = nindex;
  }
  Debug = Temp;
}

void ESP8266WebOTA::begin(char *Name, char *Password, int led)
{
  DeviceName = Name;
  DevicePassword = Password;
  LED = led;
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  WiFiMode = WIFI_AP_STA;
  String APname = (String)DeviceName + "-" + WiFi.macAddress().substring(9, 17);
  APname.replace(":", "");
  char APnameC[APname.length() + 1];
  APname.toCharArray(APnameC, APname.length() + 1);
  WiFi.softAP(APnameC, DevicePassword);
  WiFi.mode(WiFiMode);
  Server.on("/Save", handleSave);
  Server.on("/SSID", handleSSID);
  Server.on("/Reset", handleReset);
  Server.on("/Debug", handleDebug);
  Server.onNotFound(handleOther);
  Updater.setup(&Server, "/Firmware", DeviceName, DevicePassword);
  Server.begin();
  Serial.print("Ready:");
  Serial.println(WiFi.macAddress());
}

bool ESP8266WebOTA::check()
{
  Server.handleClient();
  if (WiFi.status() == WL_CONNECTED)
  {
    if (WiFiMode == WIFI_AP_STA)
    {
      WiFiMode = WIFI_STA;
      WiFi.mode(WiFiMode);
      digitalWrite(LED, HIGH);
      Serial.println(WiFi.localIP());
    }
    return true;
  }
  else
  {
    if (WiFiMode == WIFI_STA)
    {
      WiFiMode = WIFI_AP_STA;
      WiFi.mode(WiFiMode);
      digitalWrite(LED, LOW);
      Serial.print("Disconnect");
    }
    return false;
  }
}

ESP8266WebOTA WebOTA;