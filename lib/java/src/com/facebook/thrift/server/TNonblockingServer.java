
package com.facebook.thrift.server;

import com.facebook.thrift.TException;
import com.facebook.thrift.TProcessor;
import com.facebook.thrift.TProcessorFactory;
import com.facebook.thrift.protocol.TProtocol;
import com.facebook.thrift.protocol.TProtocolFactory;
import com.facebook.thrift.protocol.TBinaryProtocol;
import com.facebook.thrift.transport.TNonblockingServerTransport;
import com.facebook.thrift.transport.TIOStreamTransport;
import com.facebook.thrift.transport.TTransport;
import com.facebook.thrift.transport.TFramedTransport;
import com.facebook.thrift.transport.TNonblockingTransport;
import com.facebook.thrift.transport.TTransportException;
import com.facebook.thrift.transport.TTransportFactory;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.Iterator;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;

import java.io.IOException;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.nio.channels.Selector;
import java.nio.channels.SelectionKey;
import java.nio.channels.spi.SelectorProvider;
import java.nio.channels.CancelledKeyException;
import java.nio.channels.ClosedChannelException;

/**
 * A nonblocking TServer implementation. This allows for fairness amongst all
 * connected clients in terms of invocations.
 *
 * This server is inherently single-threaded. If you want a limited thread pool
 * coupled with invocation-fairness, see THsHaServer. 
 *
 * To use this server, you MUST use a TFramedTransport at the outermost 
 * transport, otherwise this server will be unable to determine when a whole 
 * method call has been read off the wire. Clients must also use TFramedTransport.
 */
public class TNonblockingServer extends TServer {
  // Flag for stopping the server
  private volatile boolean stopped_;
  
  private SelectThread selectThread;

  /**
   * Create server with given processor and server transport, using 
   * TBinaryProtocol for the protocol, TFramedTransport.Factory on both input
   * and output transports. A TProcessorFactory will be created that always 
   * returns the specified processor.
   */
  public TNonblockingServer(TProcessor processor,
                           TNonblockingServerTransport serverTransport) {
    this(new TProcessorFactory(processor), serverTransport);
  }

  /**
   * Create server with specified processor factory and server transport. 
   * TBinaryProtocol is assumed. TFramedTransport.Factory is used on both input
   * and output transports.
   */
  public TNonblockingServer(TProcessorFactory processorFactory,
                           TNonblockingServerTransport serverTransport) {
    this(processorFactory, serverTransport,
         new TFramedTransport.Factory(), new TFramedTransport.Factory(),
         new TBinaryProtocol.Factory(), new TBinaryProtocol.Factory());    
  }
  
  public TNonblockingServer(TProcessor processor,
                           TNonblockingServerTransport serverTransport,
                           TProtocolFactory protocolFactory) {
    this(processor, serverTransport,
         new TFramedTransport.Factory(), new TFramedTransport.Factory(),
         protocolFactory, protocolFactory);
  }
  
  public TNonblockingServer(TProcessor processor,
                           TNonblockingServerTransport serverTransport,
                           TTransportFactory transportFactory,
                           TProtocolFactory protocolFactory) {
    this(processor, serverTransport,
         transportFactory, transportFactory,
         protocolFactory, protocolFactory);
  }
  
  public TNonblockingServer(TProcessorFactory processorFactory,
                           TNonblockingServerTransport serverTransport,
                           TTransportFactory transportFactory,
                           TProtocolFactory protocolFactory) {
    this(processorFactory, serverTransport,
         transportFactory, transportFactory,
         protocolFactory, protocolFactory);
  }
  
  public TNonblockingServer(TProcessor processor,
                           TNonblockingServerTransport serverTransport,
                           TTransportFactory inputTransportFactory,
                           TTransportFactory outputTransportFactory,
                           TProtocolFactory inputProtocolFactory,
                           TProtocolFactory outputProtocolFactory) {
    this(new TProcessorFactory(processor), serverTransport,
         inputTransportFactory, outputTransportFactory,
         inputProtocolFactory, outputProtocolFactory);
  }
  
  public TNonblockingServer(TProcessorFactory processorFactory,
    TNonblockingServerTransport serverTransport, TTransportFactory inputTransportFactory,
    TTransportFactory outputTransportFactory, TProtocolFactory inputProtocolFactory,
    TProtocolFactory outputProtocolFactory) 
  {      
    super(processorFactory, serverTransport,
      inputTransportFactory, outputTransportFactory,
      inputProtocolFactory, outputProtocolFactory);
  }

  /**
   * Begin accepting connections and processing invocations.
   */
  public void serve() {
    // start listening, or exit
    if (!startListening()) {
      return;
    }

    // start the selector, or exit
    if (!startSelectorThread()) {
      return;
    }

    // this will block while we serve
    joinSelector();
    
    // do a little cleanup
    stopListening();
  }

  protected boolean startListening() {
    try {
      serverTransport_.listen();
      return true;
    } catch (TTransportException ttx) {
      // TODO: log this better.
      ttx.printStackTrace();
      return false;
    }
  }

  protected void stopListening() {
    serverTransport_.close();
  }

  protected boolean startSelectorThread() {
    // start the selector
    try {
      selectThread = new SelectThread((TNonblockingServerTransport)serverTransport_);
      selectThread.start();
      return true;
    } catch (IOException e) {
      // need to decide what to do here. probably shut other stuff down and 
      // abort.
      // TODO: log this exception
      return false;
    }    
  }

  protected void joinSelector() {
    // wait until the selector thread exits
    try {
      selectThread.join();
    } catch (InterruptedException e) {
      // hm, what should we do here? 
      // TODO: decide what to do.
    }
  }

  /**
   * Stop serving and shut everything down. 
   */
  public void stop() {
    stopped_ = true;
    serverTransport_.interrupt();
    selectThread.wakeupSelector();
  }

  /**
   * Perform an invocation. This method could behave several different ways
   * - invoke immediately inline, queue for separate execution, etc.
   */
  protected void invoke(TTransport inTrans, TTransport outTrans) {
    TProtocol inProt = inputProtocolFactory_.getProtocol(inTrans);
    TProtocol outProt = outputProtocolFactory_.getProtocol(outTrans);

    try {
      processorFactory_.getProcessor(inTrans).process(inProt, outProt);
    } catch (TException te) {
      System.out.println("Exception while invoking! " + te);
      te.printStackTrace();
    }
  }

  /**
   * The thread that will be doing all the selecting, managing new connections
   * and those that still need to be read.
   */
  protected class SelectThread extends Thread {
    
    private final TNonblockingServerTransport serverTransport;
    private final Selector selector;
    
    // Mapping of selection keys to FrameBuffers so we can find the right
    // FrameBuffer to interact with when we get some action on a selection key.
    private final HashMap<SelectionKey, FrameBuffer> keysToBuffers = 
      new HashMap<SelectionKey, FrameBuffer>();
        
    // List of FrameBuffers that want to change their selection interests.
    private final Set<FrameBuffer> selectInterestChanges = 
      new HashSet<FrameBuffer>();
    
    /**
     * Set up the SelectorThread.
     */
    public SelectThread(final TNonblockingServerTransport serverTransport) 
    throws IOException {
      this.serverTransport = serverTransport;
      this.selector = SelectorProvider.provider().openSelector();
      serverTransport.registerSelector(selector);
    }
        
    /** 
     * The work loop. Handles both selecting (all IO operations) and managing
     * the selection preferences of all existing connections.
     */
    public void run() {
      while (!stopped_) {
        select();
        processInterestChanges();
      }
    }
    
    /**
     * If the selector is blocked, wake it up. 
     */
    public void wakeupSelector() {
      selector.wakeup();
    }
    
    /**
     * Select and process IO events appropriately:
     * If there are connections to be accepted, accept them.
     * If there are existing connections with data waiting to be read, read it,
     * bufferring until a whole frame has been read.
     * If there are any pending responses, buffer them until their target client
     * is available, and then send the data.
     */
    private void select() {
      try {
        // wait for io events.
        selector.select();
        
        // process the io events we received
        Iterator<SelectionKey> selectedKeys = selector.selectedKeys().iterator();
        while (selectedKeys.hasNext()) {
          SelectionKey key = selectedKeys.next();
          selectedKeys.remove();
          
          // skip if not valid
          if (!key.isValid()) {
            cleanupSelectionkey(key);
            continue;
          }

          // if the key is marked Accept, then it has to be the server 
          // transport.
          if (key.isAcceptable()) {
            handleAccept();
          } else if (key.isReadable()) {
            // deal with reads
            handleRead(key);
          } else if (key.isWritable()) {
            // deal with writes
            handleWrite(key);
          }            
        }
      } catch (IOException e) {
        System.out.println("Got an IOException while selecting: " + e);
        // TODO: handle this appropriately
      }
    }
    
    /**
     * Check to see if there are any FrameBuffers that have switched their 
     * interest type from read to write or vice versa.
     */
    private void processInterestChanges() {
      synchronized (selectInterestChanges) {
        for (FrameBuffer fb : selectInterestChanges) {
          fb.changeSelectInterests();
        }
        selectInterestChanges.clear();
      }
    }
    
    /**
     * Accept a new connection.
     */
    private void handleAccept() throws IOException {
      try {
        // accept the connection
        TNonblockingTransport client = (TNonblockingTransport)serverTransport.accept();
        SelectionKey clientKey = client.registerSelector(selector, SelectionKey.OP_READ);

        // add this key to the map
        FrameBuffer frameBuffer = new FrameBuffer(client, clientKey);
        keysToBuffers.put(clientKey, frameBuffer);
      } catch (TTransportException tte) {
        // something went wrong accepting. 
        // we should just close our side of the connection and move on.
        // TODO: log this error
        System.out.println("Exception trying to accept! " + tte);
        tte.printStackTrace();
      }      
    }
    
    /**
     * Do the work required to read from a readable client. If the frame is 
     * fully read, then invoke the method call.
     */
    private void handleRead(SelectionKey key) {
      FrameBuffer buffer = keysToBuffers.get(key);
      if (buffer.read()) {
        // if the buffer's frame read is complete, invoke the method.
        if (buffer.isFrameFullyRead()) {
          invoke(buffer.getInputTransport(), buffer.getOutputTransport());
        }
      } else {
        cleanupSelectionkey(key);
      }
    }
    
    /** 
     * Let a writable client get written, if there's data to be written.
     */
    private void handleWrite(SelectionKey key) {
      FrameBuffer buffer = keysToBuffers.get(key);
      if (!buffer.write()) {
        cleanupSelectionkey(key);
      }
    }

    /**
     * Do connection-close cleanup on a given SelectionKey.
     */
    private void cleanupSelectionkey(SelectionKey key) {
      // remove the records from the two maps
      FrameBuffer buffer = keysToBuffers.remove(key);
      // close the buffer
      buffer.close();
      // cancel the selection key
      key.cancel();
    }

    /**
     * Class that implements a sort of state machine around the interaction with
     * a client and an invoker. It manages reading the frame size and frame data,
     * getting it handed off as wrapped transports, and then the writing of
     * response data back to the client. In the process it manages flipping the
     * read and write bits on the selection key for its client.
     */
    protected class FrameBuffer {
      //
      // Possible states for the FrameBuffer state machine.
      //
      // in the midst of reading the frame size off the wire
      private static final int READING_FRAME_SIZE = 1;
      // reading the actual frame data now, but not all the way done yet
      private static final int READING_FRAME = 2;
      // completely read the frame, so an invocation can now happen
      private static final int READ_FRAME_COMPLETE = 3;
      // waiting to get switched to listening for write events
      private static final int AWAITING_REGISTER_WRITE = 4;
      // started writing response data, not fully complete yet
      private static final int WRITING = 6;

      //
      // Instance variables
      //

      // the actual transport hooked up to the client.
      private final TNonblockingTransport trans;
      
      // the SelectionKey that corresponds to our transport
      private final SelectionKey selectionKey;
      
      // where in the process of reading/writing are we?
      private int state = READING_FRAME_SIZE;

      // the ByteBuffer we'll be using to write and read, depending on the state
      private ByteBuffer buffer;

      public FrameBuffer( final TNonblockingTransport trans, 
                          final SelectionKey selectionKey) {
        this.trans = trans;
        this.selectionKey = selectionKey;
        enterReadingFrameSizeState();
      }

      /**
       * Give this FrameBuffer a chance to read. The selector loop should have
       * received a read event for this FrameBuffer.
       * 
       * @return true if the connection should live on, false if it should be 
       * closed
       */
      public boolean read() {
        if (state == READING_FRAME_SIZE) {
          // try to read the frame size completely
          if (!internalRead()) {
            return false;
          }


          // if the frame size has not yet been read completely, we must
          // wait for the connection to become readable again to complete
          // this state.
          if (buffer.remaining() > 0) {
            return true;
          }

          // pull out the frame size as an integer.
          int frameSize = buffer.getInt(0);
          if (frameSize <= 0) {
            System.out.println("Read an invalid frame size of " + frameSize 
                               + ". Are you using TFramedTransport on the client side?");
            return false;
          }
          // reallocate the readbuffer as a frame-sized buffer
          buffer = ByteBuffer.allocate(frameSize + 4);
          // put the frame size at the head of the buffer
          buffer.putInt(frameSize);

          state = READING_FRAME;
        }

        // it is possible to fall through from the READING_FRAME_SIZE section 
        // to READING_FRAME if there's already some frame data available once
        // READING_FRAME_SIZE is complete.

        if (state == READING_FRAME) {
          if (!internalRead()) {
            return false;
          }

          // since we're already in the select loop here for sure, we can just 
          // modify our selection key directly. 
          if (buffer.remaining() == 0) {
            // get rid of the read select interests
            selectionKey.interestOps(0);
            state = READ_FRAME_COMPLETE;
          } 

          return true;
        }

        // if we fall through to this point, then the state must be invalid.
        System.out.println("Read was called but state is invalid (" + state + ")");
        return false;
      }

      /**
       * Give this FrameBuffer a chance to write its output to the final client.
       */
      public boolean write() {
        if (state == WRITING) {
          try {
            if (trans.write(buffer) < 0) {
              return false;
            }
          } catch (IOException e) {
            // TODO: log this error
            System.out.println("Got an IOException (1)! " + e);
            e.printStackTrace();
            return false;
          }

          // we're done writing. now we need to switch back to reading.
          if (buffer.remaining() == 0) {
            // we can set our interest directly without using the queue because 
            // we're in the select thread.
            selectionKey.interestOps(SelectionKey.OP_READ);
            // get ready for another go-around
            enterReadingFrameSizeState();
          }
          return true;
        } else {
          System.out.println("Write was called, but state is invalid (" 
            + state + ")");
          return false;
        }
      }

      /**
       * Give this FrameBuffer a chance to set its interest to write, once data
       * has come in.
       */
      public void changeSelectInterests() {
        if (state == AWAITING_REGISTER_WRITE) {
          // set the OP_WRITE interest
          selectionKey.interestOps(SelectionKey.OP_WRITE);
          state = WRITING;          
        } else {
          System.out.println(
            "changeSelectInterest was called, but state is invalid (" 
            + state + ")");
        }
      }

      /**
       * Reset the state of this frame buffer to READING_FRAME_SIZE, also
       * resetting the internal byte buffer.
       */
      private void enterReadingFrameSizeState() {
        state = READING_FRAME_SIZE;
        buffer = ByteBuffer.allocate(4);
      }

      /**
       * Shut the connection down.
       */
      public void close() {
        trans.close();
      }

      /**
       * Check if this FrameBuffer has a full frame read.
       */
      public boolean isFrameFullyRead() {
        return state == READ_FRAME_COMPLETE;
      }

      /** 
       * Once isComplete is true, this returns a TTransport backed by the frame 
       * data. 
       */
      public TTransport getInputTransport() {
        return inputTransportFactory_.getTransport(new TIOStreamTransport(
          new ByteArrayInputStream(buffer.array())));
      }

      /**
       * Get the transport that should be used by the invoker for responding.
       */
      public TTransport getOutputTransport() {
        return outputTransportFactory_.getTransport(new NotifyingMemoryTransport());
      }

      /**
       * Put this framebuffer in the list of framebuffers that wants to change
       * their select interest. 
       */
      private void requestInterestChange() {
        synchronized (selectInterestChanges) {
          selectInterestChanges.add(FrameBuffer.this);
        }
        // wakeup the selector, if it's currently blocked.
        selector.wakeup();
      }

      /**
       * Perform a read into buffer.
       *
       * @return true if the read succeeded, false if there was an error or the
       * connection closed.
       */
      private boolean internalRead() {
        try {
          if (trans.read(buffer) < 0) {
            return false;
          }
          return true;
        } catch (IOException e) {
          // TODO: log this better.
          System.out.println("Got an IOException! (2) " + e);
          e.printStackTrace();
          return false;
        }
      }

      /**
       * This internal class is basically a memory buffer transport whose flush 
       * method also puts the FrameBuffer into the writable state.
       */ 
      private class NotifyingMemoryTransport extends TIOStreamTransport {

        public NotifyingMemoryTransport() {
          outputStream_ = new ByteArrayOutputStream();
        }

        /**
         * When flush is called, we know the invoking process is done writing.
         * We use the call of flush as a signal to indicate we're ready to be
         * put back on the write queue.
         */ 
        @Override
        public void flush() throws TTransportException {
          super.flush();

          // capture the data we want to write as a byte array.
          buffer = ByteBuffer.wrap(((ByteArrayOutputStream)outputStream_).toByteArray());
                    
          // set state that we're waiting to be switched to write. we do this 
          // asynchronously through requestInterestChange() because there is a
          // possibility that we're not in the main thread, and thus currently 
          // blocked in select(). (this functionality is in place for the sake of
          // the HsHa server.)
          state = AWAITING_REGISTER_WRITE;
          requestInterestChange();
        }
      } // NotifyingMemoryTransport
      
    } // FrameBuffer
    
  } // SelectorThread
}