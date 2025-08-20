package com.fqwz.springbootproj.normal_m;

import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

import java.io.PrintWriter;
import java.io.IOException;


@WebServlet("/normal/first")
public class NormalServlet extends HttpServlet {
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        System.out.println("normal Servlet........");

        // 设置响应内容类型为HTML
        response.setContentType("text/html;charset=UTF-8");
        // 获取输出流对象
        PrintWriter out = response.getWriter();

        out.println("<html><body>");
        out.println("<h1>  normal Servlet </h1>");
        out.println("</body></html>");
    }
}
