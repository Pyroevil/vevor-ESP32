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

byte cmdmanual [] = {0xaa, 0x55, 0x0c, 0x22, 0x02, 0x01, 0x00, 0x31};
byte cmdauto [] = {0xaa, 0x55, 0x0c, 0x22, 0x02, 0x02, 0x00, 0x32};

byte cmdoff [] = {0xaa, 0x55, 0x0c, 0x22, 0x03, 0x00, 0x00, 0x31};
byte cmdon [] = {0xaa, 0x55, 0x0c, 0x22, 0x03, 0x01, 0x00, 0x32};

byte cmdp1 [] = {0xaa, 0x55, 0x0c, 0x22, 0x04, 0x01, 0x00, 0x33};
byte cmdp2 [] = {0xaa, 0x55, 0x0c, 0x22, 0x04, 0x02, 0x00, 0x34};
byte cmdp3 [] = {0xaa, 0x55, 0x0c, 0x22, 0x04, 0x03, 0x00, 0x35};
byte cmdp4 [] = {0xaa, 0x55, 0x0c, 0x22, 0x04, 0x04, 0x00, 0x36};
byte cmdp5 [] = {0xaa, 0x55, 0x0c, 0x22, 0x04, 0x05, 0x00, 0x37};
byte cmdp6 [] = {0xaa, 0x55, 0x0c, 0x22, 0x04, 0x06, 0x00, 0x38};
byte cmdp7 [] = {0xaa, 0x55, 0x0c, 0x22, 0x04, 0x07, 0x00, 0x39};
byte cmdp8 [] = {0xaa, 0x55, 0x0c, 0x22, 0x04, 0x08, 0x00, 0x3a};
byte cmdp9 [] = {0xaa, 0x55, 0x0c, 0x22, 0x04, 0x09, 0x00, 0x3b};
byte cmdp10 [] = {0xaa, 0x55, 0x0c, 0x22, 0x04, 0x0a, 0x00, 0x3c};

//byte value [19];

// Fonction pour gérer les événements BLE
class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pClient) {
        Serial.println("Connected to BLE Server");
    }

    void onDisconnect(BLEClient* pClient) {
        Serial.println("Disconnected from BLE Server");
    }
};

// Fonction de rappel pour gérer les notifications de la caractéristique
void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.println("Notification received:");
    for (int i = 0; i < length; i++) {
        Serial.print(pData[i]);
        Serial.print(" ");
    }
    Serial.println();
    Serial.println("Mode: ");
    Serial.println(pData[8]);
    Serial.println("Mode set (level or temp): ");
    Serial.println(pData[9]);
    Serial.println("Level: ");
    Serial.println(pData[10]);
    Serial.println("Temperature Output: ");
    Serial.println((float)pData[13]);
    Serial.println("Temperature Intake: ");
    Serial.println((float)pData[15]);
    Serial.println("Temperature Core: ");
    Serial.println((float)pData[19]);
    Serial.println("Voltage: ");
    Serial.println((float)pData[11]/10.0);
    Serial.println("Altitude: ");
    Serial.println(pData[6]);
    Serial.println();
}

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
    
    // Activation des notifications pour la caractéristique
    pRemoteCharacteristic->registerForNotify(notifyCallback);

    Serial.println("Send characteristic CMD1 value...");
    pRemoteCharacteristic->writeValue(cmdauto, sizeof(cmdauto));

    // Attente de la réception des notifications
    delay(5000);

    // Déconnexion du périphérique BLE
    Serial.println("Disconnecting from the BLE server...");
    pClient->disconnect();
}

void loop() {
    // Ne rien faire ici
}

