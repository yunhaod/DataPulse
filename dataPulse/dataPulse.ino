#include <ArduinoBLE.h>
#include <DHT.h>

#define DHTPIN 4    
#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE);

//service and characteristic
BLEService sensorService("d888a9c2-f3cc-11ed-a05b-0242ac120003");
BLEByteCharacteristic temperatureCharacteristic("d888a9c3-f3cc-11ed-a05b-0242ac120003", BLERead | BLENotify);
BLEByteCharacteristic humidityCharacteristic("d888a9c4-f3cc-11ed-a05b-0242ac120003", BLERead | BLENotify);

int temperature = 0;
int humidity = 0;
unsigned long startMillis = 0;
unsigned long currentMillis = 0;

void setup() {
  Serial.begin(115200);

  // LED pin initialization
  pinMode(LED_BUILTIN, OUTPUT);

  // BLE initialization
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE module!");
    while (1);
  }
  
  dht.begin();

  // set advertised local name and service UUID
  BLE.setLocalName("iOSArduinoBoard");

  // add the characteristics to the services
  sensorService.addCharacteristic(temperatureCharacteristic);
  sensorService.addCharacteristic(humidityCharacteristic);

  // add services to BLE stack
  BLE.addService(sensorService);

  // set read request handler for temperature characteristic
  temperatureCharacteristic.setEventHandler(BLERead, temperatureCharacteristicRead);
  humidityCharacteristic.setEventHandler(BLERead, humidityCharacteristicRead);

  // start advertising
  BLE.advertise();

  startMillis = millis();
  Serial.println("Start advertising");
}

void loop() {  
  // listen for BLE centrals to connect
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      currentMillis = millis();
      if (currentMillis - startMillis >= 1000) {
        // read temperature value every 1 second
        temperature = dht.readTemperature();
        humidity = dht.readHumidity();
        // update temperature value in temperature characteristic
        Serial.println(temperature);
        Serial.println(humidity);
        temperatureCharacteristic.writeValue(temperature);
        humidityCharacteristic.writeValue(humidity);
        startMillis = currentMillis;  
      }
    }
  }
}

void temperatureCharacteristicRead(BLEDevice central, BLECharacteristic characteristic){
  temperatureCharacteristic.writeValue(temperature);
}

void humidityCharacteristicRead(BLEDevice central, BLECharacteristic characteristic){
  humidityCharacteristic.writeValue(humidity);
}