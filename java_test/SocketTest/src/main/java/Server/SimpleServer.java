package Server;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class SimpleServer {
    public void Start() throws IOException{
        int port = 6666; // 选择一个端口
        ServerSocket serverSocket = new ServerSocket(port); // 绑定端口
        System.out.println("Server started on port " + port);
        while (true) {
            final Socket socket = serverSocket.accept(); // 接受客户端连接
            // 这里可以创建一个新的线程来处理客户端请求
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        // 处理客户端socket的通信

                        InputStream input = socket.getInputStream();
                        OutputStream output = socket.getOutputStream();

                        // 示例：发送数据到客户端
                        output.write("Hello, client!".getBytes());

                        // 示例：从客户端读取数据
                        byte[] buffer = new byte[1024];
                        int len = input.read(buffer);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }
}
