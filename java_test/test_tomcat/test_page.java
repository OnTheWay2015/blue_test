import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import jakarta.servlet.http.HttpSession;

@WebServlet("/test")
public class test_page extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        dumpRequest(request);

        // 设置响应内容类型为HTML
        response.setContentType("text/html;charset=UTF-8");
        // 获取输出流对象
        PrintWriter out = response.getWriter();
        // 获取Session对象，如果不存在则创建一个新的
        HttpSession session = request.getSession(true);
        Integer count = (Integer) session.getAttribute("count");
        if (count == null) {
            count = 1;
        } else {
            count++;
        }
        session.setAttribute("count", count);

        out.println("<html><body>");
        out.println("<h1>您访问本页面的次数是：" + count + "</h1>");
        out.println("</body></html>");
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //返回 json
        dumpRequest(request);

        response.setContentType("application/json;charset=utf-8"); // 设置响应类型:ml-citation{ref="2" data="citationList"}
        response.setCharacterEncoding("UTF-8");

        // 手动拼接 JSON 字符串
        String jsonStr = "{\"name\":\"Tom\", \"age\":25}";
        PrintWriter out = response.getWriter();
        out.print(jsonStr);
        out.flush();

    }
    private void dumpRequest(HttpServletRequest request){
        HttpServletRequest httpRequest = (HttpServletRequest) request;
        System.out.println("Request URI: " + httpRequest.getRequestURI());
        System.out.println("Request Method: " + httpRequest.getMethod());
        System.out.println("Request Headers: " + httpRequest.getHeaderNames());
        httpRequest.getHeaderNames().asIterator().forEachRemaining(headerName -> {
            System.out.println(headerName + ": " + httpRequest.getHeader(headerName));
        });
        System.out.println("Request Parameters: " + httpRequest.getParameterMap());
        httpRequest.getParameterMap().forEach((key, value) -> {
            System.out.println(key + ": " + String.join(", ", value));
        });

    }



}