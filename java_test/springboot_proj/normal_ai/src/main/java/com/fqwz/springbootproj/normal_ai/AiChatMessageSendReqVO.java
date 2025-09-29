package com.fqwz.springbootproj.normal_ai;

 
 


import javax.validation.constraints.NotNull;
import java.util.List;

//@Schema(description = "管理后台 - AI 聊天消息发送 Request VO")
//////@Data
public class AiChatMessageSendReqVO {

    //@Schema(description = "聊天对话编号", requiredMode = Schema.RequiredMode.REQUIRED, example = "1024")
    @NotNull(message = "聊天对话编号不能为空")
    private Long conversationId;

    //@Schema(description = "聊天内容", requiredMode = Schema.RequiredMode.REQUIRED, example = "帮我写个 Java 算法")
    //@NotEmpty(message = "聊天内容不能为空")
     private String content;

    //@Schema(description = "是否携带上下文", example = "true")
    private Boolean useContext;

    //@Schema(description = "是否联网搜索", example = "true")
    private Boolean useSearch;

    //@Schema(description = "附件 URL 数组", example = "https://www.iocoder.cn/1.png")
    private List<String> attachmentUrls;

}
