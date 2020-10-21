import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class ChatClient extends JFrame implements TCPConnectionListener {
    public String nickName;

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new ChatClient();
            }
        });
    }

    private TCPConnection connection;


    private final JTextField nickNameField = new JTextField();
    private final JTextField input = new JTextField();
    private final JTextArea log = new JTextArea();

    private final DefaultListModel<String> listModel = new DefaultListModel<>();
    private final JList<String> usersList = new JList<>(listModel);
    private final JScrollPane spUsers = new JScrollPane(usersList);

    private ChatClient(){
        setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        int width = 700;
        int height = 500;
        setSize(width, height);
        setLocationRelativeTo(null);
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                Object[] options = { "Да", "Нет!" };
                int n = JOptionPane
                        .showOptionDialog(e.getWindow(), "Закрыть окно?",
                                "Подтверждение", JOptionPane.YES_NO_OPTION,
                                JOptionPane.QUESTION_MESSAGE, null, options,
                                options[0]);
                if (n == JOptionPane.NO_OPTION) {
                }else{
                    setVisible(false);
                    onDisconnect(connection);
                    System.exit(0);
                }
            }
        });


        log.setEditable(false);
        log.setLineWrap(true);
        JScrollPane spLog = new JScrollPane(log);
        add(spLog, BorderLayout.CENTER);

        String string = JOptionPane.showInputDialog(this, "Введите ник:",
                JOptionPane.WARNING_MESSAGE);
        if(string!=null && string.length()>0){
            this.nickName = string;
        }else System.exit(0);
        nickNameField.setText(this.nickName);
        nickNameField.setEditable(false);

        string = JOptionPane.showInputDialog(this, "Введите IP:",
                JOptionPane.WARNING_MESSAGE);
        String IP_Address = "";
        if(string!=null && string.length()>0){
            IP_Address = string;
        }else System.exit(0);

        string = JOptionPane.showInputDialog(this, "Введите PORT:",
                JOptionPane.WARNING_MESSAGE);
        int PORT = -1;
        if(string!=null && string.length()>0){
            PORT = Integer.parseInt(string);
        }else System.exit(0);

        input.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String str = input.getText();
                if(str.equals(""))return;
                input.setText(null);
                connection.sendString(nickNameField.getText() + ": "+str);
            }
        });

        add(input, BorderLayout.SOUTH);
        add(nickNameField, BorderLayout.NORTH);



        try {
            connection = new TCPConnection(this, IP_Address, PORT);
        } catch (IOException e) {
            printMessage("Client exception: " + e);
        }

        JPanel panel = new JPanel();
        panel.add(usersList);
        add(panel, BorderLayout.EAST);

        setResizable(false);
        setVisible(true);
    }


    @Override
    public void onConnectionReady(TCPConnection tcpConnection) {
        connection.sendString("connectedUpdate "+this.nickName);
    }

    @Override
    public void onReceiveString(TCPConnection tcpConnection, String str) {
        if(str.contains("disconnectedUpdate") || str.contains("connectedUpdate")){
            String[] arr = str.split(" ");
            updateUserList(arr[1]);
            this.repaint();
        }else {
            printMessage(str);
        }
    }

    @Override
    public void onDisconnect(TCPConnection tcpConnection) {
        printMessage("Connection lost...");
    }

    @Override
    public void onException(TCPConnection tcpConnection, Exception e) {
        printMessage("Client exception: " + e);
    }

    private synchronized void printMessage(String string){
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                log.append(string + "\n");
                log.setCaretPosition(log.getDocument().getLength());
            }
        });
    }

    private void updateUserList(String str){
        listModel.removeAllElements();
        String[] arr = str.split(";");
        int count = arr.length;
        for (int i = 0; i < count; i++) {
            listModel.add(i, arr[i]);
        }
    }
}
