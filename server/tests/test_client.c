#include <assert.h>
#include "client.h"

#define NDEBUG

void _set_should_fail(Client *c, unsigned int f, unsigned int q)
{
  assert(!c->SetBid(f, q));
}

void _set_should_pass(Client *c, unsigned int f, unsigned int q)
{
  unsigned int fn, qn;
  Bid bid;

  assert(c->SetBid(f, q));
  bid = c->GetBid();
  bid.GetBid(fn, qn);
  assert(fn == f);
  assert(qn == q);
}

void test_set_bad_bid()
{
  Client c("Elso", 5);
  _set_should_fail(&c, 0, 5);
  _set_should_fail(&c, 2, 0);
  _set_should_fail(&c, 8, 5);
  _set_should_fail(&c, 0, 5);
}

void test_set_right_bid()
{
  Client c("Masodik", 5);
  _set_should_pass(&c, 2, 5);
}

int
main (int argc, char *argv[])
{
  test_set_right_bid();
  test_set_bad_bid();
}
