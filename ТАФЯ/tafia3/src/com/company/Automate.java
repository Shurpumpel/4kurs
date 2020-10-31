package com.company;

import java.io.*;
import java.util.List;

public class Automate {
    List<Command> commands;

    public Automate(File file) throws IOException {
        BufferedReader reader = new BufferedReader(new FileReader(file));
        String str = reader.readLine();
        while(str!=null){


            str = reader.readLine();
        }
    }

    Command getCommandFromString(String string){
        Command command;
        String a[] = string.split(">");
        
        return command;
    }
}

class Command{
    int startState;
    String symbol;
    String stackToRead;

    int nextState;
    String stackToWrite;

    public Command(int startState, String symbol, String stackToRead, int nextState, String stackToWrite) {
        this.startState = startState;
        this.symbol = symbol;
        this.stackToRead = stackToRead;
        this.nextState = nextState;
        this.stackToWrite = stackToWrite;
    }
}