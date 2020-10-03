package com.company;

import java.io.*;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client {
    public static void main(String[] args) throws InterruptedException, IOException {
        try(
                Socket socket = new Socket("localhost", 8000);
                BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));
                BufferedReader serverReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())))
        {
            System.out.println("Client start writing in channel...");
            while (true) {
                String clientCommand = consoleReader.readLine();
                writer.write(clientCommand);
                writer.newLine();
                writer.flush();

            }
        }catch (IOException e){
            e.printStackTrace();
        }
    }
}
