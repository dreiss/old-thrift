#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <protocol/TBinaryProtocol.h>
#include <transport/TTransportUtils.h>
#include <transport/TSocket.h>

#include <boost/shared_ptr.hpp>
#include "ThriftTest.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

using namespace boost;
using namespace std;
using namespace facebook::thrift;
using namespace facebook::thrift::protocol;
using namespace facebook::thrift::transport;
using namespace thrift::test;

//extern uint32_t g_socket_syscalls;

void test_testI32(ThriftTestClient &testClient);
void test_testAsync(ThriftTestClient &testClient);
// Current time, microseconds since the epoch
uint64_t now()
{
  long long ret;
  struct timeval tv;

  gettimeofday(&tv, NULL);
  ret = tv.tv_sec;
  ret = ret*1000*1000 + tv.tv_usec;
  return ret;
}

int main(int argc, char** argv) {
  string host = "localhost";
  int port = 9090;
  int numTests = 1;
  bool framed = true;

  for (int i = 0; i < argc; ++i) {
    if (strcmp(argv[i], "-h") == 0) {
      char* pch = strtok(argv[++i], ":");
      if (pch != NULL) {
        host = string(pch);
      }
      pch = strtok(NULL, ":");
      if (pch != NULL) {
        port = atoi(pch);
      }
    } else if (strcmp(argv[i], "-n") == 0) {
      numTests = atoi(argv[++i]);
    } else if (strcmp(argv[i], "-f") == 0) {
      framed = true;
    }
  }


  shared_ptr<TTransport> transport;

  shared_ptr<TSocket> socket(new TSocket(host, port));

  if (framed) {
    shared_ptr<TFramedTransport> framedSocket(new TFramedTransport(socket));
    transport = framedSocket;
  } else {
    shared_ptr<TBufferedTransport> bufferedSocket(new TBufferedTransport(socket));
    transport = bufferedSocket;
  }

  shared_ptr<TBinaryProtocol> protocol(new TBinaryProtocol(transport));
  ThriftTestClient testClient(protocol);

  uint64_t time_min = 0;
  uint64_t time_max = 0;
  uint64_t time_tot = 0;

  int test = 0;
  for (test = 0; test < numTests; ++test) {

    try {
      transport->open();
    } catch (TTransportException& ttx) {
      printf("Connect failed: %s\n", ttx.what());
      continue;
    }

    /**
     * CONNECT TEST
     */
    printf("Test #%d, connect %s:%d\n", test+1, host.c_str(), port);

    uint64_t start = now();

    test_testAsync(testClient);
    uint64_t ti32_start = now();
    test_testI32(testClient);
    if (now() - ti32_start > 200*1000)
        printf("i32 is too long...");

    uint64_t stop = now();
    uint64_t tot = stop-start;

    printf("Total time: %"PRIu64" us\n", stop-start);

    time_tot += tot;
    if (time_min == 0 || tot < time_min) {
      time_min = tot;
    }
    if (tot > time_max) {
      time_max = tot;
    }

    transport->close();
  }

  //  printf("\nSocket syscalls: %u", g_socket_syscalls);
  printf("\nAll tests done.\n");

  uint64_t time_avg = time_tot / numTests;

  printf("Min time: %"PRIu64" us\n", time_min);
  printf("Max time: %"PRIu64" us\n", time_max);
  printf("Avg time: %"PRIu64" us\n", time_avg);

  return 0;
}

void test_testI32(ThriftTestClient &testClient)
{
    printf("test testI32(-1)");
    int32_t i32 = testClient.testI32(-1);
    printf(" = %d\n", i32);
}

/* test async void */
void test_testAsync(ThriftTestClient &testClient)
{
    printf("testClient.testAsync(3) =>");
    uint64_t startAsync = now();
    testClient.testAsync(3);
    uint64_t elapsed = now() - startAsync;
    if (elapsed > 200 * 1000) { // 0.2 seconds
        printf("  FAILURE - took %.2f ms\n", (double)elapsed/1000.0);
    } else {
        printf("  success - took %.2f ms\n", (double)elapsed/1000.0);
    }
}
