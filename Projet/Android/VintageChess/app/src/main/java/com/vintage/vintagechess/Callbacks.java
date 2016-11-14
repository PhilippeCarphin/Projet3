package com.vintage.vintagechess;
import android.support.v7.app.AppCompatActivity;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import java.util.ArrayList;
import java.util.LinkedList;


/**
 * Created by User on 2016-11-10.
 */

public class Callbacks {


    public static ActivityCreateGame activityCreateGame;


    public static RequestCallback getPostNewGameCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {
                activityCreateGame.openGame();
            }
        };
        return ret;
    }

    public static RequestCallback getPostGameStartCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {
                HttpRunner.runGetStatusBoard();
            }
        };
        return ret;
    }

    public static RequestCallback getPostGameEndCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

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
                REST.getStatusSummary(response);
            }
        };
        return ret;
    }

    public static RequestCallback getGetStatusBoardCallback() {
        RequestCallback ret = new RequestCallback()  {
            @Override
            public void runResponse(String response) throws JSONException {
                JSONObject jsonObject = new JSONObject(response);
                Game.setConfig(REST.getStatusBoard(jsonObject));
                String white = jsonObject.getString("turn");
                Display.setWhoseTurn(white);
                Display.setMoveNumber(jsonObject.getString("move_no"));
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
