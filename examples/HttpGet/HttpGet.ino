/*
  你可以使用的頁面：
    /SSID     ---WiFi資訊設定(在WiFi未成功連接時預設跳轉此頁面)
    /Firmware ---韌體更新(在WiFi成功連接時預設跳轉此頁面)
    /Debug    ---監控頁面
    /Reset    ---重新啟動ESP8266
*/

#include <ESP8266WebOTA.h>

#include <ESP8266HTTPClient.h>

void setup()
{
  Serial.begin(115200);

  //裝置名稱 (SSID & 登入帳號), 密碼, LED腳位(ESP01->2, ESP12->1)
  WebOTA.begin("DeviceName", "Password", 2);

  //紀錄「Ready」到 /Debug 頁面
  WebOTA.print("Ready");
}

int Last = 0;
unsigned long timer = 0;
void loop()
{
  if (WebOTA.check()) //檢查WiFi是否連線
  {
    //WiFi正常連線

    unsigned long Time = millis();
    if (timer <= Time)
    {
      timer = Time + 5000;
      String T = HttpGet("http://www.hkdc.tw/Database?Name=BB6XZOQ655ELDMCW4MUJP7RLKXOYLP&Value=" + String(Last));
      Last = millis() - Time;
      WebOTA.print(T);
      WebOTA.print(String(Last) + "ms", true); //true代表靠右對齊；false代表靠左
    }

  }
  else
  {
    //WiFi已斷線

    Serial.println("WiFi Disconnected");
  }
}

int SendCount = 0; //傳送數量
int LoseCount = 0; //傳送失敗數量
String HttpGet(String URL)
{
  WebOTA.print(String(SendCount) + " " + String(LoseCount));
  SendCount++;
  WiFiClient client;
  HTTPClient http;
  if (http.begin(client, URL))
  {
    if (http.GET() == 200)
      return http.getString();
    else
      WebOTA.print("ServerError", true);
    http.end();
  }
  else
  {
    WebOTA.print("ConnectError", true);
  }
  LoseCount++;
  return "";
}
