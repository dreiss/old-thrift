import threading
import socket
import Queue
import select
import struct
import logging

from thrift.Thrift import TProcessor
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

class Slave(threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self.queue = queue

    def run(self):
        while True:
            try:
                processor, iprot, oprot, otrans, callback = self.queue.get()
                if processor is None:
                    break
                processor.process(iprot, oprot)
                callback(True, otrans.getvalue())
            except Exception, err:
                logging.exception("Exception while processing request")
                callback(False, '')

WAIT_LEN = 0
WAIT_MESSAGE = 1
WAIT_PROCESS = 2
SEND_ANSWER = 3
CLOSED = 4

def locked(func):
    def nested(self, *args, **kwargs):
        self.lock.acquire()
        try:
            return func(self, *args, **kwargs)
        finally:
            self.lock.release()
    return nested

def socket_exception(func):
    def read(self, *args, **kwargs):
        try:
            return func(self, *args, **kwargs)
        except socket.error:
            self.close()
    return read

class Connection:
    def __init__(self, socket, wake_up):
        self.socket = socket
        self.socket.setblocking(False)
        self.status = WAIT_LEN
        self.len = ''
        self.message = ''
        self.lock = threading.Lock()
        self.wake_up = wake_up

    def _read_len(self):
        read = self.socket.recv(4 - len(self.len))
        if len(read) == 0:
            self.close()
            return
        self.len += read
        if len(self.len) == 4:
            self.len, = struct.unpack('!i', self.len)
            if self.len < 0:
                logging.error("negative size, it seems client doesn't use FramedTransport")
                self.close()
                return
            self.message = ''
            self.status = WAIT_MESSAGE

    @socket_exception
    def read(self):
        if self.status == WAIT_LEN:
            self._read_len()
        elif self.status == WAIT_MESSAGE:
            read = self.socket.recv(self.len - len(self.message))
            if len(read) == 0:
                self.close()
                return
            self.message += read
            if len(self.message) == self.len:
                self.status = WAIT_PROCESS
        else:
            raise Exception

    @socket_exception
    def write(self):
        assert self.status == SEND_ANSWER
        sent = self.socket.send(self.message)
        if sent == len(self.message):
            self.status = WAIT_LEN
        else:
            self.message = self.message[sent:]

    @locked
    def ready(self, all_ok, buffer):
        if not all_ok:
            self.close()
            self.wake_up()
            return
        self.len = ''
        self.message = struct.pack('!i', len(buffer)) + buffer
        if len(buffer) == 0:
            # it was async request, do not write answer
            self.status = WAIT_LEN
        else:
            self.status = SEND_ANSWER
        self.wake_up()

    @locked
    def is_writeable(self):
        return self.status == SEND_ANSWER

    # it's not necessary, but...
    @locked
    def is_readable(self):
        return self.status in (WAIT_LEN, WAIT_MESSAGE)

    @locked
    def is_closed(self):
        return self.status == CLOSED

    def fileno(self):
        return self.socket.fileno()

    def close(self):
        self.status = CLOSED
        self.socket.close()

class TNonblockingServer:
    def __init__(self, processor, lsocket, protocol=None, threads=10):
        self.processor = processor
        self.socket = lsocket
        self.protocol = protocol or TBinaryProtocol.TBinaryProtocolFactory()
        self.threads = threads
        self.clients = {}
        self.tasks = Queue.Queue()
        self._read, self._write = socket.socketpair()

    def prepare(self):
        self.socket.listen()
        for _ in xrange(self.threads):
            thread = Slave(self.tasks)
            thread.setDaemon(True)
            thread.start()

    def wake_up(self):
        self._write.send('1')

    def _select(self):
        readable = [self.socket.handle.fileno(), self._read.fileno()]
        writable = []
        for i, connection in self.clients.items():
            if connection.is_readable():
                readable.append(connection.fileno())
            if connection.is_writeable():
                writable.append(connection.fileno())
            if connection.is_closed():
                del self.clients[i]
        return select.select(readable, writable, readable)
        
    select = _select

    def handle(self):
        r, w, x = self.select()
        for readable in r:
            if readable == self._read.fileno():
                self._read.recv(1024) # don't care i just need to clean readable flag
            elif readable == self.socket.handle.fileno():
                client = self.socket.accept().handle
                self.clients[client.fileno()] = Connection(client, self.wake_up)
            else:
                connection = self.clients[readable]
                connection.read()
                if connection.status == WAIT_PROCESS:
                    itransport = TTransport.TMemoryBuffer(connection.message)
                    otransport = TTransport.TMemoryBuffer()
                    iprot = self.protocol.getProtocol(itransport)
                    oprot = self.protocol.getProtocol(otransport)
                    self.tasks.put([self.processor, iprot, oprot, otransport, connection.ready])
        for writeable in w:
            self.clients[writeable].write()
        for oob in x:
            for i, connection in self.clients.items():
                if connection.fileno() == oob:
                    connection.close()
                    del self.clients[i]

    def close(self):
        for _ in xrange(self.threads):
            self.tasks.put([None, None, None, None, None])
        self.socket.close()
        
    def serve(self):
        self.prepare()
        while True:
            self.handle()
