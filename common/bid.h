#ifndef BID_H
#define BID_H

class Bid
{
  int face;
  int quantity;

public:
  Bid();
  bool SetBid(unsigned int face, unsigned int quantity);
  void GetBid(unsigned int &face, unsigned int &quantity);
};

#endif
