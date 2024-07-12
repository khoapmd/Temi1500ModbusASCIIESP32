#include <HardwareSerial.h>
#include "uart.h"
#include <driver/uart.h>
#include "hex.h"

#define MAX_DATA_LENGTH 51  // Adjust based on your expected maximum message length

void convertTem(const uint8_t *hex_array, size_t len) {
    char ascii_str[len + 1]; // +1 for the null-terminator

    hex_array_to_ascii(hex_array, len, ascii_str);

    printf("ASCII String: %s\n", ascii_str);

    // Extract the temperature values as hexadecimal strings
    char tempPV_hex[5] = {ascii_str[7], ascii_str[8], ascii_str[9], ascii_str[10], '\0'};
    char tempSP_hex[5] = {ascii_str[11], ascii_str[12], ascii_str[13], ascii_str[14], '\0'};
    char wetPV_hex[5] = {ascii_str[15], ascii_str[16], ascii_str[17], ascii_str[18], '\0'};
    char wetSP_hex[5] = {ascii_str[19], ascii_str[20], ascii_str[21], ascii_str[22], '\0'};
    char humiPV_hex[5] = {ascii_str[23], ascii_str[24], ascii_str[25], ascii_str[26], '\0'};
    char humiSP_hex[5] = {ascii_str[27], ascii_str[28], ascii_str[29], ascii_str[30], '\0'};
    char nowSTS_hex[5] = {ascii_str[43], ascii_str[44], ascii_str[45], ascii_str[46], '\0'};
    
    // Convert the hexadecimal strings to integer values
    uint16_t tempPV = hex_string_to_int(tempPV_hex);
    uint16_t tempSP = hex_string_to_int(tempSP_hex);
    uint16_t wetPV = hex_string_to_int(wetPV_hex);
    uint16_t wetSP = hex_string_to_int(wetSP_hex);
    uint16_t humiPV = hex_string_to_int(humiPV_hex);
    uint16_t humiSP = hex_string_to_int(humiSP_hex);
    uint16_t nowSTS = hex_string_to_int(nowSTS_hex);
    
    printf("TEMP.PV: 0x%04X (%0.1f°C)\n", tempPV, tempPV / 100.0);
    printf("TEMP.SP: 0x%04X (%0.1f°C)\n", tempSP, tempSP / 100.0);
    printf("WET.PV: 0x%04X (%0.1f%%)\n", wetPV, wetPV / 100.0);
    printf("WET.SP: 0x%04X (%0.1f%%)\n", wetSP, wetSP / 100.0);
    printf("HUMI.PV: 0x%04X (%0.1f%%)\n", humiPV , humiPV / 10.0);
    printf("HUMI.SP: 0x%04X (%0.1f%%)\n", humiSP , humiSP / 10.0);
    printf("nowSTS: 0x%04X \n", nowSTS);
}

const char* command = ":01030000000AF2\r\n";

void setup() {
    Serial.begin(115200); // Initialize Serial Monitor for debugging
    
    uart_setup(UART_NUM_2, 115200, UART_DATA_7_BITS);
    Serial.println("UART2 configured for 7 data bits.");
}

void loop() {
    uint8_t data[MAX_DATA_LENGTH];
    int len = 0;

    uart_write_bytes(UART_NUM_2, command, strlen(command));
    
    // Read Data via UART
    while (len < MAX_DATA_LENGTH) {
        int bytes_read = uart_read_bytes(UART_NUM_2, data + len, MAX_DATA_LENGTH - len, 100 / portTICK_PERIOD_MS);
        if (bytes_read <= 0) {
            break;  // No more data or timeout
        }
        len += bytes_read;
        if (data[len - 1] == '\n') {
            break;  // End of message
        }
    }

    if (len > 0) {
        Serial.printf("Received %u bytes: [ ", len);
        for (int i = 0; i < len; i++) {
            Serial.printf("0x%.2X ", data[i]);
        }
        Serial.println("]");
        
        convertTem(data, len);
    } else {
        Serial.println("No data received.");
        uart_wait_tx_done(UART_NUM_2, 10);
    }

    delay(5000);
}