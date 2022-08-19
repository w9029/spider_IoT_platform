import java.util.regex.Pattern;
import java.util.concurrent.TimeUnit;
import org.junit.*;
import static org.junit.Assert.*;
import static org.hamcrest.CoreMatchers.*;
import org.openqa.selenium.*;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.firefox.FirefoxDriver;
import org.openqa.selenium.support.ui.Select;

public class maintest {
  private WebDriver driver;
  private String baseUrl;
  private boolean acceptNextAlert = true;
  private StringBuffer verificationErrors = new StringBuffer();

  @Before
  public void setUp() throws Exception {
    System.setProperty("webdriver.chrome.driver","D:\\IDEA_pro\\233\\src\\chromedriver.exe");
    driver = new ChromeDriver();
    baseUrl = "https://www.katalon.com/";
    driver.manage().timeouts().implicitlyWait(30, TimeUnit.SECONDS);
  }

  @Test
  public void testUntitledTestCase() throws Exception {
    driver.get("http://192.168.100.63:8080/#/Main");
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='XXX物联网'])[1]/following::li[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='Sign In'])[2]/following::input[1]")).clear();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='Sign In'])[2]/following::input[1]")).sendKeys("zhy@qq.com");
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='Sign In'])[2]/following::input[2]")).clear();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='Sign In'])[2]/following::input[2]")).sendKeys("123");
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='忘记密码?'])[1]/following::button[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='确 定'])[2]/following::h2[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='鲸鱼养殖'])[1]/following::span[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='添加产品'])[1]/following::i[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='退出登录'])[1]/following::span[2]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='采集单元数量（个）'])[1]/following::span[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='添加采集单元'])[1]/following::i[1]")).click();
    driver.findElement(By.xpath("//div[@id='ManageLeftPart']/div/ul/li[3]/span")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='这是Page3的内容'])[1]/following::input[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='六'])[1]/following::span[10]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='六'])[1]/following::span[10]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='确认'])[1]/following::input[2]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='确认'])[1]/following::li[86]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='取消'])[1]/following::button[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='确定'])[1]/following::input[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='确定'])[1]/following::input[2]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='取消'])[2]/following::button[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='清空'])[1]/following::span[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='-'])[1]/following::i[2]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='蓝鲸养殖_1'])[1]/following::span[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='-'])[1]/following::span[3]")).click();
    driver.findElement(By.xpath("//div[@id='ManageLeftPart']/div/ul/li[4]/span")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='这是Page4的内容'])[1]/following::i[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='蓝鲸养殖_1'])[1]/following::span[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='这是Page4的内容'])[1]/following::span[3]")).click();
    driver.findElement(By.xpath("//div[@id='ManageLeftPart']/div/ul/li[5]/span")).click();
    driver.findElement(By.xpath("//div[@id='ManageLeftPart']/div/ul/li[6]/span")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='这是Page6的内容'])[1]/preceding::span[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='产业中心'])[1]/following::img[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='个人中心'])[1]/following::li[1]")).click();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='E-mail'])[1]/following::input[1]")).clear();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='E-mail'])[1]/following::input[1]")).sendKeys("zhy@qq.com");
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='Password'])[1]/following::input[1]")).clear();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='Password'])[1]/following::input[1]")).sendKeys("123");
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='验证码'])[1]/following::input[1]")).clear();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='验证码'])[1]/following::input[1]")).sendKeys("zhy@qq.com");
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='Password1'])[1]/following::input[1]")).clear();
    driver.findElement(By.xpath("(.//*[normalize-space(text()) and normalize-space(.)='Password1'])[1]/following::input[1]")).sendKeys("123");
  }

  @After
  public void tearDown() throws Exception {
    driver.quit();
    String verificationErrorString = verificationErrors.toString();
    if (!"".equals(verificationErrorString)) {
      fail(verificationErrorString);
    }
  }

  private boolean isElementPresent(By by) {
    try {
      driver.findElement(by);
      return true;
    } catch (NoSuchElementException e) {
      return false;
    }
  }

  private boolean isAlertPresent() {
    try {
      driver.switchTo().alert();
      return true;
    } catch (NoAlertPresentException e) {
      return false;
    }
  }

  private String closeAlertAndGetItsText() {
    try {
      Alert alert = driver.switchTo().alert();
      String alertText = alert.getText();
      if (acceptNextAlert) {
        alert.accept();
      } else {
        alert.dismiss();
      }
      return alertText;
    } finally {
      acceptNextAlert = true;
    }
  }
}
