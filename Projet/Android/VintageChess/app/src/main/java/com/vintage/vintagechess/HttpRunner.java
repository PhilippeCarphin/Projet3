package com.vintage.vintagechess;

import android.app.ProgressDialog;

import org.json.JSONException;

/**
 * Created by User on 2016-11-08.
 */

public class HttpRunner {

    private static void runRequest(final ActivityCreateGame activityCreateGame,final ActivityGame activityGame,final String suffix, final String method, final String body,final RequestCallback onSucessCallBack,final RequestCallback onFailCallBack, final boolean receiveJSON) {
        new RequestTask(activityCreateGame, activityGame,onSucessCallBack,onFailCallBack, receiveJSON, GameConfig.ip + suffix, method, body).execute();
    }


    public static void runPostNewGame(ActivityCreateGame activityCreateGame, ActivityGame activityGame, RequestCallback OnSucessCallback, RequestCallback onFailCallBack) throws JSONException {
        //TODO
        String suffix = "/new_game";
        String method = "POST";
        String body = REST.postNewGame(); //aller chercher le json
        RequestCallback callback = Callbacks.getPostNewGameCallback();
        runRequest(activityCreateGame, activityGame, suffix, method, body, OnSucessCallback, onFailCallBack, false);
    }

    public static void runPostGameStart(ActivityCreateGame activityCreateGame, ActivityGame activityGame,RequestCallback onFailCallback) throws JSONException {
        //TODO
        String suffix = "/game_start";
        String method = "POST";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getPostGameStartCallback();
        runRequest(activityCreateGame, activityGame, suffix, method, body, callback,onFailCallback, false);
    }

    public static void runPostGameEnd(ActivityCreateGame activityCreateGame, ActivityGame activityGame,RequestCallback onFailCallback) throws JSONException {
        //TODO
        String suffix = "/game_end";
        String method = "POST";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getPostGameEndCallback();
        runRequest(activityCreateGame, activityGame, suffix, method, body, callback, onFailCallback, false);
    }

    public static void runPostMove(ActivityCreateGame activityCreateGame, ActivityGame activityGame,boolean isWhite, int x1, int y1, int x2, int y2, RequestCallback onFailCallback) {
        //TODO
        String suffix = "/move/" + (isWhite ? "1" : "2") + "/" + (char)(x1+97) +Integer.valueOf(y1+1).toString() +
                "-" + (char)(x2+97) +Integer.valueOf(y2+1).toString();
        String method = "POST";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getPostMoveCallback();
        runRequest(activityCreateGame, activityGame, suffix, method, body, callback,onFailCallback, true);
    }

    public static void runPostPromote(ActivityCreateGame activityCreateGame, ActivityGame activityGame,RequestCallback onFailCallback) {
        //TODO
        String suffix = "/promote/"; //pas fini
        String method = "GET";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getPostPromoteCallback();
        runRequest(activityCreateGame, activityGame, suffix, method, body, callback,onFailCallback, false);
    }

    public static void runGetTime(ActivityCreateGame activityCreateGame, ActivityGame activityGame,boolean isWhite,RequestCallback onFailCallback) {
        //TODO
        String suffix = "/time/" + (isWhite ? "1" : "2");
        String method = "GET";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getGetTimeCallback();
        runRequest(activityCreateGame, activityGame, suffix, method, body, callback, onFailCallback, false);
    }

    public static void runGetStatusSummary(ActivityCreateGame activityCreateGame, ActivityGame activityGame,RequestCallback onFailCallback) {
        //TODO
        String suffix = "/status/summary";
        String method = "GET";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getGetStatusSummaryCallback();
        runRequest(activityCreateGame, activityGame, suffix, method, body, callback, onFailCallback, true);
    }

    public static void runGetStatusBoard(ActivityCreateGame activityCreateGame, ActivityGame activityGame,RequestCallback onFailCallback) {
        //TODO
        String suffix = "/status/board";
        String method = "GET";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getGetStatusBoardCallback();
        runRequest(activityCreateGame, activityGame, suffix, method, body, callback,onFailCallback, true);
    }

    public static void runGetGameDetails(ActivityCreateGame activityCreateGame, ActivityGame activityGame,RequestCallback onFailCallback) {
        //TODO
        String suffix = "/game_details";
        String method = "GET";
        String body = ""; //aller chercher le json
        RequestCallback callback = Callbacks.getGetGameDetailsCallback();
        runRequest(activityCreateGame, activityGame, suffix, method, body, callback,onFailCallback, true);
    }


}
