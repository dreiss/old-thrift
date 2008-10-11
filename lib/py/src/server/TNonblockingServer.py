import threading
import Queue
import select
import struct

from thrift.Thrift import TProcessor
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

class Slave(threading.Thread):
    def __init__(self, queue, stop):
        threading.Thread.__init__(self)
        self.queue = queue
        self.stop = stop

    def run(self):
        while not self.stop.isSet():
            print "wait task"
            processor, iprot, oprot, callback = self.queue.get()
            print "proc task"
            processor.process(iprot, oprot)
            callback(oprot.getvalue())

WAIT_LEN = 0
WAIT_MESSAGE = 1
WAIT_PROCESS = 2
SEND_ANSWER = 3

def locked(func):
    def nested(self, *args, **kwargs):
        self.lock.acquire()
        try:
            func(self, *args, **kwargs)
        finally:
            self.lock.release()
    return nested

class Connection:
    def __init__(self, socket):
        self.socket = socket
        self.socket.setblocking(False)
        self.status = WAIT_LEN
        self.len = ''
        self.message = ''
        self.lock = threading.Lock()

    def read(self):
        if self.status == WAIT_LEN:
            self.len += self.socket.recv(4 - len(self.len))
            if len(self.len) == 4:
                self.len = struct.unpack('!i', self.len)
                self.message = ''
                self.status = WAIT_MESSAGE
        elif self.status == WAIT_MESSAGE:
            self.message += self.socket.recv(self.len - len(self.message))
            if len(self.message) == self.len:
                self.status = WAIT_PROCESS
        else:
            raise Exception

    def write(self):
        if self.status == SEND_ANSWER:
            sent = self.socket.send(self.message)
            if sent == len(self.message):
                self.status = WAIT_LEN
            else:
                self.message = self.message[sent:]
        else:
            raise Exception

    @locked
    def ready(self, buffer):
        self.status = WAIT_PROCESS
        self.len = ''
        self.message = buffer

    @locked
    def is_writeable(self):
        return self.status == SEND_ANSWER

    # it's not necessary, but...
    @locked
    def is_readable(self):
        return self.status in (WAIT_LEN, WAIT_MESSAGE)

    def fileno(self):
        return self.socket.fileno()

class TNonblockingServer:
    def __init__(self, processor, socket, protocol=None, threads=10):
        self.processor = processor
        self.socket = socket
        self.protocol = protocol or TBinaryProtocol.TBinaryProtocolFactory()
        self.threads = threads
        self.clients = []
        self.tasks = Queue.Queue()
        self.stop = threading.Event()

    def prepare(self):
        self.socket.listen()
        for _ in xrange(self.threads):
            Slave(self.tasks, self.stop).start()

    def _select(self):
        readable = [self.socket]
        writable = []
        for connection in self.clients:
            if connection.is_readable():
                readable.append(connection.fileno())
            if connection.is_writeable():
                writable.append(connection.fileno())
        return select.select(readable, writable, readable)
        
    select = _select

    def client_by_fileno(self, fileno):
        for connection in self.clients:
            if connection.fileno() == fileno:
                return connection 
        raise ValueError("Socket with fileno %d does'n exist" % fileno)

    def handle(self):
        r, w, x = self.select()
        for readable in r:
            print "select"
            if readable == self.socket.handle.fileno():
                print "accept"
                client = self.socket.accept()
                self.clients.append(Connection(client.handle))
            else:
                print "read"
                connection = self.client_by_fileno(readable)
                connection.read()
                if connection.status == WAIT_PROCESS:
                    itransport = TTransport.TMemoryBuffer(connection.message)
                    otransport = TTransport.TMemoryBuffer()
                    iprot = self.protocol.getProtocol(itransport)
                    oprot = self.protocol.getProtocol(otransport)
                    self.tasks.put([self.processor, iprot, oprot, connection.ready])
        for writeable in w:
            print "write"
            connection = self.client_by_fileno(writeable)
            connection.write()
        for oob in x:
            print "oob"
            for i, connection in enumerate(self.clients):
                if connection.fileno() == oob:
                    del self.clients[i]
        print "handle ok"

    def serve(self):
        self.prepare()
        while True:
            self.handle()
