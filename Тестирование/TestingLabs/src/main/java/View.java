import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class View extends JFrame implements CalculatorView {
    public JTextField argA;
    public JTextField argB;
    public JButton minus;
    public JButton divide;
    public JButton plus;
    public JButton multiple;
    public JTextField result;
    private JPanel panel;
    private int width = 400;
    private int height = 200;
    private CalculatorPresenter calculatorPresenter;
    public ErrorMessage errorMessage;


    public static void main(String[] args) {
        new View();
    }

    View(){
        this.calculatorPresenter = new CalculatorPresenterImplement(this);
        this.divide.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                calculatorPresenter.onDivideClicked();
            }
        });

        this.minus.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                calculatorPresenter.onMinusClicked();
            }
        });

        this.plus.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                calculatorPresenter.onPlusClicked();
            }
        });

        this.multiple.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                calculatorPresenter.onMultiplyClicked();
            }
        });

        setContentPane(panel);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(width, height);
        setLocationRelativeTo(null);
        setResizable(false);
        setVisible(true);
    }

    @Override
    public void printResult(double result) {
        this.result.setText(String.valueOf(result));
    }

    @Override
    public void displayError(String message) {
        this.errorMessage = new ErrorMessage(message);
        errorMessage.setVisible(true);
    }

    @Override
    public String getFirstArgumentAsString() {
        return this.argA.getText();
    }

    @Override
    public String getSecondArgumentAsString() {
        return this.argB.getText();
    }
}
