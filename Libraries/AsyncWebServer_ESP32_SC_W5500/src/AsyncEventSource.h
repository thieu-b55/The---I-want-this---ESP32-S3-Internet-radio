/****************************************************************************************************************************
  AsyncEventSource.h - Dead simple Ethernet AsyncWebServer.

  For W5500 LwIP Ethernet in ESP32_SC_W5500 (ESP32_S2/S3/C3 + W5500)

  AsyncWebServer_ESP32_SC_W5500 is a library for the LwIP Ethernet W5500 in ESP32_S2/S3/C3 to run AsyncWebServer

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500
  Licensed under GPLv3 license

  Original author: Hristo Gochkov

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.

  This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with this library;
  if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Version: 1.8.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.6.3   K Hoang      15/12/2022 Initial porting for W5500 + ESP32_S3. Sync with AsyncWebServer_ESP32_W5500 v1.6.3
  1.7.0   K Hoang      19/12/2022 Add support to ESP32_S2_W5500 (ESP32_S2 + LwIP W5500)
  1.8.0   K Hoang      20/12/2022 Add support to ESP32_C3_W5500 (ESP32_C3 + LwIP W5500)
  1.8.1   K Hoang      23/12/2022 Remove unused variable to avoid compiler warning and error
 *****************************************************************************************************************************/

#ifndef ASYNCEVENTSOURCE_H_
#define ASYNCEVENTSOURCE_H_

#include <Arduino.h>

#include <AsyncTCP.h>
#define SSE_MAX_QUEUED_MESSAGES 32

#include "AsyncWebServer_ESP32_SC_W5500.h"

#include "AsyncWebSynchronization.h"

/////////////////////////////////////////////////

#define DEFAULT_MAX_SSE_CLIENTS 8

class AsyncEventSource;
class AsyncEventSourceResponse;
class AsyncEventSourceClient;

/////////////////////////////////////////////////

typedef std::function<void(AsyncEventSourceClient *client)> ArEventHandlerFunction;

/////////////////////////////////////////////////

class AsyncEventSourceMessage
{
  private:
    uint8_t * _data;
    size_t _len;
    size_t _sent;
    size_t _acked;

  public:
    AsyncEventSourceMessage(const char * data, size_t len);
    ~AsyncEventSourceMessage();
    size_t ack(size_t len, uint32_t time __attribute__((unused)));
    size_t send(AsyncClient *client);

    /////////////////////////////////////////////////

    inline bool finished()
    {
      return _acked == _len;
    }

    /////////////////////////////////////////////////

    inline bool sent()
    {
      return _sent == _len;
    }
};

/////////////////////////////////////////////////

class AsyncEventSourceClient
{
  private:
    AsyncClient *_client;
    AsyncEventSource *_server;
    uint32_t _lastId;
    LinkedList<AsyncEventSourceMessage *> _messageQueue;
    void _queueMessage(AsyncEventSourceMessage *dataMessage);
    void _runQueue();

  public:

    AsyncEventSourceClient(AsyncWebServerRequest *request, AsyncEventSource *server);
    ~AsyncEventSourceClient();

    /////////////////////////////////////////////////

    inline AsyncClient* client()
    {
      return _client;
    }

    /////////////////////////////////////////////////

    void close();
    void write(const char * message, size_t len);
    void send(const char *message, const char *event = NULL, uint32_t id = 0, uint32_t reconnect = 0);

    /////////////////////////////////////////////////

    inline bool connected() const
    {
      return (_client != NULL) && _client->connected();
    }

    /////////////////////////////////////////////////

    inline uint32_t lastId() const
    {
      return _lastId;
    }

    /////////////////////////////////////////////////

    inline size_t  packetsWaiting() const
    {
      return _messageQueue.length();
    }

    /////////////////////////////////////////////////

    //system callbacks (do not call)
    void _onAck(size_t len, uint32_t time);
    void _onPoll();
    void _onTimeout(uint32_t time);
    void _onDisconnect();
};

/////////////////////////////////////////////////

class AsyncEventSource: public AsyncWebHandler
{
  private:
    String _url;
    LinkedList<AsyncEventSourceClient *> _clients;
    ArEventHandlerFunction _connectcb;

  public:
    AsyncEventSource(const String& url);
    ~AsyncEventSource();

    /////////////////////////////////////////////////

    inline const char * url() const
    {
      return _url.c_str();
    }

    /////////////////////////////////////////////////

    void close();
    void onConnect(ArEventHandlerFunction cb);
    void send(const char *message, const char *event = NULL, uint32_t id = 0, uint32_t reconnect = 0);
    size_t count() const; //number clinets connected
    size_t  avgPacketsWaiting() const;

    //system callbacks (do not call)
    void _addClient(AsyncEventSourceClient * client);
    void _handleDisconnect(AsyncEventSourceClient * client);
    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;
};

/////////////////////////////////////////////////

class AsyncEventSourceResponse: public AsyncWebServerResponse
{
  private:
    String _content;
    AsyncEventSource *_server;

  public:
    AsyncEventSourceResponse(AsyncEventSource *server);
    void _respond(AsyncWebServerRequest *request);
    size_t _ack(AsyncWebServerRequest *request, size_t len, uint32_t time);

    /////////////////////////////////////////////////

    inline bool _sourceValid() const
    {
      return true;
    }
};

/////////////////////////////////////////////////

#endif /* ASYNCEVENTSOURCE_H_ */

