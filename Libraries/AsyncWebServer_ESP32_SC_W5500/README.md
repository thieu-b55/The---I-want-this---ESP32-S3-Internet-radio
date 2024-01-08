# AsyncWebServer_ESP32_SC_W5500 Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncWebServer_ESP32_SC_W5500.svg?)](https://www.ardu-badge.com/AsyncWebServer_ESP32_SC_W5500)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/AsyncWebServer_ESP32_SC_W5500.svg)](https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/releases)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/AsyncWebServer_ESP32_SC_W5500.svg)](http://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/issues)


<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>


---
---

## Table of contents

* [Important Note](#Important-Note)
* [Why do we need this AsyncWebServer_ESP32_SC_W5500 library](#why-do-we-need-this-AsyncWebServer_ESP32_SC_W5500-library)
  * [Features](#features)
  * [Why Async is better](#why-async-is-better)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Important things to remember](#important-things-to-remember)
* [Principles of operation](#principles-of-operation)
  * [The Async Web server](#the-async-web-server)
  * [Request Life Cycle](#request-life-cycle)
  * [Rewrites and how do they work](#rewrites-and-how-do-they-work)
  * [Handlers and how do they work](#handlers-and-how-do-they-work)
  * [Responses and how do they work](#responses-and-how-do-they-work)
  * [Template processing](#template-processing)
* [Request Variables](#request-variables)
  * [Common Variables](#common-variables)
  * [Headers](#headers)
  * [GET, POST and FILE parameters](#get-post-and-file-parameters)
  * [JSON body handling with ArduinoJson](#json-body-handling-with-arduinojson)
* [Responses](#responses)
  * [Redirect to another URL](#redirect-to-another-url)
  * [Basic response with HTTP Code](#basic-response-with-http-code)
  * [Basic response with HTTP Code and extra headers](#basic-response-with-http-code-and-extra-headers)
  * [Basic response with string content](#basic-response-with-string-content)
  * [Basic response with string content and extra headers](#basic-response-with-string-content-and-extra-headers)
  * [Respond with content coming from a Stream](#respond-with-content-coming-from-a-stream)
  * [Respond with content coming from a Stream and extra headers](#respond-with-content-coming-from-a-stream-and-extra-headers)
  * [Respond with content coming from a Stream containing templates](#respond-with-content-coming-from-a-stream-containing-templates)
  * [Respond with content coming from a Stream containing templates and extra headers](#respond-with-content-coming-from-a-stream-containing-templates-and-extra-headers)
  * [Respond with content using a callback](#respond-with-content-using-a-callback)
  * [Respond with content using a callback and extra headers](#respond-with-content-using-a-callback-and-extra-headers)
  * [Respond with content using a callback containing templates](#respond-with-content-using-a-callback-containing-templates)
  * [Respond with content using a callback containing templates and extra headers](#respond-with-content-using-a-callback-containing-templates-and-extra-headers)
  * [Chunked Response](#chunked-response)
  * [Chunked Response containing templates](#chunked-response-containing-templates)
  * [Print to response](#print-to-response)
  * [ArduinoJson Basic Response](#arduinojson-basic-response)
  * [ArduinoJson Advanced Response](#arduinojson-advanced-response)
* [Param Rewrite With Matching](#param-rewrite-with-matching)
* [Using filters](#using-filters)
* [Bad Responses](#bad-responses)
  * [Respond with content using a callback without content length to HTTP/1.0 clients](#respond-with-content-using-a-callback-without-content-length-to-http10-clients)
* [Async WebSocket Plugin](#async-websocket-plugin)
  * [Async WebSocket Event](#async-websocket-event)
  * [Methods for sending data to a socket client](#methods-for-sending-data-to-a-socket-client)
  * [Direct access to web socket message buffer](#direct-access-to-web-socket-message-buffer)
  * [Limiting the number of web socket clients](#limiting-the-number-of-web-socket-clients)
* [Async Event Source Plugin](#async-event-source-plugin)
  * [Setup Event Source on the server](#setup-event-source-on-the-server)
  * [Setup Event Source in the browser](#setup-event-source-in-the-browser)
* [Remove handlers and rewrites](#remove-handlers-and-rewrites)
* [Setting up the server](#setting-up-the-server)
  * [Setup global and class functions as request handlers](#setup-global-and-class-functions-as-request-handlers)
  * [Methods for controlling websocket connections](#methods-for-controlling-websocket-connections)
  * [Adding Default Headers](#adding-default-headers)
  * [Path variable](#path-variable)
* [How to connect W5500 to ESP32_S2/S3/C3](#How-to-connect-W5500-to-ESP32_S2S3C3)
* [Examples](#examples)
  * [ 1. Async_AdvancedWebServer](examples/Async_AdvancedWebServer)
  * [ 2. Async_AdvancedWebServer_MemoryIssues_SendArduinoString](examples/Async_AdvancedWebServer_MemoryIssues_SendArduinoString)
  * [ 3. Async_AdvancedWebServer_MemoryIssues_Send_CString](examples/Async_AdvancedWebServer_MemoryIssues_Send_CString)
  * [ 4. Async_AdvancedWebServer_SendChunked](examples/Async_AdvancedWebServer_SendChunked)
  * [ 5. Async_HelloServer](examples/Async_HelloServer)
  * [ 6. Async_HelloServer2](examples/Async_HelloServer2)
  * [ 7. Async_HttpBasicAuth](examples/Async_HttpBasicAuth)
  * [ 8. AsyncMultiWebServer_ESP32_W5500](examples/AsyncMultiWebServer_ESP32_W5500)
  * [ 9. Async_PostServer](examples/Async_PostServer)
  * [10. Async_RegexPatterns_ESP32_W5500](examples/Async_RegexPatterns_ESP32_W5500)
  * [11. AsyncSimpleServer_ESP32_W5500](examples/AsyncSimpleServer_ESP32_W5500) 
  * [12. AsyncWebServer_SendChunked](examples/AsyncWebServer_SendChunked)
  * [13. Async_WebSocketsServer](examples/Async_WebSocketsServer)
  * [14. MQTTClient_Auth](examples/MQTTClient_Auth) 
  * [15. MQTTClient_Basic](examples/MQTTClient_Basic)
  * [16. MQTT_ThingStream](examples/MQTT_ThingStream)
* [Example Async_AdvancedWebServer](#Example-Async_AdvancedWebServer) 
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. AsyncMultiWebServer_ESP32_W5500 on ESP32S3_DEV with ESP32_S3_W5500](#1-AsyncMultiWebServer_ESP32_W5500-on-ESP32S3_DEV-with-ESP32_S3_W5500)
  * [2. Async_AdvancedWebServer_MemoryIssues_Send_CString on ESP32S3_DEV with ESP32_S3_W5500](#2-Async_AdvancedWebServer_MemoryIssues_Send_CString-on-ESP32S3_DEV-with-ESP32_S3_W5500)
  * [3. Async_AdvancedWebServer_SendChunked on ESP32S3_DEV with ESP32_S3_W5500](#3-Async_AdvancedWebServer_SendChunked-on-ESP32S3_DEV-with-ESP32_S3_W5500)
  * [4. AsyncWebServer_SendChunked on ESP32S3_DEV with ESP32_S3_W5500](#4-AsyncWebServer_SendChunked-on-ESP32S3_DEV-with-ESP32_S3_W5500)
  * [5. Async_WebSocketsServer on ESP32S3_DEV with ESP32_S3_W5500](#5-Async_WebSocketsServer-on-ESP32S3_DEV-with-ESP32_S3_W5500)
  * [6. Async_HTTPBasicAuth on ESP32S3_DEV with ESP32_S3_W5500](#6-Async_HTTPBasicAuth-on-ESP32S3_DEV-with-ESP32_S3_W5500)
  * [7. Async_AdvancedWebServer_SendChunked on ESP32S2_DEV with ESP32_S2_W5500](#7-Async_AdvancedWebServer_SendChunked-on-ESP32S2_DEV-with-ESP32_S2_W5500)
  * [8. Async_AdvancedWebServer_SendChunked on ESP32C3_DEV with ESP32_C3_W5500](#8-Async_AdvancedWebServer_SendChunked-on-ESP32C3_DEV-with-ESP32_C3_W5500)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Important Note

The library permits using `CString` to save heap to send `very large data`.

Check the `marvelleous` PRs of **@salasidis** in [Portenta_H7_AsyncWebServer library](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer)
- [request->send(200, textPlainStr, jsonChartDataCharStr); - Without using String Class - to save heap #8](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer/pull/8)
- [All memmove() removed - string no longer destroyed #11](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer/pull/11)

and these new examples

1. [Async_AdvancedWebServer_MemoryIssues_Send_CString](https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/tree/main/examples/Async_AdvancedWebServer_MemoryIssues_Send_CString)
2. [Async_AdvancedWebServer_MemoryIssues_SendArduinoString](https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/tree/main/examples/Async_AdvancedWebServer_MemoryIssues_SendArduinoString)

If using Arduino String, to send a buffer around `30 KBytes`, the used `Max Heap` is around **142,972 bytes**

If using CString in regular memory, with the same `30 KBytes`, the used `Max Heap` is around **113,200 bytes, saving around a buffer size (30 KBytes)**

This is very critical in use-cases where sending `very large data` is necessary, without `heap-allocation-error`.


1. The traditional function used to send `Arduino String` is


```cpp
void send(int code, const String& contentType = String(), const String& content = String());
```

such as

```cpp
request->send(200, textPlainStr, ArduinoStr);
```
The required additional HEAP is about **3 times of the String size**


2. To use `CString` with copying while sending. Use function


```cpp
void send(int code, const String& contentType, const char *content, bool nonDetructiveSend = true);    // RSMOD
```

such as 

```cpp
request->send(200, textPlainStr, cStr);
```

The required additional HEAP is also about **2 times of the CString size** because of `unnecessary copies` of the CString in HEAP. Avoid this `unefficient` way.


3. To use `CString` without copying while sending. Use function


```cpp
void send(int code, const String& contentType, const char *content, bool nonDetructiveSend = true);    // RSMOD
```

such as 

```cpp
request->send(200, textPlainStr, cStr, false);
```

The required additional HEAP is about **1 times of the CString size**. This way is the best and **most efficient way** to use by avoiding of `unnecessary copies` of the CString in HEAP


---
---

### Why do we need this [AsyncWebServer_ESP32_SC_W5500 library](https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500)


#### Features

This library is based on, modified from:

1. [Hristo Gochkov's ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)

to apply the better and faster **asynchronous** feature of the **powerful** [ESPAsyncWebServer Library](https://github.com/me-no-dev/ESPAsyncWebServer) into **(ESP32_S2/S3/C3 + LwIP W5500)**.


#### Why Async is better

- Using asynchronous network means that you can handle **more than one connection at the same time**
- **You are called once the request is ready and parsed**
- When you send the response, you are **immediately ready** to handle other connections while the server is taking care of sending the response in the background
- **Speed is OMG**
- **Easy to use API, HTTP Basic and Digest MD5 Authentication (default), ChunkedResponse**
- Easily extensible to handle **any type of content**
- Supports Continue 100
- **Async WebSocket plugin offering different locations without extra servers or ports**
- Async EventSource (Server-Sent Events) plugin to send events to the browser
- URL Rewrite plugin for conditional and permanent url rewrites
- ServeStatic plugin that supports cache, Last-Modified, default index and more
- Simple template processing engine to handle templates


---

#### Currently supported Boards


 1. **ESP32_S3 boards** using `LwIP W5500 Ethernet`
 2. **ESP32_S2 boards** using `LwIP W5500 Ethernet`
 3. **ESP32_C3 boards** using `LwIP W5500 Ethernet`
 

---

#### ESP32S2_DEV

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/ESP32S2_DEV.png">
</p> 


#### ESP32S3_DEV

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/ESP32S3_DEV.png">
</p> 


#### ESP32C3_DEV

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/ESP32_C3_DevKitC_02.png">
</p> 

---

#### W5500

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/W5500.png">
</p>
 
<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/W5500_small.png">
</p> 
 

---
---


## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`ESP32 Core 2.0.6+`](https://github.com/espressif/arduino-esp32) for ESP32-based boards. ESP32 Latest Core [![Latest release](https://img.shields.io/github/release/espressif/arduino-esp32.svg)](https://github.com/espressif/arduino-esp32/releases/latest/)
 3. [`AsyncTCP library v1.1.1+`](https://github.com/me-no-dev/AsyncTCP).

---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for `AsyncWebServer_ESP32_SC_W5500`, then select / install the latest version. You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncWebServer_ESP32_SC_W5500.svg?)](https://www.ardu-badge.com/AsyncWebServer_ESP32_SC_W5500) for more detailed instructions.

### Manual Install

1. Navigate to [AsyncWebServer_ESP32_SC_W5500](https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500) page.
2. Download the latest release `AsyncWebServer_ESP32_SC_W5500-main.zip`.
3. Extract the zip file to `AsyncWebServer_ESP32_SC_W5500-main` directory 
4. Copy the whole `AsyncWebServer_ESP32_SC_W5500-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**AsyncWebServer_ESP32_SC_W5500** library](https://registry.platformio.org/libraries/khoih-prog/AsyncWebServer_ESP32_SC_W5500) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/AsyncWebServer_ESP32_SC_W5500/installation). Search for **AsyncWebServer_ESP32_SC_W5500** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---

## Important things to remember

- This is **fully asynchronous server** and as such does not run on the `loop()` thread.
- You can not use `yield()` or `delay()` or any function that uses them inside the callbacks
- The server is smart enough to know when to close the connection and free resources
- You can not send more than one response to a single request

---

## Principles of operation

### The Async Web server

- Listens for connections
- Wraps the new clients into `Request`
- Keeps track of clients and cleans memory
- Manages `Rewrites` and apply them on the request url
- Manages `Handlers` and attaches them to Requests

### Request Life Cycle

- TCP connection is received by the server
- The connection is wrapped inside `Request` object
- When the request head is received (type, url, get params, http version and host),
  the server goes through all `Rewrites` (in the order they were added) to rewrite the url and inject query parameters,
  next, it goes through all attached `Handlers` (in the order they were added) trying to find one
  that `canHandle` the given request. If none are found, the default(catch-all) handler is attached.
- The rest of the request is received, calling the `handleUpload` or `handleBody` methods of the `Handler` if they are needed (POST+File/Body)
- When the whole request is parsed, the result is given to the `handleRequest` method of the `Handler` and is ready to be responded to
- In the `handleRequest` method, to the `Request` is attached a `Response` object (see below) that will serve the response data back to the client
- When the `Response` is sent, the client is closed and freed from the memory

### Rewrites and how do they work

- The `Rewrites` are used to rewrite the request url and/or inject get parameters for a specific request url path.
- All `Rewrites` are evaluated on the request in the order they have been added to the server.
- The `Rewrite` will change the request url only if the request url (excluding get parameters) is fully match
  the rewrite url, and when the optional `Filter` callback return true.
- Setting a `Filter` to the `Rewrite` enables to control when to apply the rewrite, decision can be based on
  request url, http version, request host/port/target host, get parameters or the request client's localIP or remoteIP.
- The `Rewrite` can specify a target url with optional get parameters, e.g. `/to-url?with=params`

### Handlers and how do they work

- The `Handlers` are used for executing specific actions to particular requests
- One `Handler` instance can be attached to any request and lives together with the server
- Setting a `Filter` to the `Handler` enables to control when to apply the handler, decision can be based on
  request url, http version, request host/port/target host, get parameters or the request client's localIP or remoteIP.
- The `canHandle` method is used for handler specific control on whether the requests can be handled
  and for declaring any interesting headers that the `Request` should parse. Decision can be based on request
  method, request url, http version, request host/port/target host and get parameters
- Once a `Handler` is attached to given `Request` (`canHandle` returned true)
  that `Handler` takes care to receive any file/data upload and attach a `Response`
  once the `Request` has been fully parsed
- `Handlers` are evaluated in the order they are attached to the server. The `canHandle` is called only
  if the `Filter` that was set to the `Handler` return true.
- The first `Handler` that can handle the request is selected, not further `Filter` and `canHandle` are called.

### Responses and how do they work

- The `Response` objects are used to send the response data back to the client
- The `Response` object lives with the `Request` and is freed on end or disconnect
- Different techniques are used depending on the response type to send the data in packets
  returning back almost immediately and sending the next packet when this one is received.
  Any time in between is spent to run the user loop and handle other network packets
- Responding asynchronously is probably the most difficult thing for most to understand
- Many different options exist for the user to make responding a background task

### Template processing

- `AsyncWebServer_ESP32_SC_W5500` contains simple template processing engine.
- Template processing can be added to most response types.
- Currently it supports only replacing template placeholders with actual values. No conditional processing, cycles, etc.
- Placeholders are delimited with `%` symbols. Like this: `%TEMPLATE_PLACEHOLDER%`.
- It works by extracting placeholder name from response text and passing it to user provided function which should return actual value to be used instead of placeholder.
- Since it's user provided function, it is possible for library users to implement conditional processing and cycles themselves.
- Since it's impossible to know the actual response size after template processing step in advance (and, therefore, to include it in response headers), the response becomes [chunked](#chunked-response).

---

## Request Variables

### Common Variables

```cpp
request->version();       // uint8_t: 0 = HTTP/1.0, 1 = HTTP/1.1
request->method();        // enum:    HTTP_GET, HTTP_POST, HTTP_DELETE, HTTP_PUT, HTTP_PATCH, HTTP_HEAD, HTTP_OPTIONS
request->url();           // String:  URL of the request (not including host, port or GET parameters)
request->host();          // String:  The requested host (can be used for virtual hosting)
request->contentType();   // String:  ContentType of the request (not available in Handler::canHandle)
request->contentLength(); // size_t:  ContentLength of the request (not available in Handler::canHandle)
request->multipart();     // bool:    True if the request has content type "multipart"
```

### Headers

```cpp
//List all collected headers
int headers = request->headers();
int i;

for (i=0;i<headers;i++)
{
  AsyncWebHeader* h = request->getHeader(i);
  Serial.printf("HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
}

//get specific header by name
if (request->hasHeader("MyHeader"))
{
  AsyncWebHeader* h = request->getHeader("MyHeader");
  Serial.printf("MyHeader: %s\n", h->value().c_str());
}

//List all collected headers (Compatibility)
int headers = request->headers();
int i;

for (i=0;i<headers;i++)
{
  Serial.printf("HEADER[%s]: %s\n", request->headerName(i).c_str(), request->header(i).c_str());
}

//get specific header by name (Compatibility)
if (request->hasHeader("MyHeader"))
{
  Serial.printf("MyHeader: %s\n", request->header("MyHeader").c_str());
}
```

### GET, POST and FILE parameters

```cpp
//List all parameters
int params = request->params();

for (int i=0;i<params;i++)
{
  AsyncWebParameter* p = request->getParam(i);
  
  if (p->isFile())
  { 
    //p->isPost() is also true
    Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
  } 
  else if (p->isPost())
  {
    Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
  } 
  else 
  {
    Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
  }
}

//Check if GET parameter exists
if (request->hasParam("download"))
  AsyncWebParameter* p = request->getParam("download");

//Check if POST (but not File) parameter exists
if (request->hasParam("download", true))
  AsyncWebParameter* p = request->getParam("download", true);

//Check if FILE was uploaded
if (request->hasParam("download", true, true))
  AsyncWebParameter* p = request->getParam("download", true, true);

//List all parameters (Compatibility)
int args = request->args();

for (int i=0;i<args;i++)
{
  Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
}

//Check if parameter exists (Compatibility)
if (request->hasArg("download"))
  String arg = request->arg("download");
```

### JSON body handling with ArduinoJson

Endpoints which consume JSON can use a special handler to get ready to use JSON data in the request callback:

```cpp
#include "AsyncJson.h"
#include "ArduinoJson.h"

AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/rest/endpoint", [](AsyncWebServerRequest *request, JsonVariant &json) 
{
  JsonObject& jsonObj = json.as<JsonObject>();
  // ...
});

server.addHandler(handler);
```
---

## Responses

### Redirect to another URL

```cpp
//to local url
request->redirect("/login");

//to external url
request->redirect("http://esp8266.com");
```

### Basic response with HTTP Code

```cpp
request->send(404); //Sends 404 File Not Found
```

### Basic response with HTTP Code and extra headers

```cpp
AsyncWebServerResponse *response = request->beginResponse(404); //Sends 404 File Not Found
response->addHeader("Server","AsyncWebServer_ESP32_SC_W5500");
request->send(response);
```

### Basic response with string content

```cpp
request->send(200, "text/plain", "Hello World!");
```

### Basic response with string content and extra headers

```cpp
AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Hello World!");
response->addHeader("Server","AsyncWebServer");
request->send(response);
```

### Respond with content coming from a Stream

```cpp
//read 12 bytes from Serial and send them as Content Type text/plain
request->send(Serial, "text/plain", 12);
```

### Respond with content coming from a Stream and extra headers

```cpp
//read 12 bytes from Serial and send them as Content Type text/plain
AsyncWebServerResponse *response = request->beginResponse(Serial, "text/plain", 12);
response->addHeader("Server","AsyncWebServer_ESP32_SC_W5500");
request->send(response);
```

### Respond with content coming from a Stream containing templates

```cpp
String processor(const String& var)
{
  if (var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
    
  return String();
}

// ...

//read 12 bytes from Serial and send them as Content Type text/plain
request->send(Serial, "text/plain", 12, processor);
```

### Respond with content coming from a Stream containing templates and extra headers

```cpp
String processor(const String& var)
{
  if (var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
  return String();
}

// ...

//read 12 bytes from Serial and send them as Content Type text/plain
AsyncWebServerResponse *response = request->beginResponse(Serial, "text/plain", 12, processor);
response->addHeader("Server","AsyncWebServer_ESP32_SC_W5500");
request->send(response);
```

### Respond with content using a callback

```cpp
//send 128 bytes as plain text
request->send("text/plain", 128, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will not be asked for more bytes once the content length has been reached.
  //Keep in mind that you can not delay or yield waiting for more data!
  //Send what you currently have and you will be asked for more again
  return mySource.read(buffer, maxLen);
});
```

### Respond with content using a callback and extra headers

```cpp
//send 128 bytes as plain text
AsyncWebServerResponse *response = request->beginResponse("text/plain", 128, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will not be asked for more bytes once the content length has been reached.
  //Keep in mind that you can not delay or yield waiting for more data!
  //Send what you currently have and you will be asked for more again
  return mySource.read(buffer, maxLen);
});

response->addHeader("Server","AsyncWebServer_ESP32_SC_W5500");
request->send(response);
```

### Respond with content using a callback containing templates

```cpp
String processor(const String& var)
{
  if (var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
    
  return String();
}

// ...

//send 128 bytes as plain text
request->send("text/plain", 128, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will not be asked for more bytes once the content length has been reached.
  //Keep in mind that you can not delay or yield waiting for more data!
  //Send what you currently have and you will be asked for more again
  return mySource.read(buffer, maxLen);
}, processor);
```

### Respond with content using a callback containing templates and extra headers

```cpp
String processor(const String& var)
{
  if (var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
  return String();
}

// ...

//send 128 bytes as plain text
AsyncWebServerResponse *response = request->beginResponse("text/plain", 128, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will not be asked for more bytes once the content length has been reached.
  //Keep in mind that you can not delay or yield waiting for more data!
  //Send what you currently have and you will be asked for more again
  return mySource.read(buffer, maxLen);
}, processor);

response->addHeader("Server","AsyncWebServer_ESP32_SC_W5500");
request->send(response);
```

### Chunked Response

Used when content length is unknown. Works best if the client supports HTTP/1.1

```cpp
AsyncWebServerResponse *response = request->beginChunkedResponse("text/plain", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will be asked for more data until 0 is returned
  //Keep in mind that you can not delay or yield waiting for more data!
  return mySource.read(buffer, maxLen);
});

response->addHeader("Server","AsyncWebServer_ESP32_SC_W5500");
request->send(response);
```

### Chunked Response containing templates

Used when content length is unknown. Works best if the client supports HTTP/1.1

```cpp
String processor(const String& var)
{
  if (var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
    
  return String();
}

// ...

AsyncWebServerResponse *response = request->beginChunkedResponse("text/plain", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will be asked for more data until 0 is returned
  //Keep in mind that you can not delay or yield waiting for more data!
  return mySource.read(buffer, maxLen);
}, processor);

response->addHeader("Server","AsyncWebServer_ESP32_SC_W5500");
request->send(response);
```

### Print to response

```cpp
AsyncResponseStream *response = request->beginResponseStream("text/html");
response->addHeader("Server","AsyncWebServer_ESP32_SC_W5500");
response->printf("<!DOCTYPE html><html><head><title>Webpage at %s</title></head><body>", request->url().c_str());

response->print("<h2>Hello ");
response->print(request->client()->remoteIP());
response->print("</h2>");

response->print("<h3>General</h3>");
response->print("<ul>");
response->printf("<li>Version: HTTP/1.%u</li>", request->version());
response->printf("<li>Method: %s</li>", request->methodToString());
response->printf("<li>URL: %s</li>", request->url().c_str());
response->printf("<li>Host: %s</li>", request->host().c_str());
response->printf("<li>ContentType: %s</li>", request->contentType().c_str());
response->printf("<li>ContentLength: %u</li>", request->contentLength());
response->printf("<li>Multipart: %s</li>", request->multipart()?"true":"false");
response->print("</ul>");

response->print("<h3>Headers</h3>");
response->print("<ul>");
int headers = request->headers();

for (int i=0;i<headers;i++)
{
  AsyncWebHeader* h = request->getHeader(i);
  response->printf("<li>%s: %s</li>", h->name().c_str(), h->value().c_str());
}

response->print("</ul>");

response->print("<h3>Parameters</h3>");
response->print("<ul>");

int params = request->params();

for (int i=0;i<params;i++)
{
  AsyncWebParameter* p = request->getParam(i);
  
  if (p->isFile())
  {
    response->printf("<li>FILE[%s]: %s, size: %u</li>", p->name().c_str(), p->value().c_str(), p->size());
  } 
  else if (p->isPost())
  {
    response->printf("<li>POST[%s]: %s</li>", p->name().c_str(), p->value().c_str());
  } 
  else 
  {
    response->printf("<li>GET[%s]: %s</li>", p->name().c_str(), p->value().c_str());
  }
}

response->print("</ul>");

response->print("</body></html>");
//send the response last
request->send(response);
```

### ArduinoJson Basic Response

This way of sending Json is great for when the result is **below 4KB**

```cpp
#include "AsyncJson.h"
#include "ArduinoJson.h"

AsyncResponseStream *response = request->beginResponseStream("application/json");
DynamicJsonBuffer jsonBuffer;
JsonObject &root = jsonBuffer.createObject();
root["heap"] = ESP.getFreeHeap();
root["ssid"] = WiFi.SSID();
root.printTo(*response);

request->send(response);
```

### ArduinoJson Advanced Response

This response can handle really **large Json objects (tested to 40KB)**

There isn't any noticeable speed decrease for small results with the method above

Since ArduinoJson does not allow reading parts of the string, the whole Json has to be passed every time a 
chunks needs to be sent, which shows speed decrease proportional to the resulting json packets

```cpp
#include "AsyncJson.h"
#include "ArduinoJson.h"

AsyncJsonResponse * response = new AsyncJsonResponse();
response->addHeader("Server","AsyncWebServer");
JsonObject& root = response->getRoot();
root["IP"] = Ethernet.localIP();
response->setLength();

request->send(response);
```
---

## Param Rewrite With Matching

It is possible to rewrite the request url with parameter matchg. Here is an example with one parameter:
Rewrite for example "/radio/{frequence}" -> "/radio?f={frequence}"

```cpp
class OneParamRewrite : public AsyncWebRewrite
{
  protected:
    String _urlPrefix;
    int _paramIndex;
    String _paramsBackup;

  public:
  OneParamRewrite(const char* from, const char* to)
    : AsyncWebRewrite(from, to) 
    {

      _paramIndex = _from.indexOf('{');

      if ( _paramIndex >=0 && _from.endsWith("}")) 
      {
        _urlPrefix = _from.substring(0, _paramIndex);
        int index = _params.indexOf('{');
        
        if (index >= 0) 
        {
          _params = _params.substring(0, index);
        }
      } 
      else 
      {
        _urlPrefix = _from;
      }
      
      _paramsBackup = _params;
  }

  bool match(AsyncWebServerRequest *request) override 
  {
    if (request->url().startsWith(_urlPrefix)) 
    {
      if (_paramIndex >= 0) 
      {
        _params = _paramsBackup + request->url().substring(_paramIndex);
      } 
      else 
      {
        _params = _paramsBackup;
      }
      
      return true;

    } 
    else 
    {
      return false;
    }
  }
};
```

Usage:

```cpp
  server.addRewrite( new OneParamRewrite("/radio/{frequence}", "/radio?f={frequence}") );
```
---

## Using filters

Filters can be set to `Rewrite` or `Handler` in order to control when to apply the rewrite and consider the handler.
A filter is a callback function that evaluates the request and return a boolean `true` to include the item
or `false` to exclude it.

---

## Bad Responses

Some responses are implemented, but you should not use them, because they do not conform to HTTP.
The following example will lead to unclean close of the connection and more time wasted
than providing the length of the content

### Respond with content using a callback without content length to HTTP/1.0 clients

```cpp
//This is used as fallback for chunked responses to HTTP/1.0 Clients
request->send("text/plain", 0, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //You will be asked for more data until 0 is returned
  //Keep in mind that you can not delay or yield waiting for more data!
  return mySource.read(buffer, maxLen);
});
```

---

## Async WebSocket Plugin

The server includes a web socket plugin which lets you define different WebSocket locations to connect to
without starting another listening service or using different port

### Async WebSocket Event

```cpp
void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    //client connected
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
  } 
  else if (type == WS_EVT_DISCONNECT)
  {
    //client disconnected
    Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  } 
  else if (type == WS_EVT_ERROR)
  {
    //error was received from the other end
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } 
  else if (type == WS_EVT_PONG)
  {
    //pong message was received (in response to a ping request maybe)
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } 
  else if (type == WS_EVT_DATA)
  {
    //data packet
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    
    if (info->final && info->index == 0 && info->len == len)
    {
      //the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
      
      if (info->opcode == WS_TEXT)
      {
        data[len] = 0;
        Serial.printf("%s\n", (char*)data);
      } 
      else 
      {
        for (size_t i=0; i < info->len; i++)
        {
          Serial.printf("%02x ", data[i]);
        }
        
        Serial.printf("\n");
      }
      
      if (info->opcode == WS_TEXT)
        client->text("I got your text message");
      else
        client->binary("I got your binary message");
    } 
    else 
    {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if (info->index == 0)
      {
        if (info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
      
      if (info->message_opcode == WS_TEXT)
      {
        data[len] = 0;
        Serial.printf("%s\n", (char*)data);
      } 
      else 
      {
        for (size_t i=0; i < len; i++){
          Serial.printf("%02x ", data[i]);
        }
        Serial.printf("\n");
      }

      if ((info->index + len) == info->len)
      {
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        
        if (info->final)
        {
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          
          if (info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}
```

### Methods for sending data to a socket client

```cpp
//Server methods
AsyncWebSocket ws("/ws");
//printf to a client
ws.printf((uint32_t)client_id, arguments...);
//printf to all clients
ws.printfAll(arguments...);
//send text to a client
ws.text((uint32_t)client_id, (char*)text);
ws.text((uint32_t)client_id, (uint8_t*)text, (size_t)len);
//send text to all clients
ws.textAll((char*)text);
ws.textAll((uint8_t*)text, (size_t)len);
//send binary to a client
ws.binary((uint32_t)client_id, (char*)binary);
ws.binary((uint32_t)client_id, (uint8_t*)binary, (size_t)len);
ws.binary((uint32_t)client_id, flash_binary, 4);
//send binary to all clients
ws.binaryAll((char*)binary);
ws.binaryAll((uint8_t*)binary, (size_t)len);
//HTTP Authenticate before switch to Websocket protocol
ws.setAuthentication("user", "pass");

//client methods
AsyncWebSocketClient * client;
//printf
client->printf(arguments...);
//send text
client->text((char*)text);
client->text((uint8_t*)text, (size_t)len);
//send binary
client->binary((char*)binary);
client->binary((uint8_t*)binary, (size_t)len);
```

### Direct access to web socket message buffer

When sending a web socket message using the above methods a buffer is created.  Under certain circumstances you might want to manipulate or populate this buffer directly from your application, for example to prevent unnecessary duplications of the data.  This example below shows how to create a buffer and print data to it from an ArduinoJson object then send it.

```cpp
void sendDataWs(AsyncWebSocketClient * client)
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["a"] = "abc";
  root["b"] = "abcd";
  root["c"] = "abcde";
  root["d"] = "abcdef";
  root["e"] = "abcdefg";
  size_t len = root.measureLength();
  AsyncWebSocketMessageBuffer * buffer = ws.makeBuffer(len); //  creates a buffer (len + 1) for you.
  
  if (buffer) 
  {
    root.printTo((char *)buffer->get(), len + 1);
    
    if (client) 
    {
        client->text(buffer);
    } 
    else 
    {
        ws.textAll(buffer);
    }
  }
}
```

### Limiting the number of web socket clients

Browsers sometimes do not correctly close the websocket connection, even when the `close()` function is called in javascript.  This will eventually exhaust the web server's resources and will cause the server to crash.  Periodically calling the `cleanClients()` function from the main `loop()` function limits the number of clients by closing the oldest client when the maximum number of clients has been exceeded.  This can called be every cycle, however, if you wish to use less power, then calling as infrequently as once per second is sufficient.

```cpp
void loop()
{
  ws.cleanupClients();
}
```

---

## Async Event Source Plugin

The server includes `EventSource` (Server-Sent Events) plugin which can be used to send short text events to the browser.
Difference between `EventSource` and `WebSockets` is that `EventSource` is single direction, text-only protocol.

### Setup Event Source on the server

```cpp
AsyncWebServer server(80);
AsyncEventSource events("/events");

void setup()
{
  // setup ......
  events.onConnect([](AsyncEventSourceClient *client)
  {
    if (client->lastId())
    {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    
    //send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!",NULL,millis(),1000);
  });
  
  //HTTP Basic authentication
  events.setAuthentication("user", "pass");
  server.addHandler(&events);
  // setup ......
}

void loop()
{
  if (eventTriggered)
  { 
    // your logic here
    //send event "myevent"
    events.send("my event content","myevent",millis());
  }
}
```

### Setup Event Source in the browser

```javascript
if (!!window.EventSource) 
{
  var source = new EventSource('/events');

  source.addEventListener('open', function(e) 
  {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) 
  {
    if (e.target.readyState != EventSource.OPEN) 
    {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('message', function(e) 
  {
    console.log("message", e.data);
  }, false);

  source.addEventListener('myevent', function(e) 
  {
    console.log("myevent", e.data);
  }, false);
}
```
---

## Remove handlers and rewrites

Server goes through handlers in same order as they were added. You can't simple add handler with same path to override them.
To remove handler:

```cpp
// save callback for particular URL path
auto handler = server.on("/some/path", [](AsyncWebServerRequest *request)
{
  //do something useful
});

// when you don't need handler anymore remove it
server.removeHandler(&handler);

// same with rewrites
server.removeRewrite(&someRewrite);

server.onNotFound([](AsyncWebServerRequest *request)
{
  request->send(404);
});

// remove server.onNotFound handler
server.onNotFound(NULL);

// remove all rewrites, handlers and onNotFound/onFileUpload/onRequestBody callbacks
server.reset();
```
---

## Setting up the server

```cpp
#if !( defined(ESP32) )
  #error This code is designed for (ESP32_S2/3, ESP32_C3 + W5500) to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#include <Arduino.h>

#define _ASYNC_WEBSERVER_LOGLEVEL_       2

// Enter a MAC address and IP address for your controller below.
#define NUMBER_OF_MAC      20

byte mac[][NUMBER_OF_MAC] =
{
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x02 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x04 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x06 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x07 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x08 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x09 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0A },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0B },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0C },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0E },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0F },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x10 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x12 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x13 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x14 },
};

// Select the IP address according to your local network
IPAddress myIP(192, 168, 2, 232);
IPAddress myGW(192, 168, 2, 1);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);

//////////////////////////////////////////////////////////

// For W5500 & ESP32-S3
// Optional values to override default settings
// Don't change unless you know what you're doing
//#define ETH_SPI_HOST        SPI3_HOST
//#define SPI_CLOCK_MHZ       25

// Must connect INT to GPIOxx or not working
//#define INT_GPIO            4

//#define MISO_GPIO           13
//#define MOSI_GPIO           11
//#define SCK_GPIO            12
//#define CS_GPIO             10

//////////////////////////////////////////////////////////

#include <AsyncTCP.h>

#include <AsyncWebServer_ESP32_SC_W5500.h>

AsyncWebServer    server(80);

void handleRoot(AsyncWebServerRequest *request)
{
  request->send(200, "text/plain", String("Hello from Async_HelloServer on ") + ARDUINO_BOARD );
}

void handleNotFound(AsyncWebServerRequest *request)
{
  String message = "File Not Found\n\n";

  message += "URI: ";
  //message += server.uri();
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";

  for (uint8_t i = 0; i < request->args(); i++)
  {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }

  request->send(404, "text/plain", message);
}

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  delay(500);

  Serial.print(F("\nStart Async_HelloServer on "));
  Serial.print(ARDUINO_BOARD);
  Serial.print(F(" with "));
  Serial.println(SHIELD_TYPE);
  Serial.println(ASYNC_WEBSERVER_ESP32_SC_W5500_VERSION);

  AWS_LOGWARN(F("Default SPI pinout:"));
  AWS_LOGWARN1(F("SPI_HOST:"), ETH_SPI_HOST);
  AWS_LOGWARN1(F("MOSI:"), MOSI_GPIO);
  AWS_LOGWARN1(F("MISO:"), MISO_GPIO);
  AWS_LOGWARN1(F("SCK:"),  SCK_GPIO);
  AWS_LOGWARN1(F("CS:"),   CS_GPIO);
  AWS_LOGWARN1(F("INT:"),  INT_GPIO);
  AWS_LOGWARN1(F("SPI Clock (MHz):"), SPI_CLOCK_MHZ);
  AWS_LOGWARN(F("========================="));

  ///////////////////////////////////

  // To be called before ETH.begin()
  ESP32_W5500_onEvent();

  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  uint16_t index = millis() % NUMBER_OF_MAC;

  //bool begin(int MISO_GPIO, int MOSI_GPIO, int SCLK_GPIO, int CS_GPIO, int INT_GPIO, int SPI_CLOCK_MHZ,
  //           int SPI_HOST, uint8_t *W5500_Mac = W5500_Default_Mac);
  //ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST );
  ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST, mac[index] );

  // Static IP, leave without this line to get IP via DHCP
  //bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
  //ETH.config(myIP, myGW, mySN, myDNS);

  ESP32_W5500_waitForConnect();

  ///////////////////////////////////


  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    handleRoot(request);
  });

  server.on("/inline", [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", "This works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();

  Serial.print(F("HTTP EthernetWebServer is @ IP : "));
  Serial.println(ETH.localIP());
}

void loop()
{
}
```
---

### Setup global and class functions as request handlers

```cpp
#include <Arduino.h>

#include <AsyncWebServer_ESP32_SC_W5500.h>

...

void handleRequest(AsyncWebServerRequest *request){}

class WebClass 
{
public :
  AsyncWebServer classWebServer = AsyncWebServer(81);

  WebClass(){};

  void classRequest (AsyncWebServerRequest *request){}

  void begin()
  {
    // attach global request handler
    classWebServer.on("/example", HTTP_ANY, handleRequest);

    // attach class request handler
    classWebServer.on("/example", HTTP_ANY, std::bind(&WebClass::classRequest, this, std::placeholders::_1));
  }
};

AsyncWebServer globalWebServer(80);
WebClass webClassInstance;

void setup() 
{
  // attach global request handler
  globalWebServer.on("/example", HTTP_ANY, handleRequest);

  // attach class request handler
  globalWebServer.on("/example", HTTP_ANY, std::bind(&WebClass::classRequest, webClassInstance, std::placeholders::_1));
}

void loop() 
{
}
```

### Methods for controlling websocket connections

```cpp
// Disable client connections if it was activated
if ( ws.enabled() )
  ws.enable(false);

// enable client connections if it was disabled
if ( !ws.enabled() )
  ws.enable(true);
```


### Adding Default Headers

In some cases, such as when working with CORS, or with some sort of custom authentication system, 
you might need to define a header that should get added to all responses (including static, websocket and EventSource).
The DefaultHeaders singleton allows you to do this.

Example:

```cpp
DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
webServer.begin();
```

*NOTE*: You will still need to respond to the OPTIONS method for CORS pre-flight in most cases. (unless you are only using GET)

This is one option:

```cpp
webServer.onNotFound([](AsyncWebServerRequest *request) 
{
  if (request->method() == HTTP_OPTIONS) 
  {
    request->send(200);
  } 
  else 
  {
    request->send(404);
  }
});
```

---

### Path variable

With path variable you can create a custom regex rule for a specific parameter in a route. 
For example we want a `sensorId` parameter in a route rule to match only a integer.

```cpp
server.on("^\\/sensor\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) 
{
    String sensorId = request->pathArg(0);
});
```

*NOTE*: All regex patterns starts with `^` and ends with `$`

To enable the `Path variable` support, you have to define the buildflag `-DASYNCWEBSERVER_REGEX`.


For Arduino IDE create/update `platform.local.txt`:

`Windows`: C:\Users\(username)\AppData\Local\Arduino15\packages\\`{espxxxx}`\hardware\\`espxxxx`\\`{version}`\platform.local.txt

`Linux`: ~/.arduino15/packages/`{espxxxx}`/hardware/`{espxxxx}`/`{version}`/platform.local.txt

Add/Update the following line:

```
compiler.cpp.extra_flags=-DDASYNCWEBSERVER_REGEX
```

For platformio modify `platformio.ini`:

```ini
[env:myboard]
build_flags = 
  -DASYNCWEBSERVER_REGEX
```

*NOTE*: By enabling `ASYNCWEBSERVER_REGEX`, `<regex>` will be included. This will add an 100k to your binary.


---
---

### How to connect W5500 to ESP32_S2/S3/C3

You can change the `INT` pin to another one. Default is `GPIO4`

```cpp
// Must connect INT to GPIOxx or not working
#define INT_GPIO            4
```

---

#### W5500

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/W5500.png">
</p>
 
<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/W5500_small.png">
</p> 
 
--- 
 

#### ESP32S3_DEV

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/ESP32S3_DEV.png">
</p> 


|W5500|<--->|ESP32_S3|
|:-:|:-:|:-:|
|MOSI|<--->|GPIO11|
|MISO|<--->|GPIO13|
|SCK|<--->|GPIO12|
|SS|<--->|GPIO10|
|INT|<--->|GPIO4|
|RST|<--->|RST|
|GND|<--->|GND|
|3.3V|<--->|3.3V|


---

#### ESP32S2_DEV

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/ESP32S2_DEV.png">
</p> 


|ENC28J60|<--->|ESP32_S2|
|:-:|:-:|:-:|
|MOSI|<--->|GPIO35|
|MISO|<--->|GPIO37|
|SCK|<--->|GPIO36|
|SS|<--->|GPIO34|
|INT|<--->|GPIO4|
|RST|<--->|RST|
|GND|<--->|GND|
|3.3V|<--->|3.3V|

---


#### ESP32C3_DEV

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/ESP32_C3_DevKitC_02.png">
</p> 


|W5500|<--->|ESP32_C3|
|:-:|:-:|:-:|
|MOSI|<--->|GPIO6|
|MISO|<--->|GPIO5|
|SCK|<--->|GPIO4|
|SS|<--->|GPIO7|
|INT|<--->|GPIO10|
|RST|<--->|RST|
|GND|<--->|GND|
|3.3V|<--->|3.3V|




---
---

### Examples

 1. [Async_AdvancedWebServer](examples/Async_AdvancedWebServer)
 2. [Async_AdvancedWebServer_MemoryIssues_SendArduinoString](examples/Async_AdvancedWebServer_MemoryIssues_SendArduinoString) 
 3. [Async_AdvancedWebServer_MemoryIssues_Send_CString](examples/Async_AdvancedWebServer_MemoryIssues_Send_CString)
 4. [Async_AdvancedWebServer_SendChunked](examples/Async_AdvancedWebServer_SendChunked)
 5. [Async_HelloServer](examples/Async_HelloServer)
 6. [Async_HelloServer2](examples/Async_HelloServer2)
 7. [Async_HttpBasicAuth](examples/Async_HttpBasicAuth)
 8. [AsyncMultiWebServer_ESP32_W5500](examples/AsyncMultiWebServer_ESP32_W5500)
 9. [Async_PostServer](examples/Async_PostServer)
10. [Async_RegexPatterns_ESP32_W5500](examples/Async_RegexPatterns_ESP32_W5500)
11. [AsyncSimpleServer_ESP32_W5500](examples/AsyncSimpleServer_ESP32_W5500)
12. [AsyncWebServer_SendChunked](examples/AsyncWebServer_SendChunked)
13. [Async_WebSocketsServer](examples/Async_WebSocketsServer)
14. [MQTTClient_Auth](examples/MQTTClient_Auth)
15. [MQTTClient_Basic](examples/MQTTClient_Basic)
16. [MQTT_ThingStream](examples/MQTT_ThingStream)
 
  
---
---

### Example [Async_AdvancedWebServer](examples/Async_AdvancedWebServer)

https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/blob/c87fe15ac7d54b6b0a4ddd0c6450f162b38a0e4b/examples/Async_AdvancedWebServer/Async_AdvancedWebServer.ino#L40-L270


You can access the Async Advanced WebServer @ the server IP

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/Async_AdvancedWebServer.png">
</p>

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/AsyncWebServer_SendChunked_ESP32_S2.png">
</p>

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/AsyncWebServer_SendChunked_ESP32_C3.png">
</p>

---
---

### Debug Terminal Output Samples

#### 1. AsyncMultiWebServer_ESP32_W5500 on ESP32S3_DEV with ESP32_S3_W5500

Following are debug terminal output and screen shots when running example [AsyncMultiWebServer_ESP32_W5500](examples/AsyncMultiWebServer_ESP32_W5500) on `ESP32S3_DEV with LwIP W5500`, using ESP32 core `v2.0.0+`, to demonstrate the operation of 3 independent AsyncWebServers on 3 different ports and how to handle the complicated AsyncMultiWebServers.


```cpp
Start AsyncMultiWebServer_ESP32_W5500 on ESP32S3_DEV with ESP32_S3_W5500
AsyncWebServer_ESP32_SC_W5500 v1.8.1 for core v2.0.0+
[AWS] Default SPI pinout:
[AWS] SPI_HOST: 2
[AWS] MOSI: 11
[AWS] MISO: 13
[AWS] SCK: 12
[AWS] CS: 10
[AWS] INT: 4
[AWS] SPI Clock (MHz): 25
[AWS] =========================

ETH Started
ETH Connected
ETH MAC: DE:AD:BE:EF:FE:11, IPv4: 192.168.2.101
FULL_DUPLEX, 100Mbps

Connected to network. IP = 192.168.2.101
Initialize multiServer OK, serverIndex = 0, port = 8080
HTTP server started at ports 8080
Initialize multiServer OK, serverIndex = 1, port = 8081
HTTP server started at ports 8081
Initialize multiServer OK, serverIndex = 2, port = 8082
HTTP server started at ports 8082
```

You can access the Async Advanced WebServers @ the server IP and corresponding ports (8080, 8081 and 8082)

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/AsyncMultiWebServer_ESP32_W5500_SVR1.png">
</p>

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/AsyncMultiWebServer_ESP32_W5500_SVR2.png">
</p>

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/AsyncMultiWebServer_ESP32_W5500_SVR3.png">
</p>

---

#### 2. Async_AdvancedWebServer_MemoryIssues_Send_CString on ESP32S3_DEV with ESP32_S3_W5500

Following is the debug terminal and screen shot when running example [Async_AdvancedWebServer_MemoryIssues_Send_CString](examples/Async_AdvancedWebServer_MemoryIssues_Send_CString), on `ESP32S3_DEV with LwIP W5500`, to demonstrate the new and powerful `HEAP-saving` feature


##### Using CString  ===> smaller heap (113,200 bytes)

```cpp
Start Async_AdvancedWebServer_MemoryIssues_Send_CString on ESP32S3_DEV with ESP32_S3_W5500
AsyncWebServer_ESP32_SC_W5500 v1.8.1 for core v2.0.0+
[AWS] Default SPI pinout:
[AWS] SPI_HOST: 2
[AWS] MOSI: 11
[AWS] MISO: 13
[AWS] SCK: 12
[AWS] CS: 10
[AWS] INT: 4
[AWS] SPI Clock (MHz): 25
[AWS] =========================

ETH Started
ETH Connected
ETH MAC: DE:AD:BE:EF:FE:11, IPv4: 192.168.2.101
FULL_DUPLEX, 100Mbps
HTTP EthernetWebServer is @ IP : 192.168.2.101

HEAP DATA - Pre Create Arduino String  Max heap: 359588  Free heap: 258244  Used heap: 101344
..
HEAP DATA - Pre Send  Max heap: 359588  Free heap: 254396  Used heap: 105192

HEAP DATA - Post Send  Max heap: 359588  Free heap: 246396  Used heap: 111652
.
HEAP DATA - Post Send  Max heap: 359588  Free heap: 246388  Used heap: 113200
....... .
```

While using `Arduino String`, the HEAP usage is very large


#### Async_AdvancedWebServer_MemoryIssues_SendArduinoString  ===> very large heap (142,972 bytes)

```cpp
Start Async_AdvancedWebServer_MemoryIssues_SendArduinoString on ESP32S3_DEV with ESP32_S3_W5500
AsyncWebServer_ESP32_SC_W5500 v1.8.1 for core v2.0.0+
[AWS] Default SPI pinout:
[AWS] SPI_HOST: 2
[AWS] MOSI: 11
[AWS] MISO: 13
[AWS] SCK: 12
[AWS] CS: 10
[AWS] INT: 4
[AWS] SPI Clock (MHz): 25
[AWS] =========================

ETH Started
ETH Connected
ETH MAC: DE:AD:BE:EF:FE:11, IPv4: 192.168.2.101
FULL_DUPLEX, 100Mbps
HTTP EthernetWebServer is @ IP : 192.168.2.101

HEAP DATA - Pre Create Arduino String  Max heap: 359852  Free heap: 298532  Used heap: 61320
.
HEAP DATA - Pre Send  Max heap: 359852  Free heap: 254632  Used heap: 105220

HEAP DATA - Post Send  Max heap: 359852  Free heap: 215352  Used heap: 142960

HEAP DATA - Post Send  Max heap: 359852  Free heap: 215340  Used heap: 142972
.
```


You can access the Async Advanced WebServers at the displayed server IP, e.g. `192.168.2.88`

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/Async_AdvancedWebServer_MemoryIssues_Send_CString.png">
</p>

---

#### 3. Async_AdvancedWebServer_SendChunked on ESP32S3_DEV with ESP32_S3_W5500

Following is debug terminal output when running example [Async_AdvancedWebServer_SendChunked](examples/Async_AdvancedWebServer_SendChunked) on `ESP32S3_DEV with LwIP W5500`, using ESP32 core `v2.0.0+`, to demo how to use `beginChunkedResponse()` to send large `html` in chunks

```cpp
Start Async_AdvancedWebServer_SendChunked on ESP32S3_DEV with ESP32_S3_W5500
AsyncWebServer_ESP32_SC_W5500 v1.8.1 for core v2.0.0+
[AWS] Default SPI pinout:
[AWS] SPI_HOST: 2
[AWS] MOSI: 11
[AWS] MISO: 13
[AWS] SCK: 12
[AWS] CS: 10
[AWS] INT: 4
[AWS] SPI Clock (MHz): 25
[AWS] =========================

ETH Started
ETH Connected
ETH MAC: DE:AD:BE:EF:BE:12, IPv4: 192.168.2.103
FULL_DUPLEX, 100Mbps
AsyncWebServer is @ IP : 192.168.2.103
.[AWS] Total length to send in chunks = 31259
[AWS] Bytes sent in chunk = 5620
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 2864
[AWS] Bytes sent in chunk = 2864
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 2775
[AWS] Bytes sent in chunk = 0
.[AWS] Total length to send in chunks = 31279
[AWS] Bytes sent in chunk = 5620
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 4300
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 2795
[AWS] Bytes sent in chunk = 0
```

You can access the Async Advanced WebServers @ the server IP

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/AsyncWebServer_SendChunked.png">
</p>


---

#### 4. AsyncWebServer_SendChunked on ESP32S3_DEV with ESP32_S3_W5500

Following is debug terminal output when running example [AsyncWebServer_SendChunked](examples/AsyncWebServer_SendChunked) on `ESP32S3_DEV with LwIP W5500`, using ESP32 core `v2.0.0+`, to demo how to use `beginChunkedResponse()` to send large `html` in chunks


```cpp
Start AsyncWebServer_SendChunked on ESP32S3_DEV with ESP32_S3_W5500
AsyncWebServer_ESP32_SC_W5500 v1.8.1 for core v2.0.0+
[AWS] Default SPI pinout:
[AWS] SPI_HOST: 2
[AWS] MOSI: 11
[AWS] MISO: 13
[AWS] SCK: 12
[AWS] CS: 10
[AWS] INT: 4
[AWS] SPI Clock (MHz): 25
[AWS] =========================

ETH Started
ETH Connected
ETH MAC: DE:AD:BE:EF:BE:12, IPv4: 192.168.2.103
FULL_DUPLEX, 100Mbps
AsyncWebServer is @ IP : 192.168.2.103
.[AWS] Total length to send in chunks = 47809
[AWS] Bytes sent in chunk = 5624
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 4300
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 2864
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 2864
[AWS] Bytes sent in chunk = 2864
[AWS] Bytes sent in chunk = 2864
[AWS] Bytes sent in chunk = 2864
[AWS] Bytes sent in chunk = 2864
[AWS] Bytes sent in chunk = 2864
[AWS] Bytes sent in chunk = 2129
[AWS] Bytes sent in chunk = 0
```


---

#### 5. Async_WebSocketsServer on ESP32S3_DEV with ESP32_S3_W5500

Following is debug terminal output when running example [Async_WebSocketsServer](examples/Async_WebSocketsServer) on `ESP32S3_DEV with LwIP W5500`, using ESP32 core `v2.0.0+`, to demo how to use `Async_WebSocketsServer` feature


```cpp
Starting Async_WebSocketsServer on ESP32S3_DEV with ESP32_S3_W5500
AsyncWebServer_ESP32_SC_W5500 v1.8.1 for core v2.0.0+
[AWS] Default SPI pinout:
[AWS] SPI_HOST: 2
[AWS] MOSI: 11
[AWS] MISO: 13
[AWS] SCK: 12
[AWS] CS: 10
[AWS] INT: 4
[AWS] SPI Clock (MHz): 25
[AWS] =========================

ETH Started
ETH Connected
ETH MAC: DE:AD:BE:EF:BE:08, IPv4: 192.168.2.88
FULL_DUPLEX, 100Mbps
ws[Server: /ws][ClientID: 1] WSClient connected
ws[Server: /ws][ClientID: 1] WSClient disconnected
ws[Server: /ws][ClientID: 2] WSClient connected
ws[Server: /ws][ClientID: 3] WSClient connected
```

---

#### 6. Async_HTTPBasicAuth on ESP32S3_DEV with ESP32_S3_W5500

Following is debug terminal output when running example [Async_HTTPBasicAuth](examples/Async_HTTPBasicAuth) on `ESP32S3_DEV with LwIP W5500`, using ESP32 core `v2.0.0+`, to demo how to use `Async_Auth` feature


```cpp
Start Async_HTTPBasicAuth on ESP32S3_DEV with ESP32_S3_W5500
AsyncWebServer_ESP32_SC_W5500 v1.8.1 for core v2.0.0+
[AWS] Default SPI pinout:
[AWS] SPI_HOST: 2
[AWS] MOSI: 11
[AWS] MISO: 13
[AWS] SCK: 12
[AWS] CS: 10
[AWS] INT: 4
[AWS] SPI Clock (MHz): 25
[AWS] =========================

ETH Started
ETH Connected
ETH MAC: DE:AD:BE:EF:BE:08, IPv4: 192.168.2.88
FULL_DUPLEX, 100Mbps
Async_HttpBasicAuth started @ IP : 192.168.2.88
Open http://192.168.2.88/ in your browser to see it working
Login using username = admin and password = esp32_w5500
```

---

#### 7. Async_AdvancedWebServer_SendChunked on ESP32S2_DEV with ESP32_S2_W5500

Following is debug terminal output when running example [Async_AdvancedWebServer_SendChunked](examples/Async_AdvancedWebServer_SendChunked) on `ESP32S2_DEV with LwIP W5500`, using ESP32 core `v2.0.0+`, to demo how to use `beginChunkedResponse()` to send large `html` in chunks. The `built-in MAC address` is now used instead of user-defined one.

```cpp
Start Async_AdvancedWebServer_SendChunked on ESP32S2_DEV with ESP32_S2_W5500
AsyncWebServer_ESP32_SC_W5500 v1.8.1 for core v2.0.0+
[AWS] Default SPI pinout:
[AWS] SPI_HOST: 2
[AWS] MOSI: 35
[AWS] MISO: 37
[AWS] SCK: 36
[AWS] CS: 34
[AWS] INT: 4
[AWS] SPI Clock (MHz): 25
[AWS] =========================

ETH Started
ETH Connected
ETH MAC: 7E:DF:A1:08:32:C9, IPv4: 192.168.2.133
FULL_DUPLEX, 100Mbps
AsyncWebServer is @ IP : 192.168.2.133
.[AWS] Total length to send in chunks = 31259
[AWS] Bytes sent in chunk = 5620
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 2791
[AWS] Bytes sent in chunk = 0
[AWS] Total length to send in chunks = 31279
[AWS] Bytes sent in chunk = 5620
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 2811
[AWS] Bytes sent in chunk = 0
```

You can access the Async Advanced WebServers @ the server IP

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/AsyncWebServer_SendChunked_ESP32_S2.png">
</p>

---

#### 8. Async_AdvancedWebServer_SendChunked on ESP32C3_DEV with ESP32_C3_W5500

Following is debug terminal output when running example [Async_AdvancedWebServer_SendChunked](examples/Async_AdvancedWebServer_SendChunked) on `ESP32C3_DEV with LwIP W5500`, using ESP32 core `v2.0.0+`, to demo how to use `beginChunkedResponse()` to send large `html` in chunks. The `built-in MAC address` is now used instead of user-defined one.

```cpp
Start Async_AdvancedWebServer_SendChunked on ESP32C3_DEV with ESP32_C3_W5500
AsyncWebServer_ESP32_SC_W5500 v1.8.1 for core v2.0.0+
[AWS] Default SPI pinout:
[AWS] SPI_HOST: 1
[AWS] MOSI: 6
[AWS] MISO: 5
[AWS] SCK: 4
[AWS] CS: 7
[AWS] INT: 10
[AWS] SPI Clock (MHz): 25
[AWS] =========================

ETH Started
ETH Connected
ETH MAC: 7C:DF:A1:BC:BC:53, IPv4: 192.168.2.135
FULL_DUPLEX, 100Mbps
AsyncWebServer is @ IP : 192.168.2.135
.[AWS] Total length to send in chunks = 31259
[AWS] Bytes sent in chunk = 5620
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 2791
[AWS] Bytes sent in chunk = 0
[AWS] Total length to send in chunks = 31279
[AWS] Bytes sent in chunk = 5620
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 1428
[AWS] Bytes sent in chunk = 2811
[AWS] Bytes sent in chunk = 0
```

You can access the Async Advanced WebServers @ the server IP

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/raw/main/pics/AsyncWebServer_SendChunked_ESP32_C3.png">
</p>



---
---

### Debug

Debug is enabled by default on Serial. Debug Level from 0 to 4. To disable, change the _ETHERNET_WEBSERVER_LOGLEVEL_ to 0

```cpp
// Use this to output debug msgs to Serial
#define DEBUG_ASYNC_WEBSERVER_PORT       Serial
// Use this to disable all output debug msgs
// Debug Level from 0 to 4
#define _ASYNC_WEBSERVER_LOGLEVEL_       0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of Arduino IDE, the Arduino `ESP32` core or depending libraries.

Sometimes, the library will only work if you update the `ESP32` core to the latest version because I'm always using the latest cores /libraries.

---

### Issues ###

Submit issues to: [AsyncWebServer_ESP32_SC_W5500 issues](https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/issues)

---
---

## TO DO

 1. Fix bug. Add enhancement
 2. Add support to more Ethernet shields, such as **DP83848, TLK110, IP101, RTL8201, DM9051, KSZ8041, KSZ8081, W6100, etc.**
 3. Add `LittleFS` support to use with new cores

---

## DONE

 1. Initial port to `ESP32_S3` boards using `LwIP W5500` Ethernet.
 2. Add more examples.
 3. Add debugging features.
 4. Add Table-of-Contents and Version String
 5. Display compiler `#warning` only when `DEBUG_LEVEL` is 3+
 6. Fix `AsyncWebSocket` bug
 7. Support using `CString` to save heap to send `very large data`. Check [request->send(200, textPlainStr, jsonChartDataCharStr); - Without using String Class - to save heap #8](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer/pull/8)
 8. Add examples [Async_AdvancedWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_SendChunked) and [AsyncWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncWebServer_SendChunked) to demo how to use `beginChunkedResponse()` to send large `html` in chunks
 9. Use `allman astyle` and add `utils`
10. Add `Async_WebSocketsServer`, `Async_HttpBasicAuth` and `MQTT` examples
11. Add support to **ESP32S2-based boards** using `LwIP W5500 Ethernet`
12. Add support to **ESP32C3-based boards** using `LwIP W5500 Ethernet`
13. Remove unused variable to avoid compiler warning and error


---
---


### Contributions and Thanks

1. Based on and modified from [Hristo Gochkov's ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer). Many thanks to [Hristo Gochkov](https://github.com/me-no-dev) for great [ESPAsyncWebServer Library](https://github.com/me-no-dev/ESPAsyncWebServer)


<table>
  <tr>
    <td align="center"><a href="https://github.com/me-no-dev"><img src="https://github.com/me-no-dev.png" width="100px;" alt="me-no-dev"/><br /><sub><b>⭐️⭐️ Hristo Gochkov</b></sub></a><br /></td>
  </tr> 
</table>

---

### Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [GPLv3](https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_W5500/blob/main/LICENSE)

---

## Copyright

- Copyright (c) 2016- Hristo Gochkov

- Copyright (c) 2022- Khoi Hoang



