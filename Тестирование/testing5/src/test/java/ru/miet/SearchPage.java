package ru.miet;

import org.openqa.selenium.By;
import org.openqa.selenium.Keys;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.PageFactory;

import java.awt.*;
import java.util.List;

public class SearchPage {

    public WebDriver driver;
    public final WebElement inputSearch;

    public SearchPage(WebDriver driver) throws AWTException {
        PageFactory.initElements(driver, this);
        this.driver = driver;
        this.inputSearch = driver
                .findElement(By.xpath("//input[@class = 'search-bar__input']"));

    }

    public boolean isThereHuman(String fullName){
        List<WebElement> webElementList = driver.findElements(By.xpath("//*[@class = 'people-list__item-name']"));
        for(WebElement webElement : webElementList){
            System.out.println(webElement.getText());
            if(webElement.getText().equals(fullName))
                return true;
        }
        return false;
    }

    public int getCountOfResults(){
        List<WebElement> webElementList = driver.findElements(By.xpath("//a[@class = 'search-bar__list-item ']"));
        int sum = 0;
        for (WebElement countInGroup : webElementList) {
            int a = Integer.parseInt(countInGroup.getAttribute("data-count"));
            sum += a;
        }
        return sum;
    }

    public void inputSearchString(String str){
        inputSearch.clear();
        inputSearch.sendKeys(str);
    }

    public void choosePeopleList(){
        WebElement element = driver.findElement(By.xpath("//html//body//main//div//div[1]//div//a[2]"));
        element.click();
    }

    public void pushEnter(){
        inputSearch.sendKeys(Keys.ENTER);
    }

}
