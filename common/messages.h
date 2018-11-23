#ifndef MESSAGES_H
#define MESSAGES_H

#define MAX_CLIENT_NUM     5
#define MAX_NAME_LENGTH   16
#define MAX_DICE_NUM       5

#define SOCKET_SIZE 1024
#define INFO_SIZE 256

enum SenderType
{
  SERVER,
  CLIENT
};

class Msg
{
  enum SenderType;
}
#endif

