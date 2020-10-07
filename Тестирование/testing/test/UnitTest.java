import org.junit.jupiter.api.RepeatedTest;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class UnitTest {

    private final Mock mock = new Mock();
    private final String[] errorMessages = {"Error! Wrong number!",
    "Error! I can't divide to \"0\"!"};
    private final CalculatorPresenter calculatorPresenter = new CalculatorPresenterImplement(mock);

    @Test
    void firstArgNullSum(){
        mock.a = null;
        mock.b = String.valueOf(Math.random());
        calculatorPresenter.onPlusClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void secondArgNullSum(){
        mock.a = String.valueOf(Math.random());
        mock.b = null;
        calculatorPresenter.onPlusClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void allArgsNullSum(){
        mock.a = null;
        mock.b = null;
        calculatorPresenter.onPlusClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void firstArgNullSubtract(){
        mock.a = null;
        mock.b = String.valueOf(Math.random());
        calculatorPresenter.onMinusClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void secondArgNullSubtract(){
        mock.a = String.valueOf(Math.random());
        mock.b = null;
        calculatorPresenter.onMinusClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void firstArgNullMultiple(){
        mock.a = null;
        mock.b = String.valueOf(Math.random());
        calculatorPresenter.onMultiplyClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void secondArgNullMultiple(){
        mock.a = String.valueOf(Math.random());
        mock.b = null;
        calculatorPresenter.onMultiplyClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void firstArgNullDivide(){
        mock.a = null;
        mock.b = String.valueOf(Math.random());
        calculatorPresenter.onDivideClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void secondArgNullDivide(){
        mock.a = String.valueOf(Math.random());
        mock.b = null;
        calculatorPresenter.onDivideClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void secondArgZeroDivide(){
        mock.a = String.valueOf(Math.random());
        mock.b = String.valueOf(Math.random()*10e-8);
        calculatorPresenter.onDivideClicked();
        assertEquals(errorMessages[1], mock.error);

    }

    @Test
    void firstArgNotNumbersSum(){
        mock.a = "abcd";
        mock.b = String.valueOf(Math.random());
        calculatorPresenter.onPlusClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void secondArgNotNumbersSum(){
        mock.a = String.valueOf(Math.random());
        mock.b = "abcd";
        calculatorPresenter.onPlusClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void firstArgNotNumbersSubtract(){
        mock.a = "abcd";
        mock.b = String.valueOf(Math.random());
        calculatorPresenter.onMinusClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void secondArgNotNumbersSubtract(){
        mock.a = String.valueOf(Math.random());
        mock.b = "abcd";
        calculatorPresenter.onMinusClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void firstArgNotNumbersMultiple(){
        mock.a = "abcd";
        mock.b = String.valueOf(Math.random());
        calculatorPresenter.onMultiplyClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void secondArgNotNumbersMultiple(){
        mock.a = String.valueOf(Math.random());
        mock.b = "abcd";
        calculatorPresenter.onMultiplyClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void firstArgNotNumbersDivide(){
        mock.a = "abcd";
        mock.b = String.valueOf(Math.random());
        calculatorPresenter.onDivideClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    @Test
    void secondArgNotNumbersDivide(){
        mock.a = String.valueOf(Math.random());
        mock.b = "abcd";
        calculatorPresenter.onDivideClicked();
        assertEquals(errorMessages[0], mock.error);
    }

    //@RepeatedTest(10)
    @Test
    void checkSum(){
        double a = 0.1;
        double b = 0.2;
        mock.a = String.valueOf(a);
        mock.b = String.valueOf(b);
        calculatorPresenter.onPlusClicked();
        assertEquals(String.valueOf(a+b), mock.result);
    }

    @RepeatedTest(10)
    void checkSubtract(){
        double a = Math.random();
        double b = Math.random();
        mock.a = String.valueOf(a);
        mock.b = String.valueOf(b);
        calculatorPresenter.onMinusClicked();
        assertEquals(String.valueOf(a-b), mock.result);
    }

    @RepeatedTest(10)
    void checkMultiple(){
        double a = Math.random();
        double b = Math.random();
        mock.a = String.valueOf(a);
        mock.b = String.valueOf(b);
        calculatorPresenter.onMultiplyClicked();
        assertEquals(String.valueOf(a*b), mock.result);
    }

    @RepeatedTest(10)
    void checkDivide(){
        double a = Math.random();
        double b = Math.random();
        while(b==0.0){
            b = Math.random();
        }
        mock.a = String.valueOf(a);
        mock.b = String.valueOf(b);
        calculatorPresenter.onDivideClicked();
        assertEquals(String.valueOf(a/b), mock.result);
    }
}