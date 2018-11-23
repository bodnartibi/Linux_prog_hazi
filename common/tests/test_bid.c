#include <assert.h>
#include "bid.h"

#define NDEBUG

void test_set_bad_bid()
{
  Bid bid;
  unsigned int f, q, fn, qn;

  f = 0;
  q = 5;
  assert(!bid.SetBid(f, q));

  f = 2;
  q = 0;
  assert(!bid.SetBid(f, q));

  f = 8;
  q = 5;
  assert(!bid.SetBid(f, q));

  f = 0;
  q = 5;
  assert(!bid.SetBid(f, q));
}

void test_set_right_bid()
{
  Bid bid;
  unsigned int f, q, fn, qn;

  f = 2;
  q = 5;
  assert(bid.SetBid(f, q));
  bid.GetBid(fn, qn);
  assert(fn == f);
  assert(qn == q);
}

int
main (int argc, char *argv[])
{
  test_set_right_bid();
  test_set_bad_bid();
}
