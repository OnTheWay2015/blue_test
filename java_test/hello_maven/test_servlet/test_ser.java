package test_servlet;

import jakarta.servlet.*;
import jakarta.servlet.http.*;
import java.io.*;
public class test_ser extends HttpServlet{
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
        resp.setContentType("text/plain;charset=UTF-8");
        PrintWriter out = resp.getWriter();
        out.print("手动响应内容");
        out.close();
    }
}
