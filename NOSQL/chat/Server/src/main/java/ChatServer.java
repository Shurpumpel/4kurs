import com.mongodb.MongoClient;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoDatabase;
import org.bson.Document;
import org.bson.types.ObjectId;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.net.ServerSocket;
import java.util.*;
import java.util.List;

public class ChatServer extends JFrame implements TCPConnectionListener{
    private JPanel panel1;
    private JPanel southpanel;
    private JButton startButton;
    private JTextField textField1;
    private JLabel insertPortLabel;
    private JButton deleteButton;
    private JTextField idToDeleteFromLogs;
    private JTextArea textArea1;
    private JPanel northPanel;

    private static int PORT = -1;

    private final Map<TCPConnection, String> connections = new HashMap<>();

    private final List<String> nicks = new ArrayList<>();

    private MongoClient mongoClient;

    private MongoDatabase database;

    private static MongoCollection<Document> collection;


    public static void main(String[] args) {
        new ChatServer();
    }

    public ChatServer() {

        //подключение к БД
        this.mongoClient = new MongoClient();
        this.database = mongoClient.getDatabase("ChatLog");
        if(!mongoClient.getDatabase("ChatLog").listCollectionNames()
                .into(new ArrayList<String>()).contains("logs"))
            database.createCollection("logs");
        collection = database.getCollection("logs");


        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(600, 500);
        add(panel1);
        Border border = BorderFactory.createLineBorder(Color.BLACK);
        textArea1.setBorder(BorderFactory.createCompoundBorder(border,
                BorderFactory.createEmptyBorder(10, 10, 10, 10)));
        textArea1.setText(this.getDataFromDB());
        setVisible(true);

        deleteButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String str = removeDocumentFromDB(idToDeleteFromLogs.getText());
                textArea1.setText(str);
            }
        });

        startButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try{
                    PORT = Integer.parseInt(textField1.getText());
                    startServer();
                }catch (NumberFormatException ignored){
                }
            }
        });

    }

    private void startServer(){
        try(ServerSocket serverSocket = new ServerSocket(PORT)) {
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
        Date date = new Date();
        Document document = new Document();
        document.put("date", date.toString());

        String[] arr = str.split(" ");

        if(str.contains("connectedUpdate")) {
            document.put("message", "client connected");
            document.put("TCPConnection", tcpConnection.toString());
            document.put("nickname", arr[1]);
            collection.insertOne(document);

            textArea1.setText(getDataFromDB());
            connections.put(tcpConnection, arr[1]);
            nicks.add(arr[1]);
            System.out.println("connected " + tcpConnection + "->"+arr[1]);
            String string = getUserList();
            sendToAllConnections("connectedUpdate "+string);
        } else {
            if(arr[1].contains("@")){
                for (String nick : nicks) {
                    if (arr[1].equals("@" + nick)) {
                        document.put("message", str);
                        document.put("TCPConnection", tcpConnection.toString());
                        document.put("nickname", arr[1]);
                        collection.insertOne(document);
                        textArea1.setText(getDataFromDB());

                        sendToPerson(connections.get(tcpConnection), nick, str);
                        return;
                    }
                }
            }

            document.put("message", str);
            document.put("TCPConnection", tcpConnection.toString());
            document.put("nickname", arr[1]);
            collection.insertOne(document);
            textArea1.setText(getDataFromDB());

            sendToAllConnections(str);
        }

    }




    @Override
    public synchronized void onDisconnect(TCPConnection tcpConnection) {
        String nick = connections.get(tcpConnection);
        nicks.remove(nick);
        connections.remove(tcpConnection);
        String string = getUserList();
        sendToAllConnections("disconnectedUpdate "+string);

        //Добавление в БД, в других случаях по аналогии
        Document document = new Document();
        document.put("date", new Date().toString());
        document.put("message", "disconnected");
        document.put("TCPConnection", tcpConnection.toString());
        document.put("nickname", nick);
        collection.insertOne(document);
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
        StringBuilder str = new StringBuilder();
        str.append("@").append(whoSend).append(" ");
        String[] arr = msg.split(" ");
        for (int i = 1; i < arr.length; i++) {
            str.append(arr[i]).append(" ");
        }
        for (HashMap.Entry<TCPConnection, String> pair : connections.entrySet()) {
            if(pair.getValue().equals(whoTake)||pair.getValue().equals(whoSend)){
                pair.getKey().sendString(str.toString());
            }
        }
    }

    private String getUserList(){
        StringBuilder users = new StringBuilder();
        for (String nick : nicks) {
            users.append(nick);
            users.append(";");
        }
        return users.toString();
    }

    //удаление документа из коллекции по _id
    private static String removeDocumentFromDB(String id){
        Document document = new Document();
        document.put("_id", new ObjectId(id));
        collection.deleteOne(document);
        return getDataFromDB();
    }

    //получение данных из БД
    private static String getDataFromDB(){
        StringBuilder sb = new StringBuilder();
        List<Document> data = collection.find().into(new ArrayList<Document>());

        for(Document doc : data){
            sb.append(doc);
            sb.append("\n");
        }
        return sb.toString();
    }

}
