#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PZEM004Tv30.h>

PZEM004Tv30 pzem(10, 11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

float costPerKWh = 1352.0;
unsigned long previousMillis = 0;
const long interval = 2000;
const long warningInterval = 10000;
unsigned long previousWarningMillis = 0;
unsigned long count = 0;
unsigned long displayIndex = 0;

float sumPower = 0.0;
float power = 0.0;
float energy = 0.0;
float averagePower = 0.0;
float averageEnergy = 0.0;
unsigned long startTime = 0;
unsigned long activeStartTime = 0;
float totalCost = 0.0;

bool displayingWarning = false;

void setup() {
  Serial1.begin(115200);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Loading...");

  Serial3.begin(115200);
  delay(2000);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    power = pzem.power();
    energy = pzem.energy()/1000.0;

    if (power == 0.0) {
      count = 0;
      sumPower = 0.0;
      startTime = currentMillis;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Daya Kosong");
    } else {
      count++;
      sumPower += power;
      activeStartTime = (currentMillis-startTime)/1000;
      averagePower = sumPower / count;
      averageEnergy = (energy * 3600.0) / activeStartTime;
      totalCost = energy * costPerKWh;

      if (!displayingWarning) {
        switchDisplay();
      }

      if (power >= (0.9 * 200)) {
        unsigned long currentWarningMillis = millis();
        if (currentWarningMillis - previousWarningMillis >= warningInterval) {
          previousWarningMillis = currentWarningMillis;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Lebih dari 90%");
          Serial3.print("Penggunaan daya melebihi 90 %! Daya saat ini: ");
          Serial3.print(power);
          Serial3.print(" W");
          displayingWarning = true;
        }
      } else {
        displayingWarning = false;
      }
    }
  }
}

void switchDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);

  switch (displayIndex) {
    case 0:
      lcd.print("Daya: ");
      lcd.print(power, 3);
      lcd.print(" W");
      break;
    case 1:
      lcd.print("kWh: ");
      lcd.print(energy, 4);
      lcd.print(" kWh");
      break;
    case 2:
      lcd.print("Rata2 Daya: ");
      lcd.setCursor(0, 1);
      lcd.print(averagePower, 3);
      lcd.print(" Watt");
      break;
    case 3:
      lcd.print("Rata2 Energi: ");
      lcd.setCursor(0, 1);
      lcd.print(averageEnergy, 4);
      lcd.print(" kWh");
      break;
    case 4:
      lcd.print("Waktu Aktif: ");
      lcd.setCursor(0, 1);
      lcd.print(activeStartTime / 3600);
      lcd.print(" Jam ");
      lcd.print((activeStartTime % 3600) / 60);
      lcd.print(" Menit");
      break;
    case 5:
      lcd.print("Total Biaya: ");
      lcd.setCursor(0, 1);
      lcd.print("Rp ");
      lcd.print(formatCurrency(totalCost));

      break;
  }
  displayIndex = (displayIndex + 1) % 6;
  delay(2000);  

String formatCurrency(float amount) {
  String formattedAmount = String(amount, 2);
  int commaIndex = formattedAmount.indexOf('.');
  String rupiah = formattedAmount.substring(0, commaIndex);
  String cents = formattedAmount.substring(commaIndex + 1);
  formattedAmount = rupiah + "." + cents;
  return formattedAmount;
}
