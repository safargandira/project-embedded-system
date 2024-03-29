#include <CTBot.h>

CTBot myBot;
String ssid = " ";
String pass = " ";
String Token = " ";

int userIDs[] = { ,  ,  }; 

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
