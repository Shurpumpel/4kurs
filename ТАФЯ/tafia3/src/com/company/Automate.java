package com.company;

import java.io.*;
import java.util.*;

public class Automate {

    List<Command> commands;

    public Automate(File file) throws IOException {
        this.commands = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(file));
        String str = reader.readLine();
        while(str!=null){
            String[] leftRight = str.split(">");
            String[] rightParts = leftRight[1].split("\\|");
            for(String ss : rightParts){
                StringBuilder sb = new StringBuilder(ss);
                String s1 = sb.reverse().toString();
                char[] ss1 = s1.toCharArray();
                for (int i = 0; i < ss1.length; i++) {
                    switch (ss1[i]){
                        case '(':
                            ss1[i] = ')';
                            break;
                        case ')':
                            ss1[i] = '(';
                            break;
                        default:
                            break;
                    }
                }
                this.commands.add(new Command(0, "λ", leftRight[0], 0, new String(ss1)));
            }
            for(String ss : rightParts){
                char[] symbols = ss.toCharArray();
                for (char symbol : symbols) {
                    if (!Character.isUpperCase(symbol)) {
                        String word = String.valueOf(symbol);
                        this.commands.add(new Command(0, word, word, 0, "λ"));
                    }
                }
            }
            str = reader.readLine();
        }
    }

    public boolean Check_string(String str, String state, int step, int max)
    {
        String str1;
        String state1;

        if ((str.equals("λ")) && (state.equals("h0")))
            return true;

        boolean a = false;
        if (str.equals(""))
            return false;

        if (step == max)
        {
            return false;
        }

        String s1 = String.valueOf(state.charAt(state.length() - 1));
        for (Command command : commands) {
            if (s1.equals(command.word) && String.valueOf(str.charAt(0)).equals(command.word)) {
                str1 = str.substring(1);
                if (str1.equals(""))
                    str1 = "λ";
                state1 = state.substring(0, state.length() - 1);

                a = Check_string(str1, state1, step + 1, max);
                if (a) {
                    System.out.println("(s0," + str1 + "," + state1 + ")");
                    return true;
                }
            }
        }

        for (Command command : commands) {
            String s = s1;
            if (s.equals(command.text) && command.word.equals("λ")) {
                state1 = state.substring(0, state.length() - 1) + command.new_text;
                str1 = str;

                a = Check_string(str1, state1, step + 1, max);
                if (a) {
                    System.out.println("(s0," + str1 + "," + state1 + ")");
                    return true;
                }
            }
        }
        return false;
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        for(Command command : commands){
            stringBuilder.append(command.toString());
            stringBuilder.append("\n");
        }
        return stringBuilder.toString();
    }


}

class Command{
    int state;
    String word;
    String text;
    int new_state;
    String new_text;

    public Command(int state, String word, String text, int new_state, String new_text) {
        this.state = state;
        this.word = word;
        this.text = text;
        this.new_state = new_state;
        this.new_text = new_text;
    }

    @Override
    public String toString() {
        return "v(s"+this.state+", "+
                this.word+", "+
                this.text+") = (s"+
                this.new_state+", "+
                this.new_text+")";
    }
}