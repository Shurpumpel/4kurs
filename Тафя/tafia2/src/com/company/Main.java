package com.company;

import java.io.*;
import java.nio.Buffer;
import java.util.ArrayList;
import java.util.List;

public class Main {

    public static void main(String[] args) {
        try {
            File file = new File("src/com/company/textfile");

            StateMachine stateMachine = new StateMachine(file);

            System.out.println("There are " + stateMachine.withHangs + " hangs");
            System.out.println("Automate is " + (stateMachine.deterministic ? "" : "not ") + "deterministic");

            stateMachine.getDFAFromNFA();

            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            System.out.println("Enter expression to check");
            String expression = reader.readLine();
            boolean res = stateMachine.isExpressionCorrect(expression);
            if (res)
                System.out.println("Expression correct!");
            else
                System.out.println("Incorrect expression. Error position: " + stateMachine.errorPos);

            reader.close();
        }catch (Exception e){
            System.out.println(e.getMessage());
        }
    }

}
