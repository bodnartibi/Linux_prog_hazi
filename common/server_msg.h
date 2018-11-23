#define SERVER_MSG_H
#define SERVER_MSG_H

#include "msg.h"

enum ServerMsgType
{
  YOUR_ID,
  NEW_DICE_ROLL,
  PROP_BID,
  WHO_WON,
  DISCONNECT,
  INFO,
  MSG_TYPE_MAX
};

class ServerMsg : Msg
{
  enum ServerMsgType;
};

class NewRollServerMsg : ServerMsg
{
  int dices[MAX_DICE_NUM];
};

class BidServerMsg : ServerMsg
{
  int bid_face;
  int bid_quantity;
  bool your_turn;
};

class WinnerServerMsg : ServerMsg
{
  bool is_it_you;
  char name[MAX_NAME_LENGTH];
};

struct your_ID
class IDServerMsg : ServerMsg
{
  int client_ID;
};

class InfoServerMsg : ServerMsg
{
  char msg[INFO_SIZE];
};

class DisconnectServerMsg : ServerMsg
{
  char name[MAX_NAME_LENGTH];
};
#endif
