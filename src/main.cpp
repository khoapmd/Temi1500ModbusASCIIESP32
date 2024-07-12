#include <HardwareSerial.h>
#include "uart.h"
#include <driver/uart.h>
#include "hex.h"
#include <esp_task_wdt.h>
#include "main.h"
#include <Ticker.h>
#include <WiFi.h>
#include "mqttHelper.h"
#include <PubSubClient.h>


char boardID[23];
Ticker tickerGetData;
// Ticker tickerFirmware(checkFirmware, 1800003, 0, MILLIS); // 30 minutes
const char *command = ":01030000000AF2\r\n"; // Get D0001 to D0010;


void setup()
{
    Serial.begin(115200); // Initialize Serial Monitor for debugging
    uart_setup(UART_NUM_2, 115200, UART_DATA_7_BITS);
    Serial.println("UART2 configured for 7 data bits.");
    tickerGetData.attach(5, getData);
    snprintf(boardID, 23, "%llX", ESP.getEfuseMac());
    setup_wifi();
    setup_mqtt();
}

void loop()
{
    mqttLoop();
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.print("Connected Network Signal Strength (RSSI): ");
  Serial.println(WiFi.RSSI()); /*Print WiFi signal strength*/
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Disconnected Event");
}

void startWatchDog()
{
    // WatchDog
    Serial.println("Start WatchDog");
    esp_task_wdt_init(WDT_TIMEOUT, true); // enable panic so ESP32 restarts
    esp_task_wdt_add(NULL);               // add current thread to WDT watch
}

void stopWatchDog()
{
    Serial.println("Stopping WatchDog");
    esp_task_wdt_delete(NULL);
    esp_task_wdt_deinit();
}

void getData()
{
    ChamberData chamberData;
    uint8_t data[MAX_DATA_LENGTH];
    int len = 0;

    uart_write_bytes(UART_NUM_2, command, strlen(command));

    // Read Data via UART
    while (len < MAX_DATA_LENGTH)
    {
        int bytes_read = uart_read_bytes(UART_NUM_2, data + len, MAX_DATA_LENGTH - len, 100 / portTICK_PERIOD_MS);
        if (bytes_read <= 0)
        {
            break; // No more data or timeout
        }
        len += bytes_read;
        if (data[len - 1] == '\n')
        {
            break; // End of message
        }
    }

    if (len > 0)
    {
        Serial.printf("Received %u bytes: [ ", len);
        for (int i = 0; i < len; i++)
        {
            Serial.printf("0x%.2X ", data[i]);
        }
        Serial.println("]");

        convertData(data, len, &chamberData);
    }
    else
    {
        Serial.println("No data received.");
        uart_wait_tx_done(UART_NUM_2, 10);
    }
}