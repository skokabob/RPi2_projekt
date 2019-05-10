#include <string.h> //memset()
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "communication.h"
#include "logging.h"
//#include "joystick.h"
//#include "RPi2_I2C.h"
#include "km2.h"
//#include "PID.h"

//struct Joystick_data data;

/**
 * Kód pro komunikaci s Joystickem ve vláknu
 * @param dummy
 * @return 
 */
/*PI_THREAD(recv_server) {
    int sockfd;
    int endpoint_cli_size;
    struct sockaddr_in endpoint_srv, endpoint_cli;
    memset(&endpoint_cli, 0, sizeof (struct sockaddr_in));
    endpoint_srv.sin_family = AF_INET;
    endpoint_srv.sin_addr.s_addr = htonl(INADDR_ANY);
    endpoint_srv.sin_port = htons(8888);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (struct sockaddr*) &endpoint_srv, sizeof (endpoint_srv));

    int r;
    char recv_buffer[150];
    while (1) {
        r = recvfrom(sockfd, recv_buffer, 149, 0, (struct sockaddr*) &endpoint_cli, &endpoint_cli_size);
        recv_buffer[r] = 0;
        //log_message(LOG_INFO, "%s", recv_buffer);
        int i;
        for (i = 0; i < 40; ++i)
            printf("%c", recv_buffer[i]);

        // načtení a vytisk datových hodnot joysticku
        sscanf(recv_buffer, "$CTCTR,%*d,%f,%f,%*s", &data.y_joystick, &data.x_joystick);
        printf("x: %.4f; y: %.4f\n", data.x_joystick, data.y_joystick);

        // ovládání joysticku
        if (((data.x_joystick > 0) || (data.x_joystick < 0)) && (data.y_joystick == 0)) {
            i2cDriverMotor(0, data.x_joystick * speed_left);
        }
        if (((data.y_joystick > 0) || (data.y_joystick < 0)) && (data.x_joystick == 0)) {
            i2cDriverMotor(data.y_joystick*speed_right, 0);
        }
        if (((data.y_joystick > 0) || (data.y_joystick < 0)) && ((data.x_joystick > 0) || (data.x_joystick < 0))) {
            i2cDriverMotor(data.y_joystick*speed_right, data.x_joystick * speed_left);
        }
        if ((data.y_joystick == 0) && (data.x_joystick == 0)) {
            i2cDriverMotor(0, 0);
        }
    }
}

/**
 * Pro komunikaci s Joystickem
 */
///void start_recv_server() {
//   piThreadCreate(recv_server);
//}

/**
 * Kód pro posílání pozice robotu ve vláknu
 * @param dummy
 * @return 
 */
PI_THREAD(send_to_server) {
    int sockfd;

    struct sockaddr_in endpoint_srv;
    memset(&endpoint_srv, 0, sizeof (struct sockaddr_in));
    endpoint_srv.sin_family = AF_INET;

    int ret = inet_aton(IP_ADDR, &endpoint_srv.sin_addr);
    if (ret != 1) error(EXIT_FAILURE, errno, "inet_aton");

    endpoint_srv.sin_port = htons(8889);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        error(EXIT_FAILURE, errno, "socket");
    }

    float x = 5.2, y = 3.1, a = 12.0;
    int s;
    char send_buffer[16]; // posilame pozici robotu (joysticku???) float x, y, azimut
    //azimut je roztec kol/2

    while (1) {
        memcpy(send_buffer, &x, 4);
        memcpy(send_buffer + 4, &y, 4);
        memcpy(send_buffer + 8, &a, 4);
        s = sendto(sockfd, send_buffer, 12, 0, (struct sockaddr*) &endpoint_srv, sizeof (struct sockaddr_in));
        if (s < 0) {
            error(EXIT_FAILURE, errno, "send_to");
        }
        usleep(100000);

        x++;
        y++;
        a++;
    }
}

/**
 * Posíláme pozici robotu
 */
void start_send_to_server() {
    int x;
    x = piThreadCreate(send_to_server);
    if (x != 0) {
        error(EXIT_FAILURE, errno, "thread send_to_serv");
    }

}

