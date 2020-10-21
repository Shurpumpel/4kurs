package com.company;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Stack;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {

    public static boolean isError = false;

    public static HashMap<Character, Integer> priority = new HashMap<Character, Integer>();

    public static List<String> polishString = new ArrayList<>();


    static {
        priority.put('+', 2);
        priority.put('-', 2);
        priority.put('*', 3);
        priority.put('/', 3);
    }

    public static void main(String[] args) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        while (true) {
            System.out.println("Insert string");
            String string = reader.readLine();
            try {
                createPolishString(string);
                System.out.println(polishString);
                System.out.println(getSolution());
                polishString.clear();
            }catch (NullPointerException e){
                System.out.println("You didn't insert!");
            }catch (ArithmeticException e){
                System.out.println("There are bad brackets");
            }catch (NumberFormatException e){
                System.out.println("Don't insert words");
            }
        }
    }

    public static void createPolishString(String in) {
        Stack<Character> stack = new Stack<>();
        String number = "";
        String str = in;
        str = solveSpecialFunction(in);
        for (int i = 0; i < str.length(); i++) {
            char symbol = str.charAt(i);
            if (Character.isDigit(symbol) || (symbol == '.')) {
                number += symbol;
            } else {
                if (!number.equals("")) {
                    polishString.add(number);
                    number = "";
                }
                switch (symbol) {
                    case '(':
                        stack.push(symbol);
                        break;
                    case ')':
                        boolean isOpenBracket = false;
                        while (!stack.isEmpty()) {
                            if(stack.peek() != '(')
                                polishString.add(String.valueOf(stack.pop()));
                            else {
                                isOpenBracket = true;
                                stack.pop();
                                break;
                            }
                        }
                        if(!isOpenBracket){
                            System.out.println("Error in brackets!");
                            isError = true;
                            return;
                        }
                        break;
                    case '+':
                        while (!stack.isEmpty()) {
                            if(getPriority(stack.peek()) >= getPriority('+'))
                                polishString.add(String.valueOf(stack.pop()));
                            else
                                break;
                        }
                        stack.push('+');
                        break;
                    case '-':
                        while (!stack.isEmpty()) {
                            if(getPriority(stack.peek()) >= getPriority('-'))
                                polishString.add(String.valueOf(stack.pop()));
                            else break;
                        }
                        stack.push('-');
                        break;
                    case '*':
                        while (!stack.isEmpty()) {
                            if(getPriority(stack.peek()) >= getPriority('*'))
                                polishString.add(String.valueOf(stack.pop()));
                            else break;
                        }
                        stack.push('*');
                        break;
                    case '/':
                        while (!stack.isEmpty()) {
                            if(getPriority(stack.peek()) >= getPriority('/'))
                                polishString.add(String.valueOf(stack.pop()));
                            else break;
                        }
                        stack.push('/');
                        break;
                    default:
                        throw new NumberFormatException();
                }
            }
        }
        if(!number.equals(""))
            polishString.add(number);
        while (!stack.isEmpty()){
            if(stack.peek() == '('){
                throw new ArithmeticException();
            }
            polishString.add(String.valueOf(stack.pop()));
        }
    }

    public static double getSolution(){
        Stack<String> stack = new Stack<>();
        double a,b;
        for (String s : polishString) {
            switch (s) {
                case "+" -> {
                    a = Double.parseDouble(stack.pop());
                    b = Double.parseDouble(stack.pop());
                    stack.push(String.valueOf(a + b));
                }
                case "-" -> {
                    a = Double.parseDouble(stack.pop());
                    b = Double.parseDouble(stack.pop());
                    stack.push(String.valueOf(b - a));
                }
                case "*" -> {
                    a = Double.parseDouble(stack.pop());
                    b = Double.parseDouble(stack.pop());
                    stack.push(String.valueOf(a * b));
                }
                case "/" -> {
                    a = Double.parseDouble(stack.pop());
                    b = Double.parseDouble(stack.pop());
                    stack.push(String.valueOf(b / a));
                }
                default -> stack.push(s);
            }
        }
        if(stack.isEmpty()){
            throw new NullPointerException();
        }

        return Double.parseDouble(stack.pop());
    }

    public static int getPriority(char a) {
        return priority.getOrDefault(a, -1);
    }

    public static String solveSpecialFunction(String in){
        Pattern pattern = Pattern.compile("log\\((\\d+\\.\\d*|\\d+),\\s*(\\d+\\.\\d*|\\d+)\\)");
        Matcher matcher = pattern.matcher(in);
        String string = new String(in);
        while (matcher.find()){
            String str1 = matcher.group();
            double a = Double.parseDouble(str1.substring(str1.indexOf("(")+1, str1.indexOf(",")));
            double b = Double.parseDouble(str1.substring(str1.indexOf(",")+1, str1.indexOf(")")));
            String str2 = String.valueOf(Math.log(a)/Math.log(b));
            string = string.replace(str1, str2);
        }
        return string;
    }
}
