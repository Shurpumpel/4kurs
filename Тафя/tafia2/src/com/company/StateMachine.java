package com.company;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class StateMachine {
    public boolean deterministic;
    public int withHangs;
    public List<OneRule> stateList;
    public int errorPos;

    public StateMachine(File file) throws IOException {
        errorPos = -1;
        this.stateList = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(file));
        String line = reader.readLine();
        while(line!=null){
            this.stateList.add(new OneRule(line));
            line = reader.readLine();
        }
        reader.close();

        if(this.stateList.get(0).state != 0)
            throw new RuntimeException("There is no initial state");

        boolean hasFinishState = false;

        for (OneRule oneRule : this.stateList)
            if (hasFinishState = oneRule.isNextTerminal)
                break;

        if(!hasFinishState)
            throw new RuntimeException("There is no finish state");

        this.deterministic = isDeterministic();
        this.withHangs = hasHangs();
    }

    public void sortStates(){
        this.stateList.sort(new Comparator<OneRule>() {
            @Override
            public int compare(OneRule o1, OneRule o2) {
                return Integer.compare(o1.state, o2.state);
            }
        });
    }

    //хз нахрена этот метод
    public int findNextState(int nextState, String nextSymbol){
        int length = stateList.size();
        int next = -1;
        for (int i = 0; i < length; i++) {
            if(this.stateList.get(i).state==nextState && this.stateList.get(i).symbol.equals(nextSymbol)) {
                next = i;
                break;
            }
        }
        return next;
    }

    public int getRuleByState(int state){
        int val = 0;
        for (int i = 0; i < stateList.size(); i++) {
            if(stateList.get(i).state == state)
                val = i;
        }
        return val;
    }

    private boolean isDeterministic(){
        boolean isDet = true;

        int length = stateList.size();
        for (int i = 1; i < length; i++) {
            if(stateList.get(i-1).state == stateList.get(i).state &&
                    stateList.get(i - 1).symbol.equals(stateList.get(i).symbol) &&
                    (stateList.get(i-1).isNextTerminal != stateList.get(i).isNextTerminal||
                            stateList.get(i-1).nextState != stateList.get(i).nextState)){
                isDet = false;
                break;
            }

        }

        for (OneRule oneRule : stateList) {
            if (oneRule.symbol.equals("")) {
                isDet = false;
                break;
            }
        }
        return isDet;
    }

    private int hasHangs(){
        int hangs = 0;
        for (OneRule oneRule : stateList) {
            boolean found = false;
            if (oneRule.nextState == oneRule.state)
                found = true;

            if (found) {
                hangs++;
            }
        }
        return hangs;
    }

    public boolean isExpressionCorrect(String expression){
        if(!deterministic)
            throw new RuntimeException("This automate can't check expression");
        int currentState = 0;
        int length = expression.length();
        for (int i = 0; i < length; i++) {
            int idx = findNextState(currentState, String.valueOf(expression.charAt(i)));
            if(idx < 0){
                errorPos = i;
                return false;
            }

            if(stateList.get(idx).isNextTerminal) {
                if (i == length - 1) return true;
            }
            currentState = stateList.get(idx).nextState;
        }
        errorPos = length;
        return false;
    }

    public void getDFAFromNFA(){
        StringBuilder alphabet = new StringBuilder();
        List<Integer> F = new ArrayList<>();

        for (OneRule or : this.stateList){
            if(!alphabet.toString().contains(or.symbol))
                alphabet.append(or.symbol);



            if(or.isNextTerminal && !F.contains(or.nextState))
                F.add(or.nextState);
        }


        Stack<Map<Integer, String>> P = new Stack<>();
        Map<Integer, String> aa = new HashMap<>();
        aa.put(this.stateList.get(0).state, null);
        P.push(aa);

        List<Map<Integer, String>> Qd = new ArrayList<>();

        Qd.add(aa);
        List<Moves> moves = new ArrayList<>();
        while(!P.isEmpty()){
            Map<Integer, String> pd;
            pd = P.pop();
            for(char c : alphabet.toString().toCharArray()){
                Map<Integer, String> qd = new HashMap<>(); // int: state, char: prevSymbol

                for(Map.Entry<Integer, String> p : pd.entrySet()){
                    for(OneRule or : this.stateList){
                        if(or.symbol.equals(String.valueOf(c)) && or.state == p.getKey() && !qd.containsKey(or.nextState))
                            qd.put(or.nextState, String.valueOf(c));
                    }
                }
                if(Qd.contains(qd))
                    moves.add(new Moves(Qd.indexOf(pd), Qd.indexOf(qd), pd, qd, String.valueOf(c)));
                if(!Qd.contains(qd) && !qd.isEmpty()){
                    Qd.add(qd);
                    moves.add(new Moves(Qd.indexOf(pd), Qd.indexOf(qd), pd, qd, String.valueOf(c)));
                    P.push(qd);
                }
            }
        }

        List<OneRule> or = new ArrayList<>();
        for (int i = 0; i < moves.size(); i++) {
            or.add(new OneRule(moves.get(i).state1));
            or.get(i).symbol = moves.get(i).symbol;
            or.get(i).nextState = moves.get(i).state2;
            or.get(i).isNextTerminal = false;
            for(int f : F){
                if(moves.get(i).qd.containsKey(f)){
                    or.get(i).isNextTerminal = true;
                    break;
                }
            }
        }
        this.stateList = new ArrayList<>(or);
        this.deterministic = this.isDeterministic();
    }

    public void printAutomat(){
        for (OneRule or : this.stateList){
            StringBuilder str = new StringBuilder();
            str.append("q");
            str.append(or.state);
            str.append(",");
            str.append(or.symbol);
            str.append("=");
            if(or.isNextTerminal)
                str.append("f");
            else
                str.append("q");
            str.append(or.nextState);
            System.out.println(str.toString());
        }
    }
}

class Moves{
    int state1;
    int state2;
    Map<Integer, String> pd;
    Map<Integer, String> qd;
    String symbol;

    public Moves(int state1, int state2, Map<Integer, String> pd, Map<Integer, String> qd, String symbol) {
        this.state1 = state1;
        this.state2 = state2;
        this.pd = pd;
        this.qd = qd;
        this.symbol = symbol;
    }

}
