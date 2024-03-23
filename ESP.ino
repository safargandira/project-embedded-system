#include <CTBot.h>

CTBot myBot;
String ssid = "FAIZ";
String pass = "gajahrenang";
String Token = "6758670115:AAEB5jTFzd0mSRLwpvOrT81tqq0v7ncGwgg";

int userIDs[] = {1484126714, 1397805605, 5958357649 }; 

void setup() {
  Serial.begin(115200);
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(Token);

  while(!myBot.testConnection()) {
    myBot.wifiConnect(ssid, pass);
    myBot.setTelegramToken(Token);
  }
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    sendToAllUsers(data);
  }
}

void sendToAllUsers(String message) {
  for (int i = 0; i < sizeof(userIDs) / sizeof(userIDs[0]); i++) {
    myBot.sendMessage(userIDs[i], message);
    delay(1000); 
  }
}
