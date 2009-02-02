// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#include <cstdlib>
#include <sstream>

#include "THttpClient.h"
#include "TSocket.h"

namespace apache { namespace thrift { namespace transport {

using namespace std;

/**
 * Http client implementation.
 *
 * @author Mark Slee <mcslee@facebook.com> and Jeremie BORDIER <jeremie.bordier@gmail.com>
 */

THttpClient::THttpClient(boost::shared_ptr<TTransport> transport, std::string host, std::string path) :
  THttpTransport(transport), host_(host), path_(path) {
}

THttpClient::~THttpClient() {}

void THttpClient::parseHeader(char* header) {
  char* colon = strchr(header, ':');
  if (colon == NULL) {
    return;
  }
  uint32_t sz = colon - header;
  char* value = colon+1;

  if (strncmp(header, "Transfer-Encoding", sz) == 0) {
    if (strstr(value, "chunked") != NULL) {
      chunked_ = true;
    }
  } else if (strncmp(header, "Content-Length", sz) == 0) {
    chunked_ = false;
    contentLength_ = atoi(value);
  }
}

bool THttpClient::parseStatusLine(char* status) {
  char* http = status;

  char* code = strchr(http, ' ');
  if (code == NULL) {
    throw TTransportException(string("Bad Status: ") + status);
  }

  *code = '\0';
  while (*(code++) == ' ');

  char* msg = strchr(code, ' ');
  if (msg == NULL) {
    throw TTransportException(string("Bad Status: ") + status);
  }
  *msg = '\0';

  if (strcmp(code, "200") == 0) {
    // HTTP 200 = OK, we got the response
    return true;
  } else if (strcmp(code, "100") == 0) {
    // HTTP 100 = continue, just keep reading
    return false;
  } else {
    throw TTransportException(string("Bad Status: ") + status);
  }
}

void THttpClient::flush() {
  // Fetch the contents of the write buffer
  uint8_t* buf;
  uint32_t len;
  writeBuffer_.getBuffer(&buf, &len);

  // Construct the HTTP header
  std::ostringstream h;
  h <<
    "POST " << path_ << " HTTP/1.1" << CRLF <<
    "Host: " << host_ << CRLF <<
    "Content-Type: application/x-thrift" << CRLF <<
    "Content-Length: " << len << CRLF <<
    "Accept: application/x-thrift" << CRLF <<
    "User-Agent: C++/THttpClient" << CRLF <<
    CRLF;
  string header = h.str();

  // Write the header, then the data, then flush
  transport_->write((const uint8_t*)header.c_str(), header.size());
  transport_->write(buf, len);
  transport_->flush();
//  printf(">>>\n%s%s\n", header.c_str(), buf);

  // Reset the buffer and header variables
  writeBuffer_.resetBuffer();
  readHeaders_ = true;
}

}}} // apache::thrift::transport
