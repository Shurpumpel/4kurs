@calculator
Feature: Div
	As a math idiot
	I want to be told the div of two numbers
	So that I can avoid silly mistakes

@positive
Scenario: Divide two numbers
	Given I have entered first number 50.0 into the calculator
	And I have entered second number 70.0 into the calculator
	When I press divide
	Then the result should be 0.7142857142857143 on the screen
