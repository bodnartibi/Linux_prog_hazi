#include <assert.h>
#include "bid.h"

#define NDEBUG

void test_set_right_bid()
{
  Bid bid;
  unsigned int f, q, fn, qn;

  f = 2;
  q = 5;
  bid.SetBid(f, q);
  bid.GetBid(fn, qn);
  assert(fn == f);
  assert(qn == q);
}

int
main (int argc, char *argv[])
{
  test_set_right_bid();
}
