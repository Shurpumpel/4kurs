import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.InputEvent;

import static org.junit.jupiter.api.Assertions.*;

class GUITest {
    private static View gui;
    private static Robot robot;
    private static double argA;
    private static double argB;
    private final String[] errorMessages = {"Error! Wrong number!", "Error! I can't divide to \"0\"!"};

    @BeforeAll
    private static void init() throws AWTException {
        gui = new View();
        robot = new Robot();
        argA = 0.456;
        argB = 1.45687;
    }

    private static void click(JButton button) throws InterruptedException {
        Point p = button.getLocationOnScreen();
        int dx, dy;
        dx = button.getWidth()/2;
        dy=button.getHeight()/2;
        robot.mouseMove(p.x + dx, p.y + dy);
        robot.mousePress(InputEvent.BUTTON1_DOWN_MASK);
        robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
        Thread.sleep(100);
    }

    private void setA(double a) throws InterruptedException {
        gui.argA.setText(Double.toString(a));
        Thread.sleep(20);
    }
    private void setB(double b) throws InterruptedException {
        gui.argB.setText(Double.toString(b));
        Thread.sleep(20);
    }

    @Test
    public void testPlus() throws InterruptedException {
        setA(argA);
        setB(argB);
        click(gui.plus);
        assertEquals(Double.toString(argA + argB), gui.result.getText());
        Thread.sleep(100);
    }

    @Test
    public void testMinus() throws InterruptedException {
        setA(argA);
        setB(argB);
        click(gui.minus);
        assertEquals(Double.toString(argA - argB), gui.result.getText());
        Thread.sleep(100);
    }

    @Test
    public void testMultiple() throws InterruptedException {
        setA(argA);
        setB(argB);
        click(gui.multiple);
        assertEquals(Double.toString(argA * argB), gui.result.getText());
        Thread.sleep(100);
    }

    @Test
    public void testDivide() throws InterruptedException {
        setA(argA);
        setB(argB);
        click(gui.divide);
        assertEquals(Double.toString(argA / argB), gui.result.getText());
        Thread.sleep(100);
    }

    @Test
    public void testNullPlusB() throws InterruptedException {
        gui.argA.setText(null);
        gui.errorMessage = null;
        setB(argB);
        click(gui.plus);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testNullMinusB() throws InterruptedException {
        gui.argA.setText(null);
        gui.errorMessage = null;
        setB(argB);
        click(gui.minus);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testNullsMultipleB() throws InterruptedException {
        gui.argA.setText(null);
        gui.errorMessage = null;
        setB(argB);
        click(gui.multiple);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testNullDivideB() throws InterruptedException {
        gui.argA.setText(null);
        gui.errorMessage = null;
        setB(argB);
        click(gui.divide);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testAPlusNull() throws InterruptedException {
        setA(argA);
        gui.argB.setText(null);
        gui.errorMessage = null;
        click(gui.plus);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testAMinusNull() throws InterruptedException {
        setA(argA);
        gui.argB.setText(null);
        gui.errorMessage = null;
        click(gui.minus);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testAMultipleNull() throws InterruptedException {
        setA(argA);
        gui.argB.setText(null);
        gui.errorMessage = null;
        click(gui.multiple);
        assertNotNull(gui.multiple);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testADivideNull() throws InterruptedException {
        setA(argA);
        gui.argB.setText(null);
        gui.errorMessage = null;
        click(gui.divide);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testStringPlusB() throws InterruptedException {
        gui.argA.setText("some string");
        setB(argB);
        gui.errorMessage = null;
        click(gui.plus);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testStringMinusB() throws InterruptedException {
        gui.argA.setText("some string");
        setB(argB);
        gui.errorMessage = null;
        click(gui.minus);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testStringMultipleB() throws InterruptedException {
        gui.argA.setText("some string");
        setB(argB);
        gui.errorMessage = null;
        click(gui.multiple);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testStringDivideB() throws InterruptedException {
        gui.argA.setText("some string");
        setB(argB);
        gui.errorMessage = null;
        click(gui.divide);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testAPlusString() throws InterruptedException {
        setA(argA);
        gui.argB.setText("some text");
        gui.errorMessage = null;
        click(gui.plus);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testAMinusString() throws InterruptedException {
        setA(argA);
        gui.argB.setText("some text");
        gui.errorMessage = null;
        click(gui.minus);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testAMultipleString() throws InterruptedException {
        setA(argA);
        gui.argB.setText("some text");
        gui.errorMessage = null;
        click(gui.multiple);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void testADivideString() throws InterruptedException {
        setA(argA);
        gui.argB.setText("some text");
        gui.errorMessage = null;
        click(gui.divide);
        assertNotNull(gui.errorMessage);
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

    @Test
    public void DivideByZero() throws InterruptedException {
        setA(argA);
        setB(0.0);
        gui.errorMessage = null;
        click(gui.divide);
        assertEquals("Error! I can't divide to \"0\"!", gui.errorMessage.label.getText());
        click(gui.errorMessage.buttonOK);
        Thread.sleep(100);
    }

}