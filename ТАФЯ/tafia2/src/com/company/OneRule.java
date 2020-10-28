package com.company;

public class OneRule {
    public int state;
    public String symbol;
    public int nextState;
    boolean isNextTerminal;

    public OneRule(int state) {
        this.state = state;
    }

    public OneRule(String str){
        if (str.charAt(0) != 'q' && str.charAt(0) != 'Q')
            throw new IllegalArgumentException("Line must begin from q");

        if (!str.contains(","))
            throw new IllegalArgumentException("There is no comma");

        int commaPos = str.indexOf(',');
        this.state = Integer.parseInt(str.substring(1, commaPos));


        this.symbol = String.valueOf(str.charAt(commaPos+1));

        int nextState = commaPos + 3;
        if(str.charAt(commaPos+2)!='=') {
            if (str.charAt(commaPos + 2) == 'q' || str.charAt(commaPos + 2) == 'f') {
                this.symbol = "";
                nextState = commaPos + 2;
            }
            else
                throw new IllegalArgumentException("Line must contain '='");
        }

        switch (str.charAt(nextState)) {
            case 'f', 'F' -> this.isNextTerminal = true;
            case 'q', 'Q' -> this.isNextTerminal = false;
            default -> throw new IllegalArgumentException("Next state must begin from 'q' or 'f'");
        }

        this.nextState = Integer.parseInt(str.substring(nextState+1));

    }

    @Override
    public String toString() {
        return "OneRule{" +
                "state=" + state +
                ", symbol=" + symbol +
                ", nextState=" + nextState +
                ", isNextTerminal=" + isNextTerminal +
                '}';
    }
}