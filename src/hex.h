#include <stdint.h>
#include <string.h>

void hex_array_to_ascii(const uint8_t *hex_array, size_t len, char *ascii_str);
uint8_t hex_char_to_int(char c);
uint16_t hex_string_to_int(const char* hex_str);