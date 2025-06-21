package org.example;

import Server.SimpleServer;

import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        System.out.println("Hello world!");
        //server
        SimpleServer S = new SimpleServer();
        try{
            S.Start();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}