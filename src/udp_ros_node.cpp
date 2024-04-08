#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ros/ros.h>
#include <std_msgs/Header.h>
#include <nav_msgs/Odometry.h>

void sig_handler(int sig) {}

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    double buf[16] = { 0 };
    struct sigaction act;

    printf("hello %s\n", argv[1]);

    memset(&act, 0, sizeof(act));
    act.sa_handler = &sig_handler;
    sigaction(SIGINT, &act, NULL);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8800);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    /*struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);*/

    sendto(sockfd, buf, 8, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr)); //hello

    ros::init(argc, argv, "udp_ros", ros::init_options::NoSigintHandler);
    ros::NodeHandle n;
    ros::Publisher odo_pub = n.advertise<nav_msgs::Odometry>("odometry", 1);

    while (true) {
        int len = recv(sockfd, buf, sizeof(buf), 0);
        if (len > 0) {
            std_msgs::Header header;
            header.frame_id = "map";
            header.stamp = ros::Time::now();
            nav_msgs::Odometry odometry;
            odometry.header = header;
            odometry.child_frame_id = "map";
            odometry.pose.pose.position.x = buf[1];
            odometry.pose.pose.position.y = buf[2];
            odometry.pose.pose.position.z = buf[3];
            odometry.pose.pose.orientation.x = buf[4];
            odometry.pose.pose.orientation.y = buf[5];
            odometry.pose.pose.orientation.z = buf[6];
            odometry.pose.pose.orientation.w = buf[7];
            odometry.twist.twist.linear.x = buf[8];
            odometry.twist.twist.linear.y = buf[9];
            odometry.twist.twist.linear.z = buf[11];
            odo_pub.publish(odometry);
        } else break;
    }

    close(sockfd);
    printf("bye\n");

    return 0;
}
