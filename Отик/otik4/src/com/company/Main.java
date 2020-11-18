package com.company;


import java.io.*;
import java.util.*;

public class Main {

    public static Map<String, Double> P = new HashMap<>();
    public static int inputLength;

    public static void main(String[] args) throws IOException {
        File file = new File("src/com/company/textfile");
        BufferedReader reader = new BufferedReader(new FileReader(file));
        StringBuilder text = new StringBuilder();
        String str = reader.readLine();
        while (str!=null){
            text.append(str);
            str = reader.readLine();
        }

        String a = encode(text.toString());
        System.out.println(a);
        System.out.println(text.toString());
        System.out.println(decode(a));
    }

    public static String encode(String str){
        StringBuilder encoded = new StringBuilder();
        getProbability(str);
        P = sortMap(P);
        List<Segment> segments = getDefineSegments();
        System.out.println(segments);
        int left = 0;
        int right = 10000;
        for (int i = 0; i < inputLength; i++) {
            char symb = str.charAt(i);
            for (Segment segment : segments) {
                if (segment.symbol == symb) {
                    System.out.println("left:"+left +"\t"+"right: "+right);
                    int newRight = (int)(left + (right - left) * segment.right);
                    int newLeft = (int)(left + (right - left) * segment.left);

                    String newRightString = String.valueOf(newRight);
                    String newLeftString = String.valueOf(newLeft);
                    while (newRightString.charAt(0) == newLeftString.charAt(0)){
                        encoded.append(newRightString.charAt(0));
                        newLeftString = newLeftString.substring(1);
                        newRightString = newRightString.substring(1);
                        newRight = Integer.parseInt(newRightString+"9");
                        newLeft = Integer.parseInt(newLeftString + "0");
                        newRightString = String.valueOf(newRight);
                        newLeftString = String.valueOf(newLeft);
                    }

                    right = newRight;
                    left = newLeft;
                    break;
                }
            }
        }
        encoded.append(left);
        return encoded.toString();
    }


    public static String decode(String str){
        List<Segment> segments = getDefineSegments();
        String str1 = ""+str;
        StringBuilder s = new StringBuilder();
        int left = 0;
        int right = 10000;
        int Code = Integer.parseInt(str1.substring(0, 4));
        str1 = str.substring(4);
        double index = Code / 10000.0;

        for (int i = 0; i < inputLength; i++) {
            for(Segment segment : segments){
                if(index >= segment.left && index < segment.right){
                    s.append(segment.symbol);
                    System.out.println("left:"+left +"\t"+"right: "+right);
                    int newRight = (int)(left + (right - left) * segment.right);
                    int newLeft = (int)(left + (right - left) * segment.left);

                    String newRightString = String.valueOf(newRight);
                    String newLeftString = String.valueOf(newLeft);

                    while (newRightString.charAt(0) == newLeftString.charAt(0)){
                        String CodeString = String.valueOf(Code);
                        if(str1.length()>0) {
                            CodeString = CodeString.substring(1);
                            Code = Integer.parseInt(CodeString + str1.charAt(0));
                            str1 = str1.substring(1);
                        }
                        newLeftString = newLeftString.substring(1);
                        newRightString = newRightString.substring(1);
                        newRight = Integer.parseInt(newRightString+"9");
                        newLeft = Integer.parseInt(newLeftString + "0");
                        newRightString = String.valueOf(newRight);
                        newLeftString = String.valueOf(newLeft);
                    }

                    right = newRight;
                    left = newLeft;

                    index = 1.0*(Code - left) / (right - left);
                    break;
                }
            }
        }
        return s.toString();
    }

    private static void getProbability(String str){
        inputLength = str.length();
        Map<String, Integer> countOfSymbols = new HashMap<>();
        for (int i = 0; i < str.length(); i++) {
            if(!countOfSymbols.containsKey(String.valueOf(str.charAt(i)))){
                countOfSymbols.put(String.valueOf(str.charAt(i)), 1);
            }else{
                int oldValue = countOfSymbols.get(String.valueOf(str.charAt(i)));
                countOfSymbols.replace(String.valueOf(str.charAt(i)), oldValue+1);
            }
        }

        for(Map.Entry<String, Integer> entry: countOfSymbols.entrySet()){
            double p = 1.0 * entry.getValue() / inputLength;
            P.put(entry.getKey(), p);
        }

    }

    public static List<Segment> getDefineSegments(){
        List<Segment> list= new ArrayList<>();
        double l = 0;
        for(Map.Entry<String, Double>entry : P.entrySet()){
            list.add(new Segment(l, l+entry.getValue(), entry.getKey().charAt(0)));
            l += entry.getValue();
        }
        return list;
    }

    static class Segment{
        double left;
        double right;
        char symbol;
        Segment (double left, double right, char symbol){
            this.left = left;
            this.right = right;
            this.symbol = symbol;
        }

        @Override
        public String toString() {
            return "{" +
                    "left=" + left +
                    ", right=" + right +
                    ", symbol=" + symbol +
                    '}'+"\n";
        }
    }

    public static LinkedHashMap<String, Double> sortMap(Map<String, Double> map){
        List<Map.Entry<String, Double>> entries =new ArrayList<>(map.entrySet());
        entries.sort((a, b) -> b.getValue().compareTo(a.getValue()));
        LinkedHashMap<String, Double> sortedMap = new LinkedHashMap<>();
        for (Map.Entry<String, Double> entry : entries) {
            sortedMap.put(entry.getKey(), entry.getValue());
        }
        return sortedMap;
    }

}
