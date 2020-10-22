@calculator
Feature: DivManyTimes
  As a math idiot
  I want to be told the div of two numbers
  So that I can avoid silly mistakes

  @positive
  Scenario Outline: Divide two numbers
    Given I have entered first number <A> into the calculator
    And I have entered second number <B> into the calculator
    When I press divide
    Then the result should be <result> on the screen

    Examples:
      | A    |  B |result|
      | 50.0 |2   |25.0  |
      | 100.0|50.0| 2.0  |

