package com.vintage.vintagechess;

import android.content.Intent;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by Glytz on 2016-11-14.
 */

public class ErrorCallback {
    public static ActivityCreateGame activityCreateGame;


    public static RequestCallback getPostNewGameCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGame) {
                activityCreateGame.openGame();
            }
        };
        return ret;
    }

    public static RequestCallback getPostGameStartCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGamee) {
                HttpRunner.runGetStatusBoard(null, activityGame, null);
            }
        };
        return ret;
    }
    public static RequestCallback getGetGameDetailsCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGame) {

            }
        };
        return ret;
    }



}
