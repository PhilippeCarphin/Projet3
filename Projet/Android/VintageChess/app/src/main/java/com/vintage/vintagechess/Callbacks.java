package com.vintage.vintagechess;

import android.util.Log;

/**
 * Created by User on 2016-11-10.
 */

public class Callbacks {
    public static ActivityCreateGame activityCreateGame;

    public static RequestCallback getPostNewGameCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {
                try {
                    Log.d("response", "\n\n\n\n" +response +"\n\n\n\n");
                    if (!response.contains("200")) throw new Exception("Failed to create new game");
                    activityCreateGame.openGame();
                }
                catch (Exception e) {
                    e.printStackTrace();
                    Utilities.messageBox("Error while creating a new game", e.getMessage(), activityCreateGame);

                }
            }
        };
        return ret;
    }

    public static RequestCallback getPostGameStartCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

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
            public void runResponse(String response) {

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
            public void runResponse(String response) {

            }
        };
        return ret;
    }

    public static RequestCallback getGetStatusBoardCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

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
