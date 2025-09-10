package com.fqwz.springbootproj.constant;

public enum MessageChannel {
    MSG_C1(1, "MSG_C1"),
    MSG_C2(2, "MSG_C2");

    private final Integer id;
    private final String name;

    MessageChannel(Integer id, String name) {
        this.id = id;
        this.name = name;
    }

    public Integer getId() {
        return id;
    }

    public String getName() {
        return name;
    }
}
