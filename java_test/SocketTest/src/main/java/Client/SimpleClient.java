package Client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.rmi.UnknownHostException;

public class SimpleClient {

    public void Act(){
        String serverAddress = "127.0.0.1"; // 服务端的IP地址
        int port = 6666; // 服务端监听的端口
        try (Socket socket = new Socket(serverAddress, port)) {
            // 创建输入流和输出流对象
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            // 发送请求信息到服务端
            out.println("Hello, Server!");

            // 从服务端接收响应信息
            String response = in.readLine();
            System.out.println("Server Response: " + response);
        } catch (UnknownHostException e) {
            // 无法识别的主机地址
            System.err.println("Server address unknown.");
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
