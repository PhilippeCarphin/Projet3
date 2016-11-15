package com.vintage.vintagechess;
import android.content.Intent;

import org.json.JSONException;
import org.json.JSONObject;


/**
 * Created by User on 2016-11-10.
 */

public class Callbacks {


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

    public static RequestCallback getPostGameEndCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

                Intent setIntent = new Intent(Utilities.currentActivity,ActivityMenu.class);
                Utilities.currentActivity.startActivity(setIntent);
            }
        };
        return ret;
    }

    public static RequestCallback getPostMoveCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) throws JSONException {
                REST.handleMoveResponse(response);
            }
        };
        return ret;
    }

    public static RequestCallback getPostPromoteCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {


            }
        };
        return ret;
    }

    public static RequestCallback getGetTimeCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

            }
        };
        return ret;
    }

    public static RequestCallback getGetStatusSummaryCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) throws JSONException {
                REST.updateGameFromStatusSummary(response);
                Game.finishMove();
            }
        };
        return ret;
    }

    public static RequestCallback getGetStatusBoardCallback() {
        RequestCallback ret = new RequestCallback()  {
            @Override
            public void runResponse(String response) throws JSONException {
                Display.newPos = null;
                Display.lastPos = null;
                JSONObject jsonObject = new JSONObject(response);
                Game.setConfig(REST.getPiecesFromStatusBoard(jsonObject));
                String white = jsonObject.getString("turn");
                Game.activityGame.setWhoseTurn(white);
                Game.activityGame.setMoveNumberText(jsonObject.getString("move_no"));

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
