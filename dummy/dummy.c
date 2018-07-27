#define _XOPEN_SOURCE 600
#define PAYLOAD_SIZE 40

#include <signal.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../lib/payloads.h"

int main(void)
{
    struct header head = {0, 8 * (PAYLOAD_SIZE + 1), 0};

    // for (unsigned int i = 0; i < PAYLOAD_SIZE; ++i)
    // {
    //     m[i].data = htonl(i);
    //     m[i].left = htons(1);
    //     m[i].right = htons(1);
    // }

    struct molecule m[PAYLOAD_SIZE] = 
        // {{htonl(437472611), htons(6), htons(0)},
        // {htonl(2595747122), htons(0), htons(0)},
        // {htonl(3), htons(2), htons(5)},
        // {htonl(2595747120), htons(2), htons(1)},
        // {htonl(0), htons(0), htons(0)},
        // {htonl(0), htons(2), htons(4)},
        // {htonl(0), htons(3), htons(0)}};

        // {{htonl(55), htons(0), htons(0)},
        // {htonl(66), htons(0), htons(0)},
        // {htonl(44), htons(2), htons(2)},
        // {htonl(22), htons(2), htons(2)},
        // {htonl(111), htons(6), htons(7)},
        // {htonl(99), htons(0), htons(0)},
        // {htonl(88), htons(0), htons(0)},
        // {htonl(11), htons(2), htons(0)},
        // {htonl(222), htons(10), htons(0)},
        // {htonl(122), htons(0), htons(9)}};

// {{htonl(11), htons(3), htons(6 )},
// {htonl(10), htons(0), htons(4 )},
// {htonl(22), htons(2), htons(0 )},
// {htonl(33), htons(0), htons(5 )},
// {htonl(44), htons(0), htons(0 )},
// {htonl(55), htons(0), htons(0 )}};

{{htonl(1985580036), htons(17), htons(17 )},
{htonl(738718781), htons(5), htons(5 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(671991450), htons(34), htons(0 )},
{htonl(336723200), htons(5), htons(34 )},
{htonl(161111533), htons(5), htons(5 )},
{htonl(334973172), htons(5), htons(5 )},
{htonl(2450666439), htons(5), htons(5 )},
{htonl(2361859286), htons(5), htons(5 )},
{htonl(113421102), htons(5), htons(5 )},
{htonl(1231498773), htons(38), htons(38 )},
{htonl(0), htons(0), htons(0 )},
{htonl(422290108), htons(5), htons(5 )},
{htonl(370679004), htons(5), htons(5 )},
{htonl(2680882418), htons(17), htons(17 )},
{htonl(3865608935), htons(5), htons(17 )},
{htonl(3429850084), htons(17), htons(17 )},
{htonl(0), htons(0), htons(0 )},
{htonl(2281763794), htons(5), htons(34 )},
{htonl(934973089), htons(5), htons(5 )},
{htonl(2585384777), htons(23), htons(23 )},
{htonl(1594878394), htons(0), htons(0 )},
{htonl(307595936), htons(5), htons(5 )},
{htonl(0), htons(0), htons(0 )},
{htonl(2725033504), htons(17), htons(17 )},
{htonl(3859654374), htons(17), htons(17 )},
{htonl(1429656725), htons(5), htons(34 )},
{htonl(2447841726), htons(17), htons(17 )},
{htonl(319098770), htons(34), htons(5 )},
{htonl(0), htons(0), htons(0 )},
{htonl(1078392465), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(758210230), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(1617373810), htons(34), htons(5 )},
{htonl(1093629072), htons(12), htons(12 )},
{htonl(1891970152), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )}};


// {{htonl(3756987325), htons(0), htons(59)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(3202909420), htons(1), htons(13)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(2625090483), htons(1), htons(13)},
// {htonl(2503417986), htons(13), htons(10)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(611813545), htons(1), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(423391149), htons(13), htons(13)},
// {htonl(576970152), htons(24), htons(24)},
// {htonl(0), htons(0), htons(0)},
// {htonl(1147423230), htons(0), htons(124)},
// {htonl(0), htons(0), htons(0)},
// {htonl(2510917866), htons(230), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(2764094781), htons(0), htons(44)},
// {htonl(1685383587), htons(24), htons(24)},
// {htonl(2547811903), htons(1), htons(13)},
// {htonl(0), htons(0), htons(0)},
// {htonl(462833302), htons(18), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(3547358022), htons(13), htons(13)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(830050595), htons(39), htons(20)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(79988758), htons(0), htons(20)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(3904169664), htons(0), htons(0)},
// {htonl(3888033015), htons(1), htons(13)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(113824265), htons(24), htons(44)}};



// {{htonl(1841657429), htons(29), htons(22 )},
// {htonl(4150814391), htons(24), htons(24 )},
// {htonl(3457461963), htons(29), htons(29 )},
// {htonl(1128605016), htons(11), htons(11 )},
// {htonl(3779224759), htons(0), htons(0 )},
// {htonl(2008197309), htons(32), htons(32 )},
// {htonl(3905080637), htons(4), htons(4 )},
// {htonl(1258805562), htons(29), htons(12 )},
// {htonl(1202025775), htons(31), htons(31 )},
// {htonl(815969833), htons(5), htons(5 )},
// {htonl(31096508), htons(6), htons(6 )},
// {htonl(4238687955), htons(22), htons(0 )},
// {htonl(1698434119), htons(6), htons(6 )},
// {htonl(1940824614), htons(9), htons(9 )},
// {htonl(3120665228), htons(22), htons(29 )},
// {htonl(2164865097), htons(6), htons(6 )},
// {htonl(1684308084), htons(6), htons(6 )},
// {htonl(3531439471), htons(29), htons(12 )},
// {htonl(549416363), htons(12), htons(29 )},
// {htonl(3166653959), htons(29), htons(29 )},
// {htonl(731078057), htons(26), htons(26 )},
// {htonl(3790645980), htons(0), htons(29 )},
// {htonl(2432988119), htons(0), htons(7 )},
// {htonl(1191912641), htons(10), htons(10 )},
// {htonl(2370929023), htons(2), htons(2 )},
// {htonl(805531982), htons(23), htons(7 )},
// {htonl(1304639890), htons(12), htons(29 )},
// {htonl(731254606), htons(9), htons(9 )},
// {htonl(1075556578), htons(26), htons(26 )},
// {htonl(3936187611), htons(2), htons(2 )},
// {htonl(3077221036), htons(19), htons(19 )},
// {htonl(3314616588), htons(2), htons(2 )},
// {htonl(871946255), htons(10), htons(10)}};

// {{htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(3607896921), htons(45), htons(45 )},
// {htonl(3644292189), htons(45), htons(45 )},
// {htonl(2181415323), htons(45), htons(45 )},
// {htonl(228638143), htons(45), htons(45 )},
// {htonl(4281788422), htons(45), htons(45 )},
// {htonl(3769560301), htons(45), htons(45 )},
// {htonl(2209795693), htons(45), htons(45 )},
// {htonl(1629951951), htons(45), htons(45 )},
// {htonl(1204307612), htons(45), htons(45 )},
// {htonl(1010001362), htons(0), htons(29 )},
// {htonl(270901713), htons(45), htons(45)}, 
// {htonl(1515448757), htons(45), htons(45 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(3774620130), htons(0), htons(45 )},
// {htonl(1864480730), htons(45), htons(45 )},
// {htonl(2868570246), htons(45), htons(45 )},
// {htonl(529170540), htons(16), htons(45 )},
// {htonl(1594977975), htons(45), htons(45 )},
// {htonl(1012303754), htons(45), htons(45 )},
// {htonl(3916262607), htons(16), htons(45 )},
// {htonl(3538323440), htons(45), htons(45 )},
// {htonl(1007439556), htons(45), htons(45 )},
// {htonl(1874873797), htons(45), htons(45 )},
// {htonl(2151589297), htons(45), htons(45 )},
// {htonl(3567543010), htons(12), htons(12 )},
// {htonl(1287428548), htons(45), htons(45 )},
// {htonl(3720930043), htons(0), htons(0 )},
// {htonl(2946850002), htons(45), htons(45 )},
// {htonl(2612296319), htons(16), htons(45 )},
// {htonl(1149607389), htons(45), htons(45 )},
// {htonl(3006124279), htons(45), htons(45 )},
// {htonl(1661585327), htons(45), htons(45 )},
// {htonl(887311987), htons(45), htons(45 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(3479435028), htons(45), htons(45 )},
// {htonl(3621156190), htons(45), htons(45 )},
// {htonl(4177276795), htons(45), htons(45 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(2436252219), htons(45), htons(45 )},
// {htonl(319485928), htons(45), htons(45 )},
// {htonl(1722866900), htons(12), htons(29 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(1249019044), htons(45), htons(45 )},
// {htonl(4010587658), htons(45), htons(45 )},
// {htonl(2539916008), htons(45), htons(45 )}};


// {htonl(2366264555), htons(2800944625), htons(2800944625 
// {htonl(2640528533), htons(0), htons(0 
// {htonl(0), htons(0), htons(0 
// {htonl(2592975124), htons(4286585090), htons(2640528533 
// {htonl(2714421386), htons(4286585090), htons(2714421386 
// {htonl(831397483), htons(2800944625), htons(2800944625 
// {htonl(2800944625), htons(4286585090), htons(2640528533 
// {htonl(2811353713), htons(2800944625), htons(2800944625 
// {htonl(4286585090), htons(2640528533), htons(0 
// {htonl(3329688709), htons(2640528533), htons(4286585090 
// {htonl(2339814773), htons(4286585090), htons(4286585090 
// {htonl(4213765131), htons(4286585090), htons(4286585090 
// {htonl(0), htons(0), htons(0 
// {htonl(1562233029), htons(4286585090), htons(2640528533 
// {htonl(570436822), htons(2800944625), htons(2800944625 
// {htonl(1568386887), htons(4286585090), htons(4286585090 

       // {{htonl(0), htons(0), htons(0)},
       //  {htonl(0), htons(0), htons(1)},
       //  {htonl(0), htons(2), htons(0)},
       //  {htonl(2595747120), htons(1), htons(2)}};

       // {{htonl(11), htons(0), htons(0)},
       //  {htonl(22), htons(0), htons(1)},
       //  {htonl(33), htons(0), htons(1)},
       //  {htonl(44), htons(0), htons(0)},
       //  {htonl(55), htons(0), htons(0)},
       //  {htonl(2595747120), htons(0), htons(0)}};

       // {
        // {htonl(33), htons(0), htons(0)},
        // {htonl(2595747120), htons(0), htons(0)},
        // {htonl(55), htons(2), htons(0)},
        // {htonl(30), htons(0), htons(2)},
        // {htonl(44), htons(0), htons(0)},
        // {htonl(22), htons(0), htons(0)}};

// {{htonl(2385213943), htons(8), htons(8)},
// {htonl(3418842447), htons(8), htons(8)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(3908143215), htons(8), htons(8)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(3874326378), htons(17), htons(17)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(318917951), htons(0), htons(0)},
// {htonl(4197521027), htons(11), htons(11)},
// {htonl(0), htons(0), htons(0)},
// {htonl(403636815), htons(8), htons(8)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(2168191014), htons(11), htons(11)},
// {htonl(0), htons(0), htons(0)},
// {htonl(863568468), htons(11), htons(11)},
// {htonl(3308618954), htons(8), htons(8)},
// {htonl(0), htons(0), htons(0)},
// {htonl(4210505040), htons(17), htons(17)},
// {htonl(0), htons(0), htons(0)},
// {htonl(775430040), htons(11), htons(11)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(1302008166), htons(17), htons(17)},
// {htonl(3302781086), htons(17), htons(17)},
// {htonl(2168068097), htons(8), htons(8)},
// {htonl(184694182), htons(11), htons(11)},
// {htonl(3498811718), htons(8), htons(8)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(172063791), htons(8), htons(8)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)},
// {htonl(0), htons(0), htons(0)}};

// {{htonl(158826186), htons(0), htons(0 )},
// {htonl(2102341165), htons(7), htons(7 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(2323919844), htons(51), htons(51 )},
// {htonl(197313560), htons(43), htons(43 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(3163465641), htons(9), htons(9 )},
// {htonl(2299404264), htons(7), htons(7 )},
// {htonl(1850636209), htons(43), htons(43 )},
// {htonl(1665684734), htons(1), htons(1 )},
// {htonl(2500323947), htons(7), htons(7 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(3980119256), htons(9), htons(9 )},
// {htonl(2765015452), htons(26), htons(26 )},
// {htonl(1742567042), htons(7), htons(7 )},
// {htonl(2611217597), htons(7), htons(7 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(3756528821), htons(49), htons(49 )},
// {htonl(899834659), htons(49), htons(49 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(4237505087), htons(43), htons(43 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(664177049), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(2754869304), htons(49), htons(49 )},
// {htonl(1344601859), htons(7), htons(7 )},
// {htonl(843043261), htons(49), htons(49 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(3866189308), htons(7), htons(7 )},
// {htonl(2545390462), htons(1), htons(1 )},
// {htonl(1086671540), htons(4), htons(4 )},
// {htonl(447191802), htons(7), htons(7 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(953425750), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(2386866943), htons(43), htons(43 )},
// {htonl(3844059643), htons(49), htons(49 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(2030246636), htons(26), htons(26 )},
// {htonl(0), htons(0), htons(0 )},
// {htonl(3797359778), htons(7), htons(7 )},
// {htonl(0), htons(0), htons(0 )}};

    char port[5] = "3018";

    // Socket setup
    struct addrinfo hints = {0};
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Getting socket info
    struct addrinfo *results;
    int err = getaddrinfo(NULL , port, &hints, &results);
    if(err != 0) {
        fprintf(stderr, "Could not get address: %s\n", gai_strerror(err));
        return 1;
    }

    // Establishing the socket
    int sd = socket(results->ai_family, results->ai_socktype, 0);
    if(sd < 0) {
        perror("Could not establish socket");
        freeaddrinfo(results);
        return 1;
    }

    // Adding socket option for faster binding
    int yes = 1;
    err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if(err < 0) {
        perror("Unable to alter socket options");
        close(sd);
        return 1;
    }

    // Attempting to connect to dispatcher
    err = connect(sd, results->ai_addr, results->ai_addrlen);
    if(err < 0)
    {
        perror("Could not bind socket");
        close(sd);
        freeaddrinfo(results);
        return 1;
    }

    // Freeing the results
    freeaddrinfo(results);

    head.size = htons(head.size);
    send(sd, &head, 8, 0);
    send(sd, m, PAYLOAD_SIZE * 8, 0);

    for (int i = 0; i < PAYLOAD_SIZE; ++i)
    {
        printf("Data: %u\n", m[i].data);
        printf("Left: %u\n", m[i].left);
        printf("Right: %u\n\n", m[i].right);
    }

    close(sd);

    return 0;
}