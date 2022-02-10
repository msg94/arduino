/*
  This example creates a BLE peripheral with a service that contains String
  characteristics that can be read and written.
  The length of the String characteristic needs to be fixed.

  The circuit:
  - Arduino Nano 33 BLE Sense board.

  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>


//----------------------------------------------------------------------------------------------------------------------
// BLE UUIDs
//----------------------------------------------------------------------------------------------------------------------

#define BLE_UUID_TEST_SERVICE               "9A48ECBA-2E92-082F-C079-9E75AAE428B1"
#define BLE_UUID_FILE_NAME                  "b337339b-78f7-4ac0-b903-8943d83fa378"

//----------------------------------------------------------------------------------------------------------------------
// BLE
//----------------------------------------------------------------------------------------------------------------------

BLEService testService( BLE_UUID_TEST_SERVICE );
BLEStringCharacteristic fileNameCharacteristic( BLE_UUID_FILE_NAME, BLERead | BLEWrite|BLENotify, 20 );
long previousMillis = 0;
String fileName = "";
String message = "";


void setup()
{
  Serial.begin( 115200 );
  //while ( !Serial );

  BLE.begin();
 
  // set advertised local name and service UUID:
  BLE.setDeviceName( "msg" );
  BLE.setLocalName( "msg" );
  BLE.setAdvertisedService( testService );

  // BLE add characteristics
  testService.addCharacteristic( fileNameCharacteristic );

  // add service
  BLE.addService( testService );

  // set the initial value for the characeristic:
  fileNameCharacteristic.writeValue( fileName );
  BLE.setEventHandler(BLEConnected, BLE_connect_event);
  BLE.setEventHandler(BLEDisconnected, BLE_disconnect_event);
  fileNameCharacteristic.setEventHandler(BLEWritten,BLE_RX_event);
  // start advertising
  BLE.advertise();

}

void loop()
{
  BLE.poll();
  // listen for BLE peripherals to connect:

}
void BLE_connect_event(BLEDevice central){
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}
void BLE_disconnect_event(BLEDevice central){
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}
void BLE_RX_event(BLEDevice central, BLECharacteristic characteristic){
  Serial.print("Characteristic event, written: ");
      if ( fileNameCharacteristic.written() )
      {
        fileName = fileNameCharacteristic.value();
        Serial.print( fileName+"\r\n" );
        fileNameCharacteristic.writeValue("echo: " + fileName + "\r\n");
      }
      
}

  /*BLEDevice central = BLE.central();

  if ( central )
  {
    Serial.print( "Connected to central: " );
    Serial.println( central.address() );

    while ( central.connected() )
    {
      long currentMillis = millis();
      if ( fileNameCharacteristic.written() )
      {
        fileName = fileNameCharacteristic.value();
        Serial.print( fileName+"\r\n" );
      }
      if (currentMillis - previousMillis >= 500) {
        previousMillis = currentMillis;
        fileNameCharacteristic.writeValue(fileName);
      }

    }

    Serial.print( F( "Disconnected from central: " ) );
    Serial.println( central.address() );
  }*/
