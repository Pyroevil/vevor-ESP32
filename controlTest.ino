/* 
Special thanks to spin877 on GitHub with it's codes that help me a lot with UUID and commands.
spin877 Bruciatore_BLE project : https://github.com/spin877/Bruciatore_BLE
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Adresse MAC du périphérique BLE à connecter
const char* deviceAddress = "01:B6:EC:DA:07:BA"; // Remplacez par l'adresse MAC du périphérique BLE distant
// UUID du service et de la caractéristique
const char* serviceUUID = "0000ffe0-0000-1000-8000-00805f9b34fb"; // Remplacez par l'UUID du service
const char* characteristicUUID = "0000ffe1-0000-1000-8000-00805f9b34fb"; // Remplacez par l'UUID de la caractéristique
byte common_bytes [] = {0xaa, 0x55, 0x0c, 0x22};
byte cmd1 [] = {0xaa, 0x55, 0x0c, 0x22,0x01, 0x00, 0x00, 0x2f};//common_bytes + {0x01, 0x00, 0x00, 0x2f};
byte cmdoff [] = {0xaa, 0x55, 0x0c, 0x22, 0x03, 0x00, 0x00, 0x31};
byte power6 [] = {0xaa, 0x55, 0x0c, 0x22, 0x04, 0x06, 0x00, 0x38};
byte value [19];

// Fonction pour gérer les événements BLE
class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pClient) {
        Serial.println("Connected to BLE Server");
    }

    void onDisconnect(BLEClient* pClient) {
        Serial.println("Disconnected from BLE Server");
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Client...");

  // Initialisation de l'ESP32 BLE
  BLEDevice::init("");

  // Création d'un objet BLEClient
  BLEClient* pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  Serial.println("Connecting to BLE Server...");
  // Connexion au périphérique BLE distant
  if (!pClient->connect(BLEAddress(deviceAddress))) {
    Serial.println("Failed to connect to BLE Server");
    return;
  }

  delay(2000);

  // Obtention d'un pointeur sur le service recherché
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);

  if (pRemoteService == nullptr) {
    Serial.println("Failed to find the service UUID");
    pClient->disconnect();
    return;
  }

  // Obtention d'un pointeur sur la caractéristique recherchée
  BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(characteristicUUID);
  
  if (pRemoteCharacteristic == nullptr) {
    Serial.println("Failed to find the characteristic UUID");
    pClient->disconnect();
    return;
  }
  for(int ii = 0;ii<10;ii++){
    Serial.println("Send characteristic CDM1 value...");
    pRemoteCharacteristic->writeValue(cmd1, sizeof(cmd1));
    std::string value = pRemoteCharacteristic->readValue();
    Serial.println(value.c_str());
    delay(1000);

    Serial.println("Read to characteristic value...");
    // Lecture de la valeur de la caractéristique
    value = pRemoteCharacteristic->readValue();
    Serial.print("Value received: ");
    for (int i = 0; i < value.length(); i++) {
        Serial.print(value[i],DEC);
        Serial.print(" ");
    }
    Serial.println();
  }

  Serial.println("Send characteristic POWER6 value...");
  //pRemoteCharacteristic->writeValue(power6, sizeof(cmdoff));
  // Déconnexion du périphérique BLE
  Serial.println("Disconnecting from the BLE server...");
  pClient->disconnect();
}

void loop() {
  // Ne rien faire ici
}
