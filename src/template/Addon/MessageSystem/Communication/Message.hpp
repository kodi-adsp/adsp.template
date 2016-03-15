#pragma once
/*
 *      Copyright (C) 2005-2016 Team Kodi
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */



#include "threads/Thread.h"

#define MSG_INTERNAL_BUFFER_SIZE 32

class Message
{
  friend class IProtocol;

public:
  Message() 
  {
    isSync = false;
    data = NULL;
    event = NULL;
    replyMessage = NULL;
  };

  int       signal;
  bool      isSync;
  bool      isSyncFini;
  bool      isOut;
  bool      isSyncTimeout;
  int       payloadSize;
  uint8_t   buffer[MSG_INTERNAL_BUFFER_SIZE];
  uint8_t   *data;
  Message   *replyMessage;
  IProtocol *origin;
  CEvent    *event;

  void Release();
  bool Reply(int sig, void *data = NULL, int size = 0);
};