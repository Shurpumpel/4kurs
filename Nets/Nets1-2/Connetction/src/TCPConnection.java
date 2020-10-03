import java.io.*;
import java.net.Socket;
import java.util.HashSet;
import java.util.Set;

public class TCPConnection {
    private final Socket socket;
    private final Thread clientThread;
    private final BufferedReader in;
    private final BufferedWriter out;
    private final TCPConnectionListener eventListener;

    public TCPConnection(TCPConnectionListener eventListener,
                         String ipAddress, int port) throws IOException {
        this(new Socket(ipAddress, port), eventListener);
    }

    public TCPConnection(Socket socket, TCPConnectionListener eventListener)
            throws IOException {
        this.eventListener = eventListener;
        this.socket = socket;
        this.in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        this.out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));

        this.clientThread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    eventListener.onConnectionReady(TCPConnection.this);
                    while (!clientThread.isInterrupted()) {
                        eventListener.onReceiveString(TCPConnection.this, in.readLine());
                    }
                } catch (IOException e) {
                    eventListener.onException(TCPConnection.this, e);
                }finally {
                    eventListener.onDisconnect(TCPConnection.this);
                }
            }
        });
        clientThread.start();
    }

    public synchronized void sendString(String str){
        try {
            out.write(str+"\r\n");
            out.flush();
        } catch (IOException e) {
            eventListener.onException(TCPConnection.this, e);
            disconnect();
        }
    }

    public synchronized void disconnect(){
        clientThread.interrupt();
        try {
            socket.close();
        } catch (IOException e) {
            eventListener.onException(TCPConnection.this, e);
        }
    }

    @Override
    public String toString() {
        return "TCPConnection: "+ socket.getInetAddress() + ":" + socket.getPort();
    }
}
