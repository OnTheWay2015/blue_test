import * as PUPPETEER from 'puppeteer';
import * as FS from 'fs';
import * as PATH from 'path';
//const path = require('node:path');




const zlib = require('zlib');
//let url="http://www.aixiashu.info/109/109533/41816930.html"//招黑体质开局修行在废土 
let url="http://www.xbiqugu.la/75/75965/29824346.html"//招黑体质开局修行在废土 // todo 客户端会把 http 自动转成 https, 服务器没有支持会报错 
//let url="https://www.baidu.com"

const hello : string = "Hello World!"
console.log(hello)


let testZlib = () => {
  const input = '这是一段要被压缩的文本。';
  zlib.gzip(input, (err: any, result: any) => {
    if (err) {
      console.error('压缩时出错:', err);
    } else {
      console.log('压缩后的数据:', result);
    }
  });


    let a = PATH.join("./", "exportHtml.html")
    console.log(a);

};
//testZlib();


    let test_view = async ()=>{
      //puppeteer api文档
      //https://puppeteer.bootcss.com/api 

      //PUPPETEER.defaultArgs({
      //   headless:true, //是否无头模式
      //})

      const brower = await PUPPETEER.launch({
        //headless: true,
        headless: false, // 可以先设置为 false 可打开浏览器, 来查看是否因为 headless 模式导致的问题
        executablePath:"C:\\Users\\71304\\AppData\\Local\\Google\\Chrome\\Application\\chrome.exe", 
        //ignoreHTTPSErrors: true,  //废弃
        //ignoreDefaultArgs: ["--enable-automation"],
        args: [ 
         //   '--disable-web-security'
         //  , '--disable-features=IsolateOrigins,site-per-process'
         // ,'--disable-site-isolation-trials'
         // ,'--ignore-certificate-errors-spki-list' 
         // ,'--ignore-certificate-errors'/* 忽略证书错误*/

           //, '--headless=new'
            ]
      });
      //  args: [ `--headless=new`, `--disable-extensions-except=${pathToExtension}`, `--load-extension=${pathToExtension}`, '--no-sandbox', '--disable-setuid-sandbox' ]}
      //const page = await brower.newPage();
      const [page] = await brower.pages();//使用默认页面
      //await page.setRequestInterception(true);//清除缓存
      
      //await page.authenticate({'username':'admin','password':'admin1234'})

    // 将webdriver字段删除，防止反爬虫
    await page.evaluateOnNewDocument(() => {
        let a:any = navigator
        const newProto = a.__proto__;
        delete newProto.webdriver;
        a.__proto__ = newProto;
    })
    
    // 设置useragent，如果headless设置为true，则必做
    //await page.setUserAgent('Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.83 Safari/537.36 Edg/85.0.564.41');
      
    await page.setViewport({width:1080,height:720});
      //page.on('request', request => console.log('Request:', request.url()));
      //page.on('response', response => console.log('Response:', response.status()));
      page.on('requestfailed', request => console.log('Request failed:', request.url()));

      // 设置额外的 HTTP 头部
      await page.setExtraHTTPHeaders({
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.83 Safari/537.36 Edg/85.0.564.41',
        //'Accept-Language': 'en-US,en;q=0.5',
        //'Custom-Header': 'exampleValue',
        //"Host":"www.aixiashu.info" ,
        //"Upgrade-Insecure-Requests":"0",
        "Accept":"*/*" ,
        "Accept-Encoding":"gzip, deflate, br" ,
        "Connection":'keep-alive'
      });


      await page.goto(url,{
        timeout:130000
      });


      //await page.waitForSelector('ul.gl-warp>li'); //等待选择器要选的元素,复杂选择器
      //await page.waitForSelector('#s_btn_wr>#su'); //

//
      await page.screenshot({
         path:"./test.png"
         ,type:"png"
      });

      
      //await page.waitFor(15000);
    await new Promise(r => setTimeout(r, 10000));


      let h: any = await page.$("html");
      //BLUE.log(h.innerText); 
      //BLUE.log(h.innerHTML);
      let html = await h.evaluate((node: any) => node.innerHTML);
      //BLUE.log(html);
      FS.writeFileSync(PATH.join("./", "exportHtml.html"), html); 

      let content = await page.content();
      //BLUE.log(content);


      async function testinput() {
        
        //await page.waitForSelector('#su'); //s_btn_wr
        //await page.waitForSelector('#kw'); //s_btn_wr
        //let input_kw: any = await page.$('#kw');
        //let btn_su: any = await page.$('#su');
        //await input_kw.type("666");
        //await btn_su.click();

        
        await new Promise(r => setTimeout(r, 10000)); /* await page.waitFor(15000);*/
        await page.screenshot({
          path: "./test_after.png"
          , type: "png"
        });
        //return  new Promise();
      };

      await testinput();
      
      await page.close();
      await brower.close();




   };





test_view();