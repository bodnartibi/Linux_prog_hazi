#include <stdlib.h>
#include "client.h"
#include "bid.h"

Client::Client(const char *name, int fd): fd(fd)
{
  // TODO error
  this->name = (char*)malloc(strlen(name));
  strcpy(this->name, name);
}

Client::~Client()
{
  free(name);
}

bool
Client::SetBid(unsigned int face, unsigned int quantity)
{
  return this->bid.SetBid(face, quantity);
}

Bid
Client::GetBid()
{
  return this->bid;
}
