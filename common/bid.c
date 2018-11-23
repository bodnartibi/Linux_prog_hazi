#include "bid.h"

Bid::Bid()
{
  face = 0;
  quantity = 0;
}

bool
Bid::SetBid(unsigned int face, unsigned int quantity)
{
  if (face < 1 || face > 6 \
      || quantity < 0 \
      || face < this->face || quantity < this->quantity)
    return false;

  if (face == this->face && quantity == this->quantity)
    return false;

  this->face = face;
  this->quantity = quantity;
  return true;
}

void
Bid::GetBid(unsigned int &face, unsigned int &quantity)
{
  face = this->face;
  quantity = this->quantity;
}
