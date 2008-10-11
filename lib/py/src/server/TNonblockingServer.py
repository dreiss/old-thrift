import threading
import Queue
import select

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
            processor, iprot, oprot = self.queue.get()
            print "proc task"
            processor.process(iprot, oprot)

class TNonblockingServer(TServer.TServer):
    def __init__(self, processor, socket, transport=None, protocol=None, threads=10):
        self.processor = processor
        self.socket = socket
        self.transport = transport or TTransport.TFramedTransportFactory()
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
        x = []
        for socket in self.clients + [self.socket]:
            if socket.handle:
                x.append(socket.handle.fileno())
        r, _, _ = select.select(x, [], [])
        return r
    select = _select

    def client_by_fileno(self, no):
        for socket in self.clients:
            if socket.handle.fileno() == no:
                return socket
        raise ValueError("Socket with fileno %d does'n exist" % no)

    def handle(self):
        for readable in self.select():
            print "select"
            if readable == self.socket.handle.fileno():
                print "accept"
                client = self.socket.accept()
                self.clients.append(client)
            else:
                print "read"
                client = self.client_by_fileno(readable)
                itransport = self.transport.getTransport(client)
                itransport.readFrame()
                otransport = self.transport.getTransport(client)
                iprot = self.protocol.getProtocol(itransport)
                oprot = self.protocol.getProtocol(otransport)
                self.tasks.put([self.processor, iprot, oprot])
        print "handle ok"

    def serve(self):
        self.prepare()
        while True:
            self.handle()
