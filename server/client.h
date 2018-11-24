#ifndef CLIENT_H
#define CLIENT_H

#include <string.h>
#include "bid.h"

class Client
{
  Bid bid;
  int ID;
  int fd;
  char *name;

public:
  Client(const char *name, int fd);
  ~Client();
  bool SetBid(unsigned int face, unsigned int quantity);
  Bid GetBid();
};

#endif
