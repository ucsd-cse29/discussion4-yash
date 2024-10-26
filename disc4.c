#include <stdlib.h>
#include <stdio.h>

int32_t width_from_start_byte(char start_byte) {

    // given the start byte, how many bytes make up the codepoint


    // when the byte starts with a 0
    if ((start_byte & 0b10000000) == 0b00000000) { // start_byte & 0x80 == 0x00
        return 1;
    }
    // 110xxxxx
    if ((start_byte & 0b11100000) == 0b11000000) { // start_byte & 0x80 == 0x00
        return 2;
    }
    // 1110xxxx
    if ((start_byte & 0b11110000) == 0b11100000) { // start_byte & 0x80 == 0x00
        return 3;
    }
    // 11110xxx
    if ((start_byte & 0b11111000) == 0b11110000) { // start_byte & 0x80 == 0x00
        return 4;
    }

    return -1;

}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi) {

    // gives you the index of the FIRST byte of the codepoint
    // at cpi

    int byte_index = 0;
    int current_cp = 0;


    for (; current_cp < cpi; current_cp++) {
        byte_index += width_from_start_byte(str[byte_index]);
    }
    return byte_index;

}

int32_t codepoint_at(char str[], int32_t cpi) {

    // convert codepoint to byte index, interpret what the value is at that byte index
    // to determine that UTF-8 character
    int byte_index = codepoint_index_to_byte_index(str, cpi);
    char *codepoint = (str + byte_index); // points to the first byte of the codepoint

    int width = width_from_start_byte(codepoint[0]);

    if (width == 1) {
        // ASCII
        return codepoint[0];
    }

    if (width == 2) {
        // 110xxxxx 10xxxxxx
        return ((codepoint[0] & 0x1F) << 6) + (codepoint[1] & 0x3F);
    }
    if (width == 3) {
        // 1110xxxx 10xxxxxx 10xxxxxx
        return ((codepoint[0] & 0x0F) << 12) + ((codepoint[1] & 0x3F) << 6) + (codepoint[2] & 0x3F);
    }
    if (width == 4) {
        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        return ((codepoint[0] & 0x07) << 18) + ((codepoint[1] & 0x3F) << 12) + 
            ((codepoint[2] & 0x3F) << 6) + (codepoint[3] & 0x3F);
    }
    

}

char is_animal_emoji_at(char str[], int32_t cpi) {

    int codepoint = codepoint_at(str, cpi);

    // check within our range for animal emojis
    if (codepoint >= 0x1F400 && codepoint <= 0x1F43F) {
        return 1;
    } 
    // in the other range
    if (codepoint >= 0x1F980 && codepoint <= 0x1F9AE) {
        return 1;
    }

    return 0;

}

int main () {
    char salute[] = "🫡";
    char crab[] = "🦀";
    printf("Is %s an animal? %d\n", salute, is_animal_emoji_at(salute, 0));
    printf("Is %s an animal? %d\n", crab, is_animal_emoji_at(crab, 0));
    return 0;
}