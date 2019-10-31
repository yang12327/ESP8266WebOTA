# ESP8266WebOTA（Arduino Library）
ESP8266透過網頁升級韌體/變更WiFi設定/Debug偵錯頁面

需要搭配 [Generic ESP8266 Module (2.5.2)](https://arduino-esp8266.readthedocs.io/en/2.5.2/installing.html) 使用
  
## 你可以使用的頁面：
   * /SSID     ---WiFi資訊設定(在WiFi未成功連接時預設跳轉此頁面)
   * /Firmware ---韌體更新(在WiFi成功連接時預設跳轉此頁面)
   * /Debug    ---監控頁面
   * /Reset    ---重新啟動ESP8266
   
## 你可以使用的Method：
   * WebOTA.begin("DeviceName", "Password", LED_Pin)    ---初始化
   * WebOTA.check()   ---檢查狀態
   * WebOTA.print("Hello World", true/false)    ---true代表靠右對齊；false代表靠左
   
## 更新紀錄：
