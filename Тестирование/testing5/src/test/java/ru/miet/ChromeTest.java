package ru.miet;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.chrome.ChromeDriver;

import java.awt.*;
import java.util.concurrent.TimeUnit;

import static org.junit.Assert.*;

public class ChromeTest {
    public static SearchPage page;
    public static WebDriver driver;

    @BeforeClass
    public static void setup() throws AWTException {


        //определение пути до драйвера и его настройка
        System.setProperty("webdriver.chrome.driver", ConfProperties.getProperty("chromedriver"));
        //создание экземпляра драйвера
        driver = new ChromeDriver();
        //окно разворачивается на полный экран
        driver.manage().window().maximize();
        //задержка на выполнение теста = 10 сек.
        driver.manage().timeouts().implicitlyWait(10, TimeUnit.SECONDS);
        //получение ссылки на страницу входа из файла настроек
        driver.get(ConfProperties.getProperty("mietpage"));

    }

    @Test
    public void isThereInputSearch() throws AWTException {
        page = new SearchPage(driver);
        assertNotNull(page.inputSearch);
    }

    @Test
    public void searchBadText() throws InterruptedException, AWTException {
        page = new SearchPage(driver);
        page.inputSearchString("Shurpumpel");
        page.pushEnter();
        Thread.sleep(100);
        assertEquals(0, page.getCountOfResults());
    }

    @Test
    public void searchHuman() throws AWTException {
        page = new SearchPage(driver);
        page.inputSearchString("Кожухов");
        page.pushEnter();
        page.choosePeopleList();
        assertTrue(page.isThereHuman("Кожухов Игорь Борисович"));
    }

    @AfterClass
    public static void closeBrowser(){
        driver.quit();
    }
}
