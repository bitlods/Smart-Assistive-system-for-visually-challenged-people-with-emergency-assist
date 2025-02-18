#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define GPS_UART 0
#define GSM_UART 1

void UART_Init(int uart);
void UART_Send(int uart, char data);
char UART_Receive(int uart);
void GPS_Parse(char* gps_data);
void GSM_Send_SMS(const char* phone_number, const char* message);
void GPS_ReadData(void);

char latitude[15], longitude[15];
float lat, lon;

void UART_Init(int uart) {
}

void UART_Send(int uart, char data) {
}

char UART_Receive(int uart) {
    return 0;
}

void GPS_Parse(char* gps_data) {
    char* token;

    if (strstr(gps_data, "$GPRMC") != NULL) {
        token = strtok(gps_data, ",");
        for (int i = 0; i < 2; i++) {
            token = strtok(NULL, ",");
        }
        strcpy(latitude, strtok(NULL, ","));
        strcpy(longitude, strtok(NULL, ","));
        lat = atof(latitude);
        lon = atof(longitude);
        printf("Latitude: %s, Longitude: %s\n", latitude, longitude);
    }
}

void GSM_Send_SMS(const char* phone_number, const char* message) {
    UART_Send(GSM_UART, 'A');
    UART_Send(GSM_UART, 'T');
    UART_Send(GSM_UART, 'C');
    UART_Send(GSM_UART, 'M');
    UART_Send(GSM_UART, 'S');
}

void GPS_ReadData(void) {
    char gps_buffer[256];
    int index = 0;

    while (1) {
        char c = UART_Receive(GPS_UART);
        if (c == '$') {
            gps_buffer[index++] = c;
        } else if (c == '\n') {
            gps_buffer[index] = '\0';
            GPS_Parse(gps_buffer);
            index = 0;
        } else {
            gps_buffer[index++] = c;
        }

        if (lat != 0.0 && lon != 0.0) {
            char message[160];
            snprintf(message, sizeof(message), 
                     "I am in a critical situation, please help me. Location: https://www.google.com/maps?q=%.6f,%.6f", lat, lon);
            GSM_Send_SMS("+919353717456", message);
            break;
        }
    }
}

int main() {
    UART_Init(GPS_UART);
    UART_Init(GSM_UART);
    
    while (1) {
        GPS_ReadData();
    }
    
    return 0;
}
