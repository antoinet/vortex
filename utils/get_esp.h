#ifndef GET_ESP_H
#define GET_ESP_H

unsigned long get_esp(void) {
   __asm__("movl %esp,%eax");
}

#endif /* GET_ESP_H */
