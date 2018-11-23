#include <assert.h>
#include "bid.h"

#define NDEBUG

void _set_should_fail(Bid *bid, unsigned int f, unsigned int q)
{
  assert(!bid->SetBid(f, q));
}

void _set_should_pass(Bid *bid, unsigned int f, unsigned int q)
{
  unsigned int fn, qn;

  assert(bid->SetBid(f, q));
  bid->GetBid(fn, qn);
  assert(fn == f);
  assert(qn == q);
}

void test_set_bad_bid()
{
  Bid bid;
  _set_should_fail(&bid, 0, 5);
  _set_should_fail(&bid, 2, 0);
  _set_should_fail(&bid, 8, 5);
  _set_should_fail(&bid, 0, 5);
}

void test_set_right_bid()
{
  Bid bid;
  _set_should_pass(&bid, 2, 5);
}

void test_new_bid()
{
  Bid bid;

  _set_should_pass(&bid, 2, 5);
  _set_should_pass(&bid, 3, 5);

  _set_should_fail(&bid, 3, 5);
  _set_should_fail(&bid, 2, 6);
  _set_should_fail(&bid, 4, 3);
}

int
main (int argc, char *argv[])
{
  test_set_right_bid();
  test_set_bad_bid();
  test_new_bid();
}
