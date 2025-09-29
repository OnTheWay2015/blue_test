package com.fqwz.springbootproj.normal_ai;

import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

import java.io.PrintWriter;
import java.io.IOException;


@WebServlet("/ai/first")
public class AiNormalServlet extends HttpServlet {
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        System.out.println("ai Servlet........");

        // 设置响应内容类型为HTML
        response.setContentType("text/html;charset=UTF-8");
        // 获取输出流对象
        PrintWriter out = response.getWriter();

        out.println("<html><body>");
        out.println("<h1>  ai Servlet </h1>");
        out.println("</body></html>");
    }
}
