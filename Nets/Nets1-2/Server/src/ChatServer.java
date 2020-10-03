
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.util.*;

public class ChatServer implements TCPConnectionListener{
    private int PORT = -1;

    private Map<TCPConnection, String> connections = new HashMap<>();

    private List<String> nicks = new ArrayList<>();

    public static void main(String[] args)  {
        new ChatServer();
    }


    private ChatServer(){
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        while (this.PORT < 0) {
            try
            {
                System.out.println("Insert port");
                this.PORT = Integer.parseInt(reader.readLine());
            }
            catch (IOException e) {
                System.out.println("Wrong number");
                continue;
            }
        }

        try(ServerSocket serverSocket = new ServerSocket(this.PORT)) {
            System.out.println("Server running...");
            while (true){
                try {
                    TCPConnection connection = new TCPConnection(serverSocket.accept(), this);
                }catch (IOException e){
                    System.out.println("TCPConnection exception: " + e);
                }
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public synchronized void onConnectionReady(TCPConnection tcpConnection) {
        //System.out.println("Client connected " + tcpConnection);
    }

    @Override
    public synchronized void onReceiveString(TCPConnection tcpConnection, String str) {
        if(str.contains("connectedUpdate")) {
            String arr[] = str.split(" ");
            connections.put(tcpConnection, arr[1]);
            nicks.add(arr[1]);
            System.out.println("Client connected " + tcpConnection + "->"+arr[1]);
            String string = getUserList();
            sendToAllConnections("connectedUpdate "+string);
        } else {
            String arr[] = str.split(" ");
            if(arr[1].contains("@")){
                for (int i = 0; i < nicks.size(); i++) {
                    if(arr[1].equals("@" + nicks.get(i))){
                        sendToPerson(connections.get(tcpConnection), nicks.get(i), str);
                        return;
                    }
                }
            }
            sendToAllConnections(str);
        }
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ChatServer that = (ChatServer) o;
        return PORT == that.PORT &&
                Objects.equals(connections, that.connections) &&
                Objects.equals(nicks, that.nicks);
    }

    @Override
    public int hashCode() {
        return Objects.hash(PORT, connections, nicks);
    }

    @Override
    public synchronized void onDisconnect(TCPConnection tcpConnection) {
        nicks.remove(connections.get(tcpConnection));
        connections.remove(tcpConnection);
        String string = getUserList();
        sendToAllConnections("disconnectedUpdate "+string);
        System.out.println("Client disconnected " + tcpConnection);
    }

    @Override
    public synchronized void onException(TCPConnection tcpConnection, Exception e) {
        System.out.println("TCPConnection: " + e);
    }

    public void sendToAllConnections(String str){
        for (HashMap.Entry<TCPConnection, String> pair : connections.entrySet()) {
            pair.getKey().sendString(str);
        }
    }

    public void sendToPerson(String whoSend, String whoTake, String msg){
        String str = "";
        str+="@"+whoSend+" ";
        String arr[] = msg.split(" ");
        for (int i = 1; i < arr.length; i++) {
            str+=arr[i]+" ";
        }
        for (HashMap.Entry<TCPConnection, String> pair : connections.entrySet()) {
            if(pair.getValue().equals(whoTake)||pair.getValue().equals(whoSend)){
                pair.getKey().sendString(str);
            }
        }
    }

    private String getUserList(){
        StringBuilder users = new StringBuilder();
        int count = nicks.size();
        for (int i = 0; i < count; i++) {
            users.append(nicks.get(i));
            users.append(";");
        }
        return users.toString();
    }



}
