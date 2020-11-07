package com.company;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Main {

    public static void main(String[] args) throws IOException {
        File file = new File("src/com/company/textfile");
        Automate automate = new Automate(file);
        System.out.println("Получившийся автомат:");
        System.out.println(automate.toString());
        while(true){
            System.out.println("Введите строку");
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            String str = reader.readLine();
            boolean a = automate.Check_string(str, "h0E", 0, 100);
            if(a) {
                System.out.println("Строку можно разобрать");
            }else{
                System.out.println("Строку нельзя разобрать");
            }
        }
    }
}
