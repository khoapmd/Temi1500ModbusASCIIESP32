#include <hex.h>

// Function to convert an array of hex values to an ASCII string
void hex_array_to_ascii(const uint8_t *hex_array, size_t len, char *ascii_str) {
    for (size_t i = 0; i < len; i++) {
        ascii_str[i] = (char)hex_array[i];
    }
    ascii_str[len] = '\0'; // Null-terminate the ASCII string
}

// Function to convert a hexadecimal character to its integer value
uint8_t hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else {
        return 0; // Error case, should not happen for valid input
    }
}

// Function to convert a two-character hexadecimal string to an integer value
uint16_t hex_string_to_int(const char* hex_str) {
    return (hex_char_to_int(hex_str[0]) << 12) |
           (hex_char_to_int(hex_str[1]) << 8)  |
           (hex_char_to_int(hex_str[2]) << 4)  |
           hex_char_to_int(hex_str[3]);
}