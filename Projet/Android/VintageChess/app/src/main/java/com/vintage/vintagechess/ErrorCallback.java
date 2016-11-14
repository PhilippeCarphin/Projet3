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
            public void runResponse(String response) {
                Game.initializeVariables();
                activityCreateGame.openGame();
                Game.clearPieces();
            }
        };
        return ret;
    }

    public static RequestCallback getPostGameStartCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {
                Game.initializeVariables();
                HttpRunner.runGetStatusBoard(null);
            }
        };
        return ret;
    }
    public static RequestCallback getGetGameDetailsCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

            }
        };
        return ret;
    }



}
