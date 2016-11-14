package com.vintage.vintagechess;

import android.app.DownloadManager;

import org.json.JSONException;

/**
 * Created by User on 2016-11-08.
 */

public class HttpRunner {

    private static void runRequest(final String suffix, final String method, final String body,final RequestCallback onSucessCallBack,final RequestCallback onFailCallBack, final boolean receiveJSON) {
        new RequestTask(onSucessCallBack,onFailCallBack, receiveJSON, Game.ip + suffix, method, body).execute();
    }


    public static void runPostNewGame(RequestCallback OnSucessCallback, RequestCallback onFailCallBack) throws JSONException {
        //TODO
        String suffix = "/new_game";
        String method = "POST";
        String body = REST.postNewGame(); //aller chercher le json
        RequestCallback callback = Callbacks.getPostNewGameCallback();
        runRequest(suffix, method, body, OnSucessCallback, onFailCallBack, false);
    }

    public static void runPostGameStart(RequestCallback onFailCallback) throws JSONException {
        //TODO
        String suffix = "/game_start";
        String method = "POST";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getPostGameStartCallback();
        runRequest(suffix, method, body, callback,onFailCallback, false);
    }

    public static void runPostGameEnd(RequestCallback onFailCallback) throws JSONException {
        //TODO
        String suffix = "/game_end";
        String method = "POST";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getPostGameEndCallback();
        runRequest(suffix, method, body, callback, onFailCallback, false);
    }

    public static void runPostMove(boolean isWhite, int x1, int y1, int x2, int y2, RequestCallback onFailCallback) {
        //TODO
        String suffix = "/move/" + (isWhite ? "1" : "2") + "/" + (char)(x1+97) +Integer.valueOf(y1+1).toString() +
                "-" + (char)(x2+97) +Integer.valueOf(y2+1).toString();
        String method = "POST";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getPostMoveCallback();
        runRequest(suffix, method, body, callback,onFailCallback, true);
    }

    public static void runPostPromote(RequestCallback onFailCallback) {
        //TODO
        String suffix = "/promote/"; //pas fini
        String method = "GET";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getPostPromoteCallback();
        runRequest(suffix, method, body, callback,onFailCallback, false);
    }

    public static void runGetTime(boolean isWhite,RequestCallback onFailCallback) {
        //TODO
        String suffix = "/time/" + (isWhite ? "1" : "2");
        String method = "GET";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getGetTimeCallback();
        runRequest(suffix, method, body, callback, onFailCallback, false);
    }

    public static void runGetStatusSummary(RequestCallback onFailCallback) {
        //TODO
        String suffix = "/status/summary";
        String method = "GET";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getGetStatusSummaryCallback();
        runRequest(suffix, method, body, callback, onFailCallback, true);
    }

    public static void runGetStatusBoard(RequestCallback onFailCallback) {
        //TODO
        String suffix = "/status/board";
        String method = "GET";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getGetStatusBoardCallback();
        runRequest(suffix, method, body, callback,onFailCallback, true);
    }

    public static void runGetGameDetails(RequestCallback onFailCallback) {
        //TODO
        String suffix = "/game_details";
        String method = "GET";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getGetGameDetailsCallback();
        runRequest(suffix, method, body, callback,onFailCallback, true);
    }


}
