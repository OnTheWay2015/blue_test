package com.fqwz.springbootproj.normal_ai;

import org.springframework.ai.chat.messages.Message;
import org.springframework.ai.chat.messages.SystemMessage;
import org.springframework.ai.chat.messages.UserMessage;
import org.springframework.ai.chat.model.ChatResponse;
import org.springframework.ai.chat.prompt.ChatOptions;
import org.springframework.ai.chat.prompt.Prompt;
import org.springframework.ai.deepseek.DeepSeekChatModel;
import org.springframework.ai.deepseek.DeepSeekChatOptions;
import org.springframework.ai.deepseek.api.DeepSeekApi;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import org.yaml.snakeyaml.util.Tuple;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;

import javax.validation.constraints.Null;
import java.util.*;
@RestController //要返回json时使用（它组合了@Controller和 @ResponseBody）
//@Controller
public class AiNormalController {
    @RequestMapping("/ai/hello")
    @ResponseBody
    public String hello(){
        System.out.println("ai  Controller!");
        //tttt();
        return "ai Controller!";
    }


    //这里是返回 flux 异步处理结果，注意要添加 produces 设定
     @RequestMapping(value = "/send-stream", produces = MediaType.TEXT_EVENT_STREAM_VALUE)
    public Flux<AiChatMessageSendRespVO> sendChatMessageStream() {
        //return chatMessageService.sendChatMessageStream(sendReqVO, getLoginUserId());

         return tttt();

        //return null;
    }
/*

chatMessageService.sendChatMessageStream(sendReqVO, getLoginUserId())
        .onBackpressureBuffer() // 处理背压
        .doOnNext(response -> log.info("Sending stream response: {}", response))
        .doOnError(e -> log.error("Stream error occurred", e))
        .doOnComplete(() -> log.info("Stream completed"));

*
* */



    //openAiChatModel.call 阻碍式调用
    private Flux<AiChatMessageSendRespVO>  tttt(){
        String model = "Qwen/Qwen3-8B";
        Double temperature = Double.valueOf(0.7f);
        Integer maxTokens = Integer.valueOf(512);
        DeepSeekChatModel openAiChatModel = DeepSeekChatModel.builder()
                .deepSeekApi(DeepSeekApi.builder()
                        .baseUrl("https://api.siliconflow.cn")
                        .apiKey("sk-wqbrhkcpzfpkddnsxvylxbrifwpfqshmwxpnmwqoaiobaxfe") // apiKey
                        .build())
                .defaultOptions(DeepSeekChatOptions.builder()
                        .model(model) // 模型
//                    .model("deepseek-ai/DeepSeek-R1") // 模型（deepseek-ai/DeepSeek-R1）可用赠费
                        .temperature(temperature)
                        .build())
                .build();

        UserMessage message_user = UserMessage.builder()
                .text("你好")
                .build();
        SystemMessage message_sys = SystemMessage.builder()
                .text("请用中文回答")
                .build();
        List<Message> messages = Arrays.asList(
                message_sys,
                message_user
        );


        ChatOptions ops = DeepSeekChatOptions.builder()
                .model(model)
                .temperature(temperature)
                .maxTokens(maxTokens)
                //.toolCallbacks(toolCallbacks)
                //.toolContext(toolContext)
                .build();

/*
.toolCallbacks(toolCallbacks) 用于定义AI模型调用外部工具时的回调处理逻辑,
                                比如当AI模型决定调用外部工具（如API、数据库等）时触发 

* */
        // 构造用户输入
        Prompt prompt = new Prompt(messages);

        //return testCall(openAiChatModel,prompt);
        return testStream(openAiChatModel,prompt);

    }
    private Flux<AiChatMessageSendRespVO>  testCall(DeepSeekChatModel chatModel,Prompt prompt) {
        ChatResponse response = chatModel.call(prompt) ;

        // 打印结果
        System.out.println(response);
        return null;
    }
    public Flux<AiChatMessageSendRespVO>  testStream(DeepSeekChatModel chatModel,Prompt prompt) {
        // 调用
        Flux<ChatResponse> flux = chatModel.stream(prompt);
        List<StringBuffer> stores = new ArrayList<>();
        stores.add(new StringBuffer());
        stores.add(new StringBuffer());
        stores.add(new StringBuffer());
        Flux<AiChatMessageSendRespVO> fluxmsg =  flux.scan(stores, (buffers, response) -> {
            // 累积内容   此处 buffers 就是 stores
            String content = AiUtils.getChatResponseContent(response);
            String reasoning = AiUtils.getChatResponseReasoningContent(response);
            if (!isEmpty(content)) buffers.get(0).append(content);
            if (!isEmpty(reasoning)) buffers.get(1).append(reasoning);
            if (isStreamStop(response)) buffers.get(2).append("stop");

            System.out.println(buffers);
            return buffers;
        }).filterWhen(buffers ->{
                    if (String.valueOf("stop").equals(buffers.get(2).toString()))
                    {
                       return Mono.just( true );
                    }
                    return Mono.just( false );
                })
                .map(buffers -> {
                    // 构建最终响应对象
                    AiChatMessageSendRespVO vo = new AiChatMessageSendRespVO();
                    vo.getReceiveMessage().setContent(buffers.get(0).toString());
                    vo.getReceiveMessage().setReasoningContent(buffers.get(1).toString());
                    return vo;
                });


        return fluxmsg;
    }

    public Flux<AiChatMessageSendRespVO>  testStream1(DeepSeekChatModel chatModel,Prompt prompt) {
        // 准备参数
        //List<Message> messages = new ArrayList<>();
        //messages.add(new SystemMessage("你是一个优质的文言文作者，用文言文描述着各城市的人文风景。"));
        //messages.add(new UserMessage("1 + 1 = ？"));


        StringBuffer contentBuffer = new StringBuffer();
        StringBuffer reasoningContentBuffer = new StringBuffer();


        // 调用
        Flux<ChatResponse> flux = chatModel.stream(prompt);
        Flux<AiChatMessageSendRespVO> fluxmsg =  flux.doOnNext(
                response -> {
                    //System.out.println(response);
                    //System.out.println(response.getResult().getOutput());

                    String newContent = AiUtils.getChatResponseContent(response);
                    String newReasoningContent = AiUtils.getChatResponseReasoningContent(response);
                    if (!isEmpty(newContent)) {
                        contentBuffer.append(newContent);
                    }
                    if (!isEmpty(newReasoningContent)) {
                        reasoningContentBuffer.append(newReasoningContent);
                    }

                    if ( isStreamStop(response) ){
                        System.out.println(contentBuffer.toString());
                        System.out.println(reasoningContentBuffer.toString());

                    }

                }
        ).map(response->{
            AiChatMessageSendRespVO vo = new AiChatMessageSendRespVO();
            String newContent = AiUtils.getChatResponseContent(response);
            String newReasoningContent = AiUtils.getChatResponseReasoningContent(response);
            if (!isEmpty(newContent)) {
                String s = vo.getReceiveMessage().getContent();
                if ( s==null){
                    s = new String();
                }
                s =  s.concat(newContent)  ;
                vo.getReceiveMessage().setContent(s);
             }
            if (!isEmpty(newReasoningContent)) {
                String s = vo.getReceiveMessage().getReasoningContent();

                if ( s==null){
                    s = new String();
                }
                s =  s.concat(newReasoningContent)  ;
                vo.getReceiveMessage().setReasoningContent(s);
            }
            return vo;
        });
        return fluxmsg;
        /*
        map()  类似于 Java Stream 的 map()，对 Flux 流中的每个元素应用给定的函数，生成新的元素并继续向下游传递
        doOnNext() 对流中的每个元素执行副作用操作（如打印日志），但不改变流的内容
        then() 在上游流完成后，返回一个新的Mono<Void>， 表示后续可以连接其他操作
        block() 方法是一个阻塞式操作，它会‌同步等待‌流处理完成并返回最终结果（或抛出异常）
        * */


    }

    public boolean isStreamStop(ChatResponse response) {
        Set<String> stopkeys = new HashSet<String>(Arrays.asList("STOP" ));

        if ( response.hasFinishReasons(stopkeys))
        {
            return true;
        }

        return false;
    }
/*

 */
    public boolean isEmpty(CharSequence str) {
        return str == null || str.length() == 0;
    }
/*

ChatModel的标准接口定义中（如Spring AI或LangChain），主要提供以下方法：
    invoke()：同步调用模型获取完整响应（LangChain常用）
    call()：同步调用模型（Spring AI常用）
    stream()：流式响应处理
    generate()：批量消息处理



*
* */
}
