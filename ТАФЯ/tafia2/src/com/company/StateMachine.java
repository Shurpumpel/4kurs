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
            for (OneRule rule : stateList) {
                if (oneRule.nextState == rule.state) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                hangs++;
            }
        }
        return hangs;
    }

    public boolean isExpressionCorrect(String expression){
        if(!deterministic || withHangs > 0)
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
                errorPos = i + 1;
                return false;
            }
            currentState = stateList.get(idx).nextState;
        }
        errorPos = length;
        return false;
    }

    public void getDFAFromNFA(){
        for (int i = 0; i < stateList.size(); i++) {
            if(stateList.get(i).symbol.equals("")){
                int nextPos = getRuleByState(stateList.get(i).nextState);
                stateList.get(i).symbol = stateList.get(nextPos).symbol;
            }
        }
    }

}
