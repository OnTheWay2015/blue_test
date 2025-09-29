package com.fqwz.springbootproj.normal_ai;



import java.time.LocalDateTime;
import java.util.List;


public class AiChatMessageSendRespVO {

    //@Schema(description = "发送消息", requiredMode = Schema.RequiredMode.REQUIRED)
    private Message send;

    //@Schema(description = "接收消息", requiredMode = Schema.RequiredMode.REQUIRED)
    private Message receive;

    public AiChatMessageSendRespVO(){
        this.send = new Message();
        this.receive = new Message();

    }
    public Message getSendMessage() { return this.send; }
    public Message getReceiveMessage() { return this.receive; }


    //@Schema(description = "消息")
    //////@Data
    public static class Message {

        //@Schema(description = "编号", requiredMode = Schema.RequiredMode.REQUIRED, example = "1024")
        private Long id;

        //@Schema(description = "消息类型", requiredMode = Schema.RequiredMode.REQUIRED, example = "role")
        private String type; // 参见 MessageType 枚举类

        //@Schema(description = "聊天内容", requiredMode = Schema.RequiredMode.REQUIRED, example = "你好，你好啊")
        private String content;

        //@Schema(description = "推理内容", example = "要达到这个目标，你需要...")
        private String reasoningContent;

        //@Schema(description = "知识库段落编号数组", example = "[1,2,3]")
        private List<Long> segmentIds;

        //@Schema(description = "知识库段落数组")
        //private List<AiChatMessageRespVO.KnowledgeSegment> segments;

        //@Schema(description = "联网搜索的网页内容数组")
        //private List<AiWebSearchResponse.WebPage> webSearchPages;

        //@Schema(description = "创建时间", requiredMode = Schema.RequiredMode.REQUIRED)
        private LocalDateTime createTime;


        public Long getId() { return this.id; }
        public String getContent() { return this.content; }
        public String getReasoningContent() { return this.reasoningContent; }



        public void setId(Long id) { this.id = id; }
        public void setContent(String content) { this.content = content; }
        public void setReasoningContent(String reasoningContent) { this.reasoningContent = reasoningContent; }


    }

}
