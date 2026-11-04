#include "traceroute.h"

// ****************************************************************************
// * Compute the Internet Checksum over an arbitrary buffer.
// * (written with the help of ChatGPT 3.5)
// ****************************************************************************
uint16_t checksum(unsigned short *buffer, int size) {
    unsigned long sum = 0;
    while (size > 1) {
        sum += *buffer++;
        size -= 2;
    }
    if (size == 1) {
        sum += *(unsigned char *) buffer;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short) (~sum);
}
void fill_in_IP_Header(char *packet, int ttl) {
  struct iphdr *ip = (struct iphdr *)packet;
  ip-> version = 4;
  ip->ihl = 5;
  ip->tos = 0;
  ip->tot_len = htons(BUF_SIZE);
  ip->frag_off = 0;
  ip->ttl = ttl;
  ip->protocol = IPPROTO_ICMP;
  ip->check = 0;
}
void fill_in_ICMP_Header(char *packet) {
  struct icmphdr *icmp = (struct icmphdr *)packet;
  icmp->type = ICMP_ECHO;
  icmp->code = 0;
  icmp->un.echo.id = htons(getpid());
  icmp->un.echo.sequence = htons(1);
  icmp->checksum = 0;
  icmp->checksum = checksum((unsigned short *)icmp, BUF_SIZE - sizeof(struct iphdr));
}

int main (int argc, char *argv[]) {
  std::string destIP;

  // ********************************************************************
  // * Process the command line arguments
  // ********************************************************************
  int opt = 0;
  while ((opt = getopt(argc,argv,"t:d:")) != -1) {

    switch (opt) {
    case 't':
      destIP = optarg;
      break;
    case 'd':
      LOG_LEVEL = atoi(optarg);;
      break;
    case ':':
    case '?':
    default:
      std::cout << "useage: " << argv[0] << " -t [target ip] -d [Debug Level]" << std::endl;
      exit(-1);
    }
  }

}
