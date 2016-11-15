package com.vintage.vintagechess;
import android.content.Intent;

import org.json.JSONException;
import org.json.JSONObject;


/**
 * Created by User on 2016-11-10.
 */

public class Callbacks {

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
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGame) {
                HttpRunner.runGetStatusBoard(activityCreateGame, activityGame, null);
            }
        };
        return ret;
    }

    public static RequestCallback getPostGameEndCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGame) {

                Intent setIntent = new Intent(Utilities.currentActivity,ActivityCreateGame.class);
                Utilities.currentActivity.startActivity(setIntent);
            }
        };
        return ret;
    }

    public static RequestCallback getPostMoveCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGame) throws JSONException {
                REST.handleMoveResponse(response, activityGame);
            }
        };
        return ret;
    }

    public static RequestCallback getPostPromoteCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGame) {


            }
        };
        return ret;
    }

    public static RequestCallback getGetTimeCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGame) {

            }
        };
        return ret;
    }

    public static RequestCallback getGetStatusSummaryCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGame) throws JSONException {
                REST.updateGameFromStatusSummary(response, activityGame);
                activityGame.game.finishMove();
            }
        };
        return ret;
    }

    public static RequestCallback getGetStatusBoardCallback() {
        RequestCallback ret = new RequestCallback()  {
            @Override
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGame) throws JSONException {
                JSONObject jsonObject = new JSONObject(response);
                activityGame.game.setConfig(REST.getPiecesFromStatusBoard(jsonObject));
                String white = jsonObject.getString("turn");
                activityGame.setWhoseTurn(white);
                activityGame.setMoveNumberText(jsonObject.getString("move_no"));
                activityGame.game.display.drawFullBoard();

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
