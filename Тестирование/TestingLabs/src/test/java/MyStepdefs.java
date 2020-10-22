import cucumber.api.java.en.And;
import cucumber.api.java.en.Given;
import cucumber.api.java.en.Then;
import cucumber.api.java.en.When;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

public class MyStepdefs {
    View app;
    @Given("^I have entered first number ([0-9.,-]+) into the calculator$")
    public void enterFirst(double arg1){
        app = new View();
        app.setVisible(true);
        app.argA.setText(Double.toString(arg1));
    }
    @And("^I have entered second number ([0-9.,-]+) into the calculator$")
    public void enterSecond(double arg1) {
        app.argB.setText(Double.toString(arg1));
    }
    @When("^I press plus$")
    public void pressPlus() {
        app.plus.doClick();
    }
    @When("^I press minus$")
    public void pressMinus() {
        app.minus.doClick();
    }
    @When("^I press multiply$")
    public void pressMultiply() {
        app.multiple.doClick();
    }
    @When("^I press divide$")
    public void pressDivide() {
        app.divide.doClick();
    }
    @Then("^the result should be ([0-9.,-]+) on the screen$")
    public void result(double arg1){
        assertEquals(Double.toString(arg1), app.result.getText());
    }

    @Then("^there should be error window$")
    public void result(){
        assertNotNull(app.errorMessage);
    }
}
