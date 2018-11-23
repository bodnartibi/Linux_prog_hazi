#ifndef CLIENT_MSG_H
#define CLIENT_MSG_H

#include "msg.h"

enum ClientMsgType
{
  REG_CLIENT,
  CLIENT_READY,
  CHALLENGE,
  NEW_BID,
};

class ClientMsg : Msg
{
  enum ClientMsgType;
}

class RegClientMsg : ClientMsg
{
  char name[MAX_NAME_LENGTH];
};

class RegClientMsg : ClientMsg
{
  bool ready;
};

class GameClientMsg : ClientMsg
{
  int challenge;
  int bid_face;
  int bid_quantity;
};
#endif
