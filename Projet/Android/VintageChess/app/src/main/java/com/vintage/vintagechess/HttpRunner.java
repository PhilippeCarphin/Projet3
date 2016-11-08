package com.vintage.vintagechess;

/**
 * Created by User on 2016-11-08.
 */

public class HttpRunner {
    private static void runRequest(String suffix, String method, String body) {
        new RequestTask().execute(suffix, method, body);
    }

    public static void runPostNewGame() {
        //TODO
        String suffix = "/new_game";
        String method = "POST";
        String body = ""; //aller chercher le json
        runRequest(suffix, method, body);
    }

    public static void runPostGameStart() {
        //TODO
        String suffix = "/game_start";
        String method = "POST";
        String body = ""; //aller chercher le json
        runRequest(suffix, method, body);
    }

    public static void runPostGameEnd() {
        //TODO
        String suffix = "/game_end";
        String method = "POST";
        String body = ""; //aller chercher le json
        runRequest(suffix, method, body);
    }

    public static void runPostMove(boolean isWhite, int x1, int y1, int x2, int y2) {
        //TODO
        String suffix = "/move/" + (isWhite ? "1" : "2") + (char)(x1+97) +Integer.valueOf(y1).toString() +
                "-" + (char)(x2+97) +Integer.valueOf(y2).toString();
        String method = "POST";
        String body = ""; //aller chercher le json
        runRequest(suffix, method, body);
    }

    public static void runPostPromote() {
        //TODO
        String suffix = "/promote/"; //pas fini
        String method = "GET";
        String body = ""; //aller chercher le json
        runRequest(suffix, method, body);
    }

    public static void runGetTime(boolean isWhite) {
        //TODO
        String suffix = "/time/" + (isWhite ? "1" : "2");
        String method = "GET";
        String body = ""; //aller chercher le json
        runRequest(suffix, method, body);
    }

    public static void runGetStatusSummary() {
        //TODO
        String suffix = "/status/summary";
        String method = "GET";
        String body = ""; //aller chercher le json
        runRequest(suffix, method, body);
    }

    public static void runGetStatusBoard() {
        //TODO
        String suffix = "/status/board";
        String method = "GET";
        String body = ""; //aller chercher le json
        runRequest(suffix, method, body);
    }

    public static void runGetGameDetails() {
        //TODO
        String suffix = "/game_details";
        String method = "GET";
        String body = ""; //aller chercher le json
        runRequest(suffix, method, body);
    }


}
