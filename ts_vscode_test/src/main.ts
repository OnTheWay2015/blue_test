const zlib = require('zlib');


const hello : string = "Hello World!"
console.log(hello)

const input = '这是一段要被压缩的文本。';
zlib.gzip(input, (err:any, result:any) => {
  if (err) {
    console.error('压缩时出错:', err);
  } else {
    console.log('压缩后的数据:', result);
  }
});

