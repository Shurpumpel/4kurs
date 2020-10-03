package com.company;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

    public static void main(String[] args) {


        try(ServerSocket server = new ServerSocket(8000)) {
            System.out.println("Server started!");
            while(true)
                try (
                        Socket client = server.accept();
                        BufferedReader reader = new BufferedReader(
                                new InputStreamReader(client.getInputStream()));
                        BufferedWriter writer = new BufferedWriter(
                                new OutputStreamWriter(client.getOutputStream())))
                {
                    System.out.println("Server reading...");
                    String request = reader.readLine();
                    System.out.println("Request: " + request);

                    if (request.equals("quit")) {
                        System.out.println("Client initialise connection suicide");
                        writer.write("Server disconnect");
                        break;
                    }
                    writer.write("It's response");
                    writer.newLine();
                    writer.flush();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            System.out.println("Disconnection DONE");



        }catch (IOException e){
            e.printStackTrace();
        }
    }
}
