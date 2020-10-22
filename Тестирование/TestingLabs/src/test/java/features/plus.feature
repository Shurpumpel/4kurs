@calculator
Feature: Sum
	As a math idiot
	I want to be told the sum of two numbers
	So that I can avoid silly mistakes

@positive
Scenario: Add two numbers
	Given I have entered first number 50.0 into the calculator
	And I have entered second number 70.0 into the calculator
	When I press plus
	Then the result should be 120.0 on the screen
